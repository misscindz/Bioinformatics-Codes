#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
""" Calculates Electrostatic Energy
	
	Parameters
    ----------
	natoms			: total number of atoms (vector)
	enVector		: electronegativities (vector)
	valenceVector	: valence electrons (vector)
	distanceMatrix	: distances between atoms (2-dim matrix)
	
    Returns
    -------
	U_electrostatics: electrostatic Energy (vector)	
	"""	
'''
import sys; sys.dont_write_bytecode = True
import math
import argparse
import numpy as np
from constants import alpha
from constants import DZ_params
from constants import get_T
from constants import valence_dict
from constants import en_dict
from cm5radii import r_in_AA
from cm5radii import r_in_AU
from cm5radii import AA2AU
from calc_distance import calculateDistances
from dispersion import read_coors
from dispersion import read_elem
from read_rcov import readCov
from change_unit import convertAtomicToAngstrom
from read_xyz import readXYZSplit

#-------FUNCTIONS FOR DATASTRUCTURE CREATION------------------------------------	

def calcEnVector(atomTypes):
	enVector = []
	for atom in atomTypes:
		enVector.append(en_dict[atom.upper()])
	return enVector 
	
def calcValenceVector(atomTypes):
	valenceVector = []
	for atom in atomTypes:
		valenceVector.append(valence_dict[atom.upper()])
	return valenceVector 
	
def getCovVector_AU(
			atomtypes,
			elements):
	cov_vec = []
	for atom in atomtypes:
		cov_vec.append(r_in_AU[elements.index(atom)+1])	
	return cov_vec
	
def getCovVector_AA(
			atomtypes,
			elements):
	cov_vec = []
	for atom in atomtypes:
		cov_vec.append(r_in_AA[elements.index(atom)+1])	
	return cov_vec
	
def getCovVector_old(atomtypes,elements):
	cov_vec = []
	rcovs=readCov("data/sw_project_data/rcov.dat")
	for atom in atomtypes:
		cov_vec.append((float(rcovs[elements.index(atom)+1]))/(4.0/3.0)) 
	return cov_vec #all in AU

#-------FUNCTIONS FOR CM5-CORRECTION---------------------------------------------

def getProcuctTB(
			atom1_type,
			atom2_type,
			distance_atom1_atom2,
			covalenceRadius_atom1,
			covalenceRadius_atom2):

	T=get_T(atom1_type,atom2_type)
	B=math.exp((-1.0)*(alpha)*(float(distance_atom1_atom2)\
									- float(covalenceRadius_atom1)\
									- float(covalenceRadius_atom2)))

	return T*B
	
def cm5correction(
			partialCharges,
			natoms,
			atomTypes,
			distanceMatrix, #AA
			elements):
	
	
	corrPartialCharges = []
	covRadii = getCovVector_AA(atomTypes,elements)
	
	for atom1 in range(natoms):
		sum_TB = 0.0
		for atom2 in range(natoms):
			if atom1 != atom2:
				sum_TB += getProcuctTB(atomTypes[atom1],atomTypes[atom2],\
							distanceMatrix[atom1][atom2],covRadii[atom1],\
							covRadii[atom2])
		corrPartialCharges.append((partialCharges[atom1]+sum_TB))
	return corrPartialCharges
	
	
#-------FUNCTIONS FOR PARTIAL CHARGE CALCULATION--------------------------------

def f_damp(a,b,distMat,covRadii,n):
	#AU-covRadii 
	k1=16.0
	k2=4.0/3.0

	damp = 1.0/(1.0+math.exp(-k1*(k2*((float(covRadii[a])\
									+ float(covRadii[b])))\
									/ (float(distMat[a][b])*AA2AU)\
									- 1.0)))
	return damp
	
def calculatePartialCharge(
		natoms,
		atomtypes,
		elements,
		distMat, #AU
		correct=True):
	
	distMat= map(convertAtomicToAngstrom,distMat) #AA
		
	partialCharges = []
	covRadii = getCovVector_old(atomtypes,elements)	#AU skaliert
	enVector = calcEnVector(atomtypes)
	valenceVector = calcValenceVector(atomtypes)
	 
	for a in range(natoms):
		q_a = 0
		for b in range(natoms):
			if a != b:
				q_a += f_damp(a,b,distMat,covRadii,natoms)\
										* (enVector[b]-enVector[a])*0.08		
		partialCharges.append(q_a)
	if correct == True:
		covRadii = getCovVector_AA(atomtypes,elements)
		corrPartialCharges = cm5correction(partialCharges,natoms,atomtypes,distMat,elements)
		return corrPartialCharges
	return partialCharges
			

#-------FUNCTIONS FOR ELECTROSTATIC ENERGY CALCULATION--------------------------

def calculateElectrostaticEnergy(natoms,distanceMatrix,partialCharges):
	U_electrostatics = 0
	for i in range(natoms-1):
		for j in range(i+1,natoms):
				U_electrostatics +=(partialCharges[i]*partialCharges[j])\
					/ (distanceMatrix[i][j])
	return U_electrostatics*627.51
	

#-------MAIN FOR TESTING--------------------------------------------------------	
	
if __name__ == "__main__":

	elem_file = "elements.txt"
	energyDict = {}
	parser = argparse.ArgumentParser(
									prog="./electrostatic.py",
									usage="%(prog)s <moleculeFile> <option>",
									description="Calculates electrostatic"
									+"energy.")
	parser.add_argument("moleculeFile",type=str, nargs='?',
			default = "data/testDataEmmi/01_Water-Water.emmi",
			help= "Paths to the xyz-molecule-file for which the"
			+ " Energy shall be calculated.\nThe file must be in the fol"
			+ "lowing format:\n "
			+ "\n<AU|AA>\n<number of atoms>\nComment line\n"
			+ "<Molecule id of atom>")
	parser.add_argument("-ncm5","--noCM5Correction",
									action="store_false",
									default = True,
       								help="Disables CM5-corrrection.")
	parser.add_argument("-spc","--showPartialCharge",
									action="store_true",    
									default = False,
       								help="Disables printing of partial charges.")
	parser.add_argument("-noe","--noElectrostaticEnergy",
									action="store_false",    
									default = True,
       								help="Disables printing of energy.")	

	args = parser.parse_args()

	molecule = args.moleculeFile
	useCM5 = args.noCM5Correction
	spc = args.showPartialCharge
	noe = args.noElectrostaticEnergy
	
	with open(molecule, "r") as fin:
		atomPositionsDict, atomTypesDict, nAtmosDict, moleculeIdsDict, comment  = readXYZSplit(fin)
        
	for k in atomPositionsDict.keys():
		coords = np.array(atomPositionsDict[k])
		atomtypes = np.array(atomTypesDict[k])
		natoms = nAtmosDict[k]
		moleculeIds = moleculeIdsDict[k]
	

		fin.close()
		elements = read_elem(elem_file)
		distanceMatrix_AU = calculateDistances(coords) #AU
		distanceMatrix_AA = [ x/AA2AU for x in distanceMatrix_AU] #AA		
		partialCharges = calculatePartialCharge(natoms,atomtypes,elements,\
												distanceMatrix_AU,useCM5)
		electrostaticEnergy = calculateElectrostaticEnergy(natoms,\
											distanceMatrix_AU,partialCharges)
		energyDict[k] = electrostaticEnergy
	totalEnergy = 2*energyDict[0] - sum(energyDict.values())
	
	if spc:
		print "\n%s %20s" % ("atomtypes","partial Charges")
		print "------------------------------"
	
		for x in range(natoms):
			print "{0: >5s}".format(atomtypes[x]),\
	 						"{0: >20.3f}".format(partialCharges[x]),"\n"
	if noe: 
		print totalEnergy
		
		
	 

