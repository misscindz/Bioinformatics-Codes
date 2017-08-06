#!/usr/bin/python
# -*- coding: utf-8 -*-

# make test callable in subfolder
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import unittest

import numpy as np

from calcDistances import calculateDistances

class TestCalcDistances(unittest.TestCase):
    def test_calc_distances_0(self):
        vector1 = np.array([2.0,1.0,2.0])
        atomPositions = np.array([vector1,vector1])
        expectedResult = np.array([[0.0,0.0],[0.0,0.0]])
        
        result = calculateDistances(atomPositions)
        
        self.assertTrue( np.array_equal(result, expectedResult)) 
    
    def test_calc_distances_complex(self):
        vector1 = np.array([2.0,1.0,2.0])
        vector2 = np.array([4.0,1.0,2.0])
        vector3 = np.array([4.0,5.0,2.0])
        vector4 = np.array([4.0,1.0,-2.0])
        
        atomPositions = np.array([vector1,vector2,vector3,vector4])
        expectedResult = np.array(
                [[ 0.         , 2.         , 4.47213595 , 4.47213595],
                 [ 2.         , 0.         , 4.         , 4.        ],
                 [ 4.47213595 , 4.         , 0.         , 5.65685425],
                 [ 4.47213595 , 4.         , 5.65685425 , 0.        ]])
        result = calculateDistances(atomPositions)
        e = all([all(x) for x in map(np.isclose,result,expectedResult)])
        self.assertTrue(e)
                
if __name__ == '__main__':   
    unittest.main()
