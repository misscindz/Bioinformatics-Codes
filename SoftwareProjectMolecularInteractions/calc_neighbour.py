#!/usr/bin/python2.7
# -*- coding: utf-8 -*-

import numpy as np
import ctypes
import re, sys
import math
from copy import deepcopy

sys.dont_write_bytecode = True

def getNeighborMatrix(natom, atomtypes, distance, covalenceRadii, elements):

    """
    given natom (number of atoms), atomtypes (a vector of atom types),
    distance (a matrix of distances), rcov (a vector of covalent radius),
    and elememts (a vector of elements)
    return the coordination number for each atom, which saved in a vector
    """
    rcov = deepcopy(covalenceRadii)
    len_distanceMatrix = len(distance)
    neighbour_matrix = np.zeros(shape=(len_distanceMatrix,len_distanceMatrix))
  
    k2 = 4.0/3.0
    k1 = 16
    autoang = 0.52917726
    max_elem = 94
    ri = 0
    rj = 0
    for i in range(len(rcov)):
        rcov[i] = float(rcov[i])/k2
    for i in range(0,natom):
        for j in range(0,natom):
            if j != i:
                r = distance[i][j]
                for k in range(0, max_elem):
                    if atomtypes[i].find(elements[k])>-1:
                        ri=k
                    if atomtypes[j].find(elements[k])>-1:
                        rj=k
            
                rco = rcov[ri] + rcov[rj]
                rco = rco * k2
                rr = rco/r
                damp = 1.0/(1.0 + math.exp(-k1*(rr-1.0)))
        
                if (damp > 0.8):
                  neighbour_matrix[i][j] = 1.0
                if (damp < 0.8): 
                  neighbour_matrix[i][j] = 0.0
    return neighbour_matrix


#if __name__ == "__main__":
  
  ## Neighbour_matrix contains the neighbours of every atom. 1 represent "neighbours". 0 represent "not neighbours"
  ## function call: ./neighbours.py elements.txt rcov.dat input.xyz

  #f_elem = sys.argv[1]			        #chemical elements file name
  #f_rcov = sys.argv[2]        			#covalent radius file name
  #f_coor = sys.argv[3]        			#coordination file name
  #elements = read_elem(f_elem)
  #rcov = readcov(f_rcov)
  #coor, atomtypes, natom = read_coors(f_coor)
  #distance = calculateDistances(coor)
  #neighbour_matrix = calc_neighbour(natom, atomtypes, distance, rcov, elements)
  #print (neighbour_matrix)
      

