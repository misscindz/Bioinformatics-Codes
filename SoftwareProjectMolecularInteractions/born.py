#!/usr/bin/python
# -*- coding: utf-8 -*-


import re,sys
import numpy as np

from supplement import radii
from supplement import empiricalCoefficient
from supplement import empiricalH
from supplement import rcoval
from dispersion import getCoordinationNumber
from electrostatics import calculatePartialCharge
from calc_neighbour import getNeighborMatrix
from copy import deepcopy

sys.dont_write_bytecode = True

#coordinate_datei parsen
def read_coors(fin):
    """
    given the file name
    return a matrix of coordinates, a list of atom types
    and a integer of the number of atoms in the molecule
    which is in the file
    """
    fin = open(fin)
    natom = int(fin.readline())
    title = fin.readline()[:-1]
    coords = np.zeros([natom,3], dtype = "float64")
    atomtypes = []
    for x in coords:
        line = fin.readline().split()
        atomtypes.append(line[0])
        x[:] = map(float,line[1:4])
    fin.close()
    return (coords,atomtypes,natom)

#rcov_datei parsen
def readcov(fin):
    """
    Given a file name
    Return a list of covalent radius
    """
    
    rcov_file = open(fin,"r")
    pre_rcov = []
    rcov = []
    for line in rcov_file.readlines():
        if re.match(r"^\s+\.",line):
            r_line = line.split()
            pre_rcov.append(r_line[1:6])

    for i in range (0,len(pre_rcov)):
        for j in range (0,len(pre_rcov[i])):
            new_rcov = pre_rcov[i][j].split(',')
            rcov.append(new_rcov[0])
    for n in range(0,len(rcov)):
        if re.match('\D',rcov[n]):
            rcov.pop(n)
    #print rcov
    rcov_file.close()
    return rcov


#elements_datei parsen
def read_elem(fin):
    """
    given the file name
    return a vector of elements
    """
    elem_file = open(fin)
    elements = []
    for line in elem_file.readlines():
        if re.match(r"^'",line):
            line = line.rstrip('\n')
            line = re.sub(r"'","",line)
            line = re.sub(r" ",'',line)
            elements = line.split(",")
    
    
    elem_file.close()
    return elements

#calculate the distance
def calculateDistances(vectors):
    """ read a xyz file from file handle
    Parameters
    ----------
    vectors : List of vectors
    List containing of which the pairwise distanze is
    calculated
    Returns
    -------
    matrix with pairwise distances
    """
    numberOfVectors = len(vectors)
    matrix = np.zeros(shape=(numberOfVectors,numberOfVectors))
    
    for i in range(numberOfVectors):
        for j in range(numberOfVectors):
            vector1 = vectors[i]*1.889725989
            vector2 = vectors[j]*1.889725989
            distance = np.linalg.norm(vector1-vector2)
            matrix[i][j] = distance
    return matrix



def calculateA(i, j):
    a = 0.0
    dij = 0.0
    dij = rcoval[i] + rcoval[j]
    a = (np.power(radii[j], 2) - np.power((radii[i] - dij), 2))/ (2*dij)

    return a
    
    
# function for calculating size of the spheres of every atom
def calculateSpheres(natom,elements,neighbors, atomtypes, rcov):
    
    #creating spheres vector
    num_spheres = len(atomtypes)
    spheres = np.zeros(shape = (num_spheres))
    
    # vi - vector for sphere calculation
    vi_values = np.zeros(shape = (num_spheres))

    # defining some variables
    value1 = 1.0/3.0
    ri = 0
    rj = 0
    aij = 0.0
    aji = 0.0

    for i in range(0,natom):
        ri = elements.index(atomtypes[i])
        for j in range (0,natom):
            if neighbors[i][j] == 1:
                rj = elements.index(atomtypes[j])
		aij = 0.0
		aji = 0.0
		aij = calculateA(elements[ri] , elements[rj])
		aji = calculateA(elements[rj] , elements[ri])
		vi_values[i] += (np.power(aij, 2)* \
		(3*radii[elements[ri]] - aij)) + np.power(aji, 2)* \
		(3*radii[elements[rj]] - aji)
        # calculate actual vi value out of aggregated pre_vi's
	vi_values[i] = vi_values[i] * 1./8
        vi_values[i] = radii[elements[ri]]**3 - vi_values[i]
	
	if vi_values[i] < 0 :
	    vi_values[i] = abs(vi_values[i]) **1./3
	    vi_values[i] = vi_values[i] * -1
	else:
	    vi_values[i] = vi_values[i] **1./3
	
	    
        # final calculation of sphere values
	vi_values[i] = max(0, vi_values[i])
        spheres[i] = vi_values[i]*0.95

    return spheres
   
 
