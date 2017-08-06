#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import re
import sys
import os
from subprocess import Popen, PIPE
import random

import unittest

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from polarisation import _calc_Epol
from polarisation import _calc_mu_ind
from polarisation import _invert
from polarisation import _calc_t2
from polarisation import _calc_alpha_inv
from polarisation import _calculate_tensor2_denominator
from polarisation import _calculate_tensor2xy_numerator
from polarisation import _calculate_tensor2zz_numerator
from polarisation import _calculate_tensor2yy_numerator
from polarisation import _calculate_tensor2xx_numerator
from polarisation import _tensofunction_1z
from polarisation import _tensofunction_1y
from polarisation import _tensofunction_1x
from polarisation import _tensofunction_0
from polarisation import _calc_f
from polarisation import calculatePolarisationEnergy

from change_unit import convertAtomicToAngstrom, EnergyAUToKcalPerMol
from read_xyz import readXYZ, readXyzSimple, readXYZSplit
from electrostatics import calculatePartialCharge
from pse import getElements
from calc_distance import calculateDistances
from read_rcov import readCov
from dispersion import calculateDispersionEnergy
from dispersion import getCoordinationNumber




def _write_inp_file(filename, atomPoints, polarisierung, dispersionCoeff, ex):
    na = len(atomPoints)
    assert len(polarisierung) == na,\
        "length of polarisierung ({}) is no equeal to length of atompoints" + \
        " ({})".format(len(polarisierung), na)
    assert len(dispersionCoeff) == na
    assert len(ex) == na

    commentLine = "$point_charges\n"
    unitLine = "AU" + "\n"
    noMMsites = str(na) + " "
    orderK = "0"+" "
    orderPol = "1"+" "
    lengthExcludeList = "1"+"\n"

    genralInfo = " ".join([noMMsites, orderK, orderPol, lengthExcludeList])

    with open(filename, "w") as text_file:
        text_file.write(commentLine)
        text_file.write(unitLine)
        text_file.write(genralInfo)

        for i in range(na):
            x = atomPoints[i][0]
            y = atomPoints[i][1]
            z = atomPoints[i][2]
            e = polarisierung[i]
            d = dispersionCoeff[i]
            molecule = ex[i]

            atomLine = " ".join(["{:<2}".format(molecule),
                                 '{:12.7f}'.format(x),
                                 '{:12.7f}'.format(y), '{:12.7f}'.format(z),
                                 '{:4.1f}'.format(e), '{:4.1f}'.format(d)])

            text_file.write(atomLine+"\n")
        text_file.write("$end\n")


def _call_refernz_programm(
        atomPoints,
        polarisierung,
        dispersionCoeff,
        ex,
        filename="./testMolecularInteraction/pot2.inp"):
    p = "./testMolecularInteraction/projekt.x"

    _write_inp_file(filename, atomPoints, polarisierung, dispersionCoeff, ex)

    process = Popen([p, filename], stdout=PIPE)

    (output, err) = process.communicate()
    exit_code = process.wait()

    #print output.split('\n')[-2].split()[-1]

    os.remove(filename)
    return (output, err, exit_code)


