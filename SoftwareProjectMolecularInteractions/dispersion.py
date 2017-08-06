#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
    Calaculates Dispersion Energy (DFT-D3(CSO))
    Usage:
        python dispersion.py <coordination_datei> <covalentRadii_datei> -h
        python dispersion.py <coordination_datei> <covalentRadii_datei>
    Parameters
    ----------
    cn:     Coordination of molecule
    rcov:   Covalent radii
    rc6:    Reference dispersion coefficient
    elements:   chemical elements (1-94)
    r2r4:   r2r4
    
    Returns
    -------
    Dispersion energy (float), Dispersion coefficients C6AA (vector)
    
"""

import re, sys; import sys; sys.dont_write_bytecode = True
import numpy as np
import math
import argparse
from copy import deepcopy
from parameters import *

#coordinate_datei parsen
def read_coors(fin):

    """
    given the file name
    return a matrix of coordinates, a vector of atom types
    and an integer of the number of atoms in the molecule
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
    for i in range(0,natom):
        for j in range(0, 3):
            coords[i][j] = coords[i][j] / 0.5291772083

    fin.close()
    return (coords,atomtypes,natom)


#rcov_datei parsen
def readCov(fin):
   
    """
    Given a file name
    Return a vector of covalent radii
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
    rcov_file.close()
    return rcov


#coordinate_datei parsen
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
    
    """
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
            vector1 = vectors[i]
            vector2 = vectors[j]
            distance = np.linalg.norm(vector1-vector2)
            matrix[i][j] = distance
    return matrix

#calculate coordination number
def getCoordinationNumber(natom, atomtypes, distance, covalenceRadii, elements):
    
    """
    given natom (number of atoms), atomtypes (a vector of atom types),
    distance (a matrix of distances), rcov (a vector of covalent radius),
    and elememts (a vector of elements)
    return the coordination number for each atom, which is saved in a vector
    """
    rcov = deepcopy(covalenceRadii)
    k2 = 4.0/3.0
    k1 = 16
    autoang = 0.52917726
    max_elem = 94
    ri = 0
    rj = 0
    cn = []
    for i in range(len(rcov)):
        rcov[i] = float(rcov[i])/k2
    for i in range(0,natom):
        cn_a = 0
        for j in range(0, natom):
            if j != i:
                r = distance[i][j]
                for k in range(0, max_elem):
                    if atomtypes[i].find(elements[k])>-1:
                        ri=k
                    if atomtypes[j].find(elements[k])>-1:
                        rj=k
            
                rco = rcov[ri] + rcov[rj]
                rco = rco * k2
                rr = rco / r
                damp = 1.0/(1.0 + math.exp(-k1*(rr-1.0)))
                cn_a = cn_a + damp
        cn.append(cn_a)
    return cn

#copy reference c6_coefficient in matrix c6ab
def copy_rc6(rc6):
    rc6ab = [[0] * 94 for x in xrange(94)]
    nlines = 32385
    
    for iat in range(0, 94):
        for jat in range(0, 94):
            rc6ab[iat][jat] = [[0] * 5 for x in xrange(5)]

    for n in range(0, nlines):
        icon = 0
        jcon = 0
        iat = int(rc6[n][1])-1
        jat = int(rc6[n][2])-1
        
        while iat > 99:
            icon = icon + 1
            iat = iat - 100
        while jat > 99:
            jcon = jcon + 1
            jat = jat - 100

        rc6ab[iat][jat][icon][jcon] = []
        rc6ab[iat][jat][icon][jcon].append(rc6[n][0])
        rc6ab[iat][jat][icon][jcon].append(rc6[n][3])
        rc6ab[iat][jat][icon][jcon].append(rc6[n][4])

        rc6ab[jat][iat][jcon][icon] = []
        rc6ab[jat][iat][jcon][icon].append(rc6[n][0])
        rc6ab[jat][iat][jcon][icon].append(rc6[n][4])
        rc6ab[jat][iat][jcon][icon].append(rc6[n][3])
    return rc6ab