# calculation of L. Helper function for calculateV
def calculateL(r, x, S):
    
    t = 3.0/2.0
    l = 0.0
    l = 1.0/(4.0 * r * np.power(x, 2))
    l = l - (1.0 / (3 * np.power(x, 3)))
    l = l + ((np.power(r, 2)- np.power(S, 2)) / \
	    (8.0 * r * np.power(x, 4)))
    l = t * l
    
    return l


# calculation of V. Helper function for calculateFormal
def calculateV(r, R, S):
    # defining some values
    x_1 = r + S
    x_2 = r - S
    x_max = max(R, x_2)
    
    # V is dependent on a 3-case scenario:

    # case 1
    if (abs(R - S) < r):
        v = 0.0
        l_1 = calculateL(r, x_1, S)
        l_2 = calculateL(r, x_max, S)
        v = l_1 - l_2

    # case 2
    if 0 <= r and r <= (R - S):
        v = 0.0

    # case 3
    if 0 <= r and r <= (S - R):
        v = 0.0
        l_3 = calculateL(r, x_1, S)
        l_4 = calculateL(r, x_2, S)
        v = (l_3 - l_4) + np.power(R, -3.0)
    return v
   
# function fo calulation of the formal integration radii
def calculateFormal(natom,atomtypes,distance, elements, spheres):
    
    # creating vector
    num_formal = len(spheres)
    formal = np.zeros(shape = (num_formal))
    # setting variables
    exp = 1.0/3.0
    dist = 0.0
    ri = 0
    rj = 0
    # calculation
    for i in range(0,natom):
        ri = elements.index(atomtypes[i])
	for j in range (0,natom):
	    if i != j:
		rj = elements.index(atomtypes[j])
		dist = distance[i][j]
		# using calculateV to calculate values
		formal[i] += calculateV(dist, 
					radii[elements[ri]], 
					spheres[j])
	formal[i] = (np.power(radii[elements[ri]], -3.0)) - formal[i]
	formal[i] = formal[i] **  (1. /3)
	formal[i] = formal[i] * -1.0
	formal[i] = formal[i] / 2
	
    return formal


# calculation of SGB(t) . helper function for calulateFinal
def calculateSGB(t):

  sgbt = 0.0
  sgbt = 1 / np.sqrt(1 + np.power(t, -1.0) * np.exp(-t) / 4.0)
  
  return sgbt
  
# final calcualation of Born Energy calculation
def calculateFinal(	natom, 
			atomtypes, 
			distance,
			formal,
			elements, 
			partialCharge,
			empiricalCoeff):

    # setting variables
    tau = 0.0
    final = 0.0
    value1 = 0.0
    value2 = 0.0
    value3 = 0.0

    #dielectric constants for solvent medium.
    dielectricInterior = 1.0    
    # water = 78, this can be changed to preferred  solvent
    dielectricSolvent = 78.0

    #calculation of "tau"-factor
    tau = 	np.power(dielectricInterior, -1) - \
		np.power(dielectricSolvent, -1)
  
    # 3 values are calculated for the final calculation

    # calculating value 1
    for i in range(0,natom):
        value1 += (np.power(partialCharge[i],2) * formal[i])

    # calculating value 2
    for i in range(0,natom):
        for j in range (0,natom):
            if i < j:
                t = 0.0
                t = np.power(distance[i][j],2) * formal[i] * formal[j]
                sgbt = calculateSGB(t)
                value2 +=(partialCharge[i] * partialCharge[j] * sgbt)/ \
                        (distance[i][j])
    
    # calculating value 3
    for i in range(0,natom):
	ri = elements.index(atomtypes[i])
	value3 +=	empiricalCoeff[i] * \
			(np.power((2 * radii[elements[ri]] * 
			formal[i]), 3))
    # final calculation
    final = tau*(value1 - value2 + value3)
  
    return final