class TestPolarization(unittest.TestCase):
    def test_tensofunction_1z0(self):
        expectedResult = 0.0
        vector1 = np.array([2.0, 1.0, 2.0])
        result = _tensofunction_1z(vector1, vector1)
        self.assertTrue(result == expectedResult)

    def test_tensofunction_1z(self):
        vector1 = np.array([2.0, 1.0, 2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _tensofunction_1z(vector1, vector2)
        expectedResult = -2.0/27.0
        self.assertTrue(result == expectedResult)

    def test_tensofunction_1y0(self):
        expectedResult = 0.0
        vector1 = np.array([2.0, 1.0, 2.0])
        result = _tensofunction_1y(vector1, vector1)
        self.assertTrue(result == expectedResult)

    def test_tensofunction_1y(self):
        vector1 = np.array([2.0, 1.0, 2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _tensofunction_1y(vector1, vector2)
        expectedResult = -2.0/27.0
        self.assertTrue(result == expectedResult)

    def test_tensofunction_1x0(self):
        expectedResult = 0.0
        vector1 = np.array([2.0, 1.0, 2.0])
        result = _tensofunction_1x(vector1, vector1)
        self.assertTrue(result == expectedResult)

    def test_tensofunction_1x(self):
        vector1 = np.array([2.0,  1.0,  2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _tensofunction_1x(vector1, vector2)
        expectedResult = -1.0/27.0
        self.assertTrue(result == expectedResult)

    def test_calculate_tensor2xx_numerator(self):
        vector1 = np.array([2.0,  1.0,  2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _calculate_tensor2xx_numerator(vector1, vector2)
        expectedResult = -6.0
        self.assertTrue(result == expectedResult)

    def test_calculate_tensor2yy_numerator(self):
        vector1 = np.array([2.0,  1.0,  2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _calculate_tensor2yy_numerator(vector1, vector2)
        expectedResult = 3.0
        self.assertTrue(result == expectedResult)

    def test_calculate_tensor2zz_numerator(self):
        vector1 = np.array([2.0,  1.0, 2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _calculate_tensor2zz_numerator(vector1, vector2)
        expectedResult = 3.0
        self.assertTrue(result == expectedResult)

    def test_calculate_tensor2xy_numerator(self):
        vector1 = np.array([2.0,  1.0, 2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _calculate_tensor2xy_numerator(vector1, vector2)
        expectedResult = 6.0
        self.assertTrue(result == expectedResult)

    def test_calculate_tensor2_denominator(self):
        vector1 = np.array([2.0,  1.0, 2.0])
        vector2 = np.array([1.0, -1.0, 0.0])
        result = _calculate_tensor2_denominator(vector1, vector2)
        expectedResult = 243.0
        self.assertTrue(result == expectedResult)

    def test_calc_f(self):
        atomPositions = np.array([[0, 0, 0], [1, 0, 0]])
        pointCharge = np.array([1, 2])
        ex = np.array(range(2))
        self.assertTrue(np.array_equal(_calc_f(atomPositions, pointCharge, ex),\
                                        [-2.0, 0.0, 0.0, 1.0, 0.0, 0.0]))

    def test_tensofunction_0(self):
        vector1 = np.array([1, 2, 3])
        vector2 = np.array([-1, 0, 2])
        result = _tensofunction_0(vector1, vector2)
        expectedResult = 1.0/3.0
        self.assertTrue(result == expectedResult)

    def test_calc_alpha_inv(self):
        dispersionCoefficients = [1.0, 4.0]
        result = _calc_alpha_inv(dispersionCoefficients)
        expectedResult = np.array([[1.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                                   [0.0, 1.0, 0.0, 0.0, 0.0, 0.0],
                                   [0.0, 0.0, 1.0, 0.0, 0.0, 0.0],
                                   [0.0, 0.0, 0.0, 0.5, 0.0, 0.0],
                                   [0.0, 0.0, 0.0, 0.0, 0.5, 0.0],
                                   [0.0, 0.0, 0.0, 0.0, 0.0, 0.5]])
        self.assertTrue(np.array_equal(result, expectedResult))

    def test_calc_t2_1(self):
        v1 = np.array([0.0, 0.0, 0.0])
        v2 = np.array([1.0, 0.0, 0.0])
        positions = [v1, v2]
        exclusionList = [1, 2]
        result = _calc_t2(positions, exclusionList)
        expectedResult = np.array([[0.0, 0.0, 0.0, 2.0, 0.0, 0.0],
                                   [0.0, 0.0, 0.0, 0.0, -1.0, 0.0],
                                   [0.0, 0.0, 0.0, 0.0, 0.0, -1.0],
                                   [2.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                                   [0.0, -1.0, 0.0, 0.0, 0.0, 0.0],
                                   [0.0, 0.0, -1.0, 0.0, 0.0, 0.0]])
        self.assertTrue(np.array_equal(result, expectedResult))

    def test_calc_t2_2(self):
        positions = np.array([[1.2527771,  0.7921532, -0.7711092],
                              [0.7442494,  1.1155204,  0.0],
                              [1.2527771,  0.7921532,  0.7711092],
                              [-0.4977129, -0.3559043,  0.0],
                              [-0.8958592, -1.2571864,  0.0],
                              [-1.8562314, -1.0867361,  0.0]])
        exclusionList = [1, 1, 1, 2, 2, 2]
        expectedResult = np.array(
               [[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.07629389, 0.10910781, -0.07328381, 0.01633724, 0.04861878, -0.01829389, 0.02153145, 0.02481265, -0.01018328],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.10910781, -0.01850895, -0.04806313, 0.04861878, 0.01173465, -0.01744846, 0.02481265, -0.00453097, -0.00615414],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.07328381, -0.04806313, -0.05778494, -0.01829389, -0.01744846, -0.0280719 , -0.01018328, -0.00615414, -0.01700048],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.03475332, 0.20713188, 0.0, -0.00125132, 0.05847491, 0.0, 0.01887838, 0.03738821, 0.0],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.20713188, 0.10532391, 0.0, 0.05847491, 0.0429228 , 0.0, 0.03738821, 0.0063922 , 0.0],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.14007723, 0.0, 0.0, -0.04167149, 0.0, 0.0, -0.02527057],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.07629389, 0.10910781, 0.07328381, 0.01633724, 0.04861878, 0.01829389, 0.02153145, 0.02481265, 0.01018328],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.10910781, -0.01850895, 0.04806313, 0.04861878, 0.01173465, 0.01744846, 0.02481265, -0.00453097, 0.00615414],
                [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.07328381, 0.04806313, -0.05778494, 0.01829389, 0.01744846, -0.0280719 , 0.01018328, 0.00615414, -0.01700048],
                [0.07629389, 0.10910781, -0.07328381, 0.03475332, 0.20713188, 0.0, 0.07629389, 0.10910781, 0.07328381, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [0.10910781, -0.01850895, -0.04806313, 0.20713188, 0.10532391, 0.0, 0.10910781, -0.01850895, 0.04806313, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [-0.07328381, -0.04806313, -0.05778494, 0.0, 0.0, -0.14007723, 0.07328381, 0.04806313, -0.05778494, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [0.01633724, 0.04861878, -0.01829389, -0.00125132, 0.05847491, 0.0, 0.01633724, 0.04861878, 0.01829389, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [0.04861878, 0.01173465, -0.01744846, 0.05847491, 0.0429228 , 0.0, 0.04861878, 0.01173465, 0.01744846, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [-0.01829389, -0.01744846, -0.0280719 , 0.0, 0.0, -0.04167149, 0.01829389, 0.01744846, -0.0280719 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [0.02153145, 0.02481265, -0.01018328, 0.01887838, 0.03738821, 0.0, 0.02153145, 0.02481265, 0.01018328, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [0.02481265, -0.00453097, -0.00615414, 0.03738821, 0.0063922 , 0.0, 0.02481265, -0.00453097, 0.00615414, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
                [-0.01018328, -0.00615414, -0.01700048, 0.0, 0.0, -0.02527057, 0.01018328, 0.00615414, -0.01700048, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]])
        result = _calc_t2(positions, exclusionList)

        self.assertTrue(np.allclose(result, expectedResult))

    def test_invert(self):
        matrix = np.array([[3.0, 2.0, 6.0],
                           [1.0, 1.0, 3.0],
                           [-3.0, -2.0, -5.0]])

        result = _invert(matrix)
        expectedResult = np.array([[1.0, -2.0, 0.0],
                                   [-4.0, 3.0, -3.0],
                                   [1.0, 0.0, 1.0]])

        # e = all([all(x) for x in map(np.isclose,result,expectedResult)])
        self.assertTrue(np.allclose(result, expectedResult))

    @unittest.expectedFailure
    def test_invert_not_possible(self):
        # TODO how to react in this case?
        matrix = np.array([[3.0, 2.0, 6.0],
                           [1.0, 1.0, 3.0],
                           [1.0, 1.0, 3.0]])
        result = _invert(matrix)

    def test_calculate_polarisation_Energy_InputFiles(self):
        pathToFiles = "./data/"
        files = ["testDataEmmi/01_Water-Water.emmi",
                 "testDataEmmi/09_MeNH2-MeOH.emmi",
                 "testDataEmmi/23_AcNH2-Uracil.emmi"]
        dataPath = "sw_project_data/"
        atomPositions = []
        atomTypes = []
        n = 0
        elements = getElements()
        dispersionCoefficient = []
        pointCharge = []
        ex = None

        for f in files:
            with open(pathToFiles + f, "r") as fin:
                # atomPositions, atomTypes, n, comment = readXYZSimple(fin)
                (atomPositionsDict,
                atomTypesDict,
                ndict,
                moleculeIdDict,
                comment) = readXYZSplit(fin)

                atomPositions = atomPositionsDict[0]
                atomTypes = atomTypesDict[0]
                ex = moleculeIdDict[0]
                n = ndict[0]

            ex = range(n)

            distanceMatrix = calculateDistances(np.array(atomPositions))
            pointCharge = calculatePartialCharge(
                n,
                atomTypes,
                elements,
                distanceMatrix)

            covalenceRadii = readCov(pathToFiles+dataPath + "rcov.dat")

            coordinationNumber = getCoordinationNumber(
                n,
                atomTypes,
                distanceMatrix,
                covalenceRadii,
                elements)

            _,dispersionCoefficient = calculateDispersionEnergy(
                distanceMatrix,
                n,
                covalenceRadii,
                elements,
                atomTypes,
                coordinationNumber)

            dispersionCoefficientR =  np.sqrt(dispersionCoefficient)
            output, err, exit_code = _call_refernz_programm(
                atomPositions,
                pointCharge,
                dispersionCoefficientR,
                ex)

            r = float(output.split('\n')[-2].split()[-1])
            p = calculatePolarisationEnergy(
                    atomPositions,
                    n,
                    pointCharge,
                    dispersionCoefficient,
                    ex)


    def test_calculate_polarisation_Energy(self):
        #Parameter besimmen, Dateischreiben, Programmaufruf
        #Wert auslesen
        #vergleichen
        atomPositions = np.array([
            [1.2527771,0.7921532,-0.7711092]\
            ,[0.7442494,1.1155204,0.0000000]\
            ,[1.2527771,0.7921532,0.7711092]\
            ,[-0.4977129,-0.3559043,0.0000000]\
            ,[-0.8958592,-1.2571864,0.0000000]\
            ,[-1.8562314,-1.0867361,0.0000000]])
        pointCharge = np.array([-0.6,0.3,0.3,-0.6,0.3,0.3])
        dispersionCoefficient = np.array([5.4,2.5,2.5,5.4,2.5,2.5])
        n = len(pointCharge)
        ex = np.array([1,1,1,2,2,2])

        #up = 3.0
        #low = -3.0
        #atomPositions = np.array([
        #    [random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
        #    ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
        #    ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
        #    ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
        #    ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
        #    ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]])

        output, err, exit_code = _call_refernz_programm(atomPositions, 
                                                        pointCharge, 
                                                        dispersionCoefficient,
                                                        ex)
        dispersionCoefficient = map(lambda x:pow(x, 2.0), dispersionCoefficient)
        r = float(output.split('\n')[-2].split()[-1])
        p = calculatePolarisationEnergy(
                atomPositions,
                n,
                pointCharge,
                dispersionCoefficient,
                ex)

        difference = abs(EnergyAUToKcalPerMol(r)-p)

        self.assertTrue(difference < 0.001)


if __name__ == '__main__':
    unittest.main()

    '''
    atomPoints = np.array([
        [1.2527771,0.7921532,-0.7711092]\
        ,[0.7442494,1.1155204,0.0000000]\
        ,[1.2527771,0.7921532,0.7711092]\
        ,[-0.4977129,-0.3559043,0.0000000]\
        ,[-0.8958592,-1.2571864,0.0000000]\
        ,[-1.8562314,-1.0867361,0.0000000]])
    polarisierung = np.array([-0.6,0.3,0.3,-0.6,0.3,0.3])
    dispersionCoeff = np.array([5.4,2.5,2.5,5.4,2.5,2.5])
    ex = np.array([1,1,1,2,2,2])

    maxreldif = 10.0**10
    maxdiff = 0.0

    for i in range(10):
        up = 3.0
        low = -3.0
        atomPoints = np.array([
            [random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
            ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
            ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
            ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
            ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]\
            ,[random.uniform(up, low),random.uniform(up, low),random.uniform(up, low)]])

        output, err, exit_code = _call_refernz_programm(atomPoints,polarisierung,dispersionCoeff,ex)
        dispersionCoeff = map(lambda x:pow(x,2.0),dispersionCoeff)
        r = float(output.split('\n')[-2].split()[-1])
        p = calculatePolarisationEnergy(
            atomPoints*1.889725989
            ,len(atomPoints)
            ,polarisierung
            ,dispersionCoeff
            ,ex)

        differenz = r-p
        maxreldif = min(abs(p+r)/2.0/abs(differenz),maxreldif)
        maxdiff = max(maxdiff,abs(differenz))
        if abs(differenz) > 0.0001:
            print "p", p
            print "r", r
            print "differenz", differenz
            print (p+r)/2.0/differenz
    print maxreldif,maxdiff

    '''
