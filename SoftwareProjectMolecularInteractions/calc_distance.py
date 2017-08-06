#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import sys

sys.dont_write_bytecode = True

def calculateDistances(vectors):
    """ read a xyz file from file handle
    Parameters
    ----------
    vectors : List of vectors
        List containing of which the pairwise distanze is 
        calculated

    Returns 
    -------
    matrix with pairwise distances in angrtom
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
 
if __name__ == "__main__":
     vectors = np.array([[1.0, 1.0, 0.0],\
                         [2.2, -1.3, -0.2],\
                         [1.0,2.0,-1.0]])

     print calculateDistances(vectors)