'''
# function for the integration of pi bonds in the calculation
def checkPiBond(	natom,
			atomtypes,
			distance,
			rcov,
			elements	):
    
    # adding elements for pibond calculation
    elements.append("C_pi")
    elements.append("N_pi")
    elements.append("O_pi")

    # adding covalent radii for easier pibond calculation
    rcov.append(rcov[5])
    rcov.append(rcov[6])
    rcov.append(rcov[7])

    # getting the coordination number of every atom
    coordNumber = getCoordinationNumber(	natom,
						atomtypes,
						distance,
						rcov,
						elements	)

    # check if any atom has coordinationnumber == 2 (pi bond) if so, 
    # radii and empirical coefficeint are changed
    for i in range(0,natom):
		if coordNumber[i] == 2 and(	atomtypes[i] == "C" or 
						atomtypes[i] == "N" or 
						atomtypes[i] == "O"):

			if atomtypes[i] == "C":
				atomtypes[i] = "C_pi"
			if atomtypes[i] == "N":
				atomtypes[i] = "N_pi"
			if atomtypes[i] == "O":
				atomtypes[i] = "O_pi"

'''

def createEmpirical(natom, neighbors, atomtypes):

    #creating empirical vector
    num_atomtypes = len(atomtypes)
    empiricalCoeff = np.zeros(shape= (num_atomtypes))


    for i in range(0,natom):
        for j in range(0,natom):
            if neighbors[i][j] > 0.8:
                if atomtypes[i]== "H":
                    empiricalCoeff[i] = empiricalH[atomtypes[j]]
                if atomtypes[j] == "H":
                    empiricalCoeff[j] = empiricalH[atomtypes[i]]

    for i in range(0,num_atomtypes):
        if empiricalCoeff[i] == 0.0:
            empiricalCoeff[i] = empiricalCoefficient[atomtypes[i]]

    return empiricalCoeff

#main function for calculating born energy
def calcBornEnergy(	natom, 
			atomTypes, 
			distance, 
			covalenceRadii, 
			elementsPeriodic, 
			partialCharge,
			neighbors		):
    # set to zero and float
    bornEnergy = 0.0

    # copying rcov, atomtypes and elements as they are manipulated
    rcov = deepcopy(covalenceRadii)
    atomtypes = deepcopy(atomTypes)
    elements = deepcopy(elementsPeriodic)
    '''
    # checking if a pi bond is present in the configuration
    checkPiBond(	natom,
			atomtypes,
			distance,
			rcov,
			elements	)

    '''
    empiricalCoeff = createEmpirical(natom,neighbors,atomtypes)

    # calculating the size of the spheres of every atom
    spheres = calculateSpheres(		natom,
					elements,
					neighbors, 
					atomtypes,
					rcov)

    # calculating the formal integration radii of every such sphere
    formal = calculateFormal(		natom,
					atomtypes,
					distance,
					elements, 
					spheres		)

    # final Born Energy calculation
    bornEnergy = calculateFinal(	natom, 
					atomtypes, 
					distance, 
					formal,
					elements, 
					partialCharge,
					empiricalCoeff)
					
    return bornEnergy
 
#call the functions for testing purposes
if __name__ == "__main__":

    
    f_coor = sys.argv[1]        #coordination file name
    f_rcov = sys.argv[2]        #covalent radius file name
    f_elem = sys.argv[3]        #chemical elements file name
    coor,atomtypes, natom = read_coors(f_coor)
    distance = calculateDistances(coor)
    rcov = readcov(f_rcov)
    elements = read_elem(f_elem)
    neighbours = getNeighborMatrix(natom,atomtypes,distance,rcov,elements)
    useCM5Correction = 0
    partialCharge = calculatePartialCharge(
                natom,
                atomtypes,
                elements,
                distance,
                useCM5Correction)
    born = calcBornEnergy(natom,atomtypes,distance,rcov,elements,partialCharge,neighbours)
    print born
    
    