#calculate c6 value for atom pair
def getdispersioncoefficient(rc6ab, atomtypes,cn,natom,elements):
    
    """
    given c6ab (vector of reference c6 value), atomtypes (vector of atom types),
    cn (vector of coordination numbers),natom (number of atoms), elements (vector of elements)
    atomi (index of atom i) and atomj (index of atom j)
    return a vector of c6aa and a vector of c6ab
    """
    k3 = -4.0
    mxc = []
    w = 0.0
    z = 0.0
    c6 = 0.0
    c6ab = []
    c6aa = []
    for j in range (0,94):
        mxc.append(0)
        for k in range(0, natom):
            if atomtypes[k].find(elements[j]) > -1:
                for l in range(0, 5):
                    if isinstance(rc6ab[j][j][l][l], (list,tuple)):
                        if rc6ab[j][j][l][l][0] > 0:
                            mxc[j] = mxc[j] + 1
                break

    for atomi in range (0, natom):
       for atomj in range (atomi, natom):
           c6mem = -1.0E99
           w = 0.0
           z = 0.0
           c6 = 0.0
            
           for i in range(0,94):
               if atomtypes[atomi].find(elements[i])> -1: iat = i
               if atomtypes[atomj].find(elements[i])> -1: jat = i

           for m in range(0,mxc[iat]):
               for n in range(0,mxc[jat]):
                   if isinstance(rc6ab[iat][jat][m][n], (list, tuple)):
                       c6 = rc6ab[iat][jat][m][n][0]
                       if c6 > 0:
                            c6mem = c6
                            cn1 = rc6ab[iat][jat][m][n][1]
                            cn2 = rc6ab[iat][jat][m][n][2]
                            r = (cn1 - cn[atomi]) ** 2 + (cn2 - cn[atomj]) ** 2
                            tmp1 = math.exp(k3 * r)
                            w = w  + tmp1
                            z = z +  tmp1 * c6
           if (w > 0):
                c6 = z/w
           else:
                c6 = c6mem
           if atomi == atomj:
                c6aa.append(c6)
           if atomi != atomj :
                c6ab.append(c6)

    return c6ab,c6aa


#calculate dispersion energy
def calculateDispersionEnergy(distance, natom, rcov,elements,atomtypes,cn):
    """
    given distance(a matrix of distance for atom pair), natom (the number of atoms),
    rcov (a vector of covalent radii), elements (a vector of elements (atoms), atomtypes (a vector of atom types), cn (a vector of coordination number))
    return the calculated dispersion energy in kcal/mol.
    """
    s6 = 1.0
    a1 = 1.0
    a4 = 2.5**2
    a2 = 2.5
    a3 = 0
    dis_energy = 0
    tmp = 0
    l = 0
    c6aa = []
    c6ab = []
    rc6 = getRc6()
    rc6ab = copy_rc6(rc6)
    r2r4 = getr2r4()
    
    c6ab,c6aa = getdispersioncoefficient(rc6ab, atomtypes, cn, natom, elements)
    
    for i in range(natom):
        for j in range(i,natom):
            if i != j:
                for k in range(0,94):
                    if atomtypes[i].find(elements[k])>-1: Zi = k
                    if atomtypes[j].find(elements[k])>-1: Zj = k
                r0ab = math.pow((3 * float(r2r4[Zi]) * float(r2r4[Zj])),0.5)
                tmp = s6 + (a1/(1+math.exp(distance[i][j] - a2 * r0ab)))
                tmp1 = tmp * (c6ab[l]/(math.pow(distance[i][j],6) + math.pow(a4,6)))
                dis_energy = dis_energy + tmp1
                l = l + 1
    dis_energy *= 627.51
    return -dis_energy,c6aa

if __name__ == "__main__":

    parser = argparse.ArgumentParser(
                                     prog="./dispersion.py",
                                     usage="%(prog)s <option>",
                                     description="Calculates Dispersion"
                                     +"Energy.")
    parser.add_argument("moleculeFile",type=str, nargs='?',
                        default = "data/testDataXYZ/01_Water-Water_1.00.xyz",
                        help= "Paths to the xyz-molecule-file for which the"
                        + " Energy shall be calculated.\nThe file must be in the fol"
                        + "lowing format:\n "
                        + "\n<AU|AA>\n<number of atoms>\n<Comment line>\n"
                        + "Molecule Coordination")
    parser.add_argument("covalentRadiiFile",type=str, nargs='?',
                        default = "data/sw_project_data/rcov.dat",
                        help= "Paths to the covalent_radii_file\n"
                        + "The file must be in the fol"
                        + "\nlowing format:\n "
                        + "\n<Comment line>\n"
                        + "\n<covalent radii> each line starts with '.'")
    parser.add_argument("-sdc6","--showDispersionCoefficientc6aa",
                                    action="store_true",
                                    default = False,
                                    help="Disables printing of Dispersion coefficient c6aa.")
    args = parser.parse_args()
    if args.moleculeFile is not None:
        coor,atomtypes, natom = read_coors(args.moleculeFile)
    if args.covalentRadiiFile is not None:
        rcov = readCov(args.covalentRadiiFile)

    sdc6 = args.showDispersionCoefficientc6aa
    distance = calculateDistances(coor)
    elements = getElements()
    rc6 = getRc6()
    rc6ab = copy_rc6(rc6)
    cn = getCoordinationNumber(natom, atomtypes, distance, rcov, elements)
    c6ab, c6aa = getdispersioncoefficient(rc6ab, atomtypes, cn,natom,elements)
    disp_energy,c6aa = calculateDispersionEnergy(distance, natom, rcov, elements,atomtypes,cn)
    print "**** Edisp: {:.1f} kcal/mol ****".format(disp_energy)
    if sdc6:
        print "**** C6AA ********************"
        for i in range(len(c6aa)):
            print "c6_" + atomtypes[i] + atomtypes[i] + ": " + "{:.3f}".format(c6aa[i])





