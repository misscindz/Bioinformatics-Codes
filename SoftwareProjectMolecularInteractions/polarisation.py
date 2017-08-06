#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
    ./polarisation.py <path to argument file>

    Calculates polarisation energy.

    Examples:
        ./polarisation.py -h
        ./polarisation.py ./testMolecularInteraction/pot.inp

"""
import sys; sys.dont_write_bytecode = True
import numpy as np
import argparse
import sys
import collections
import types

from read_xyz import readXYZ, readXyzSimple
from testMolecularInteraction.readInpData import readInp
from change_unit import convertAtomicToAngstrom
from change_unit import EnergyAUToKcalPerMol


def _print_matrix(matrix):
    """
    Prints a matrix.

    Parameter
    ----------
    matrix: 2D-matrix containing floating-point numbers.
    """
    for i in range(len(matrix)):
        if i % 3 == 0 and i != 0:
            print
        for j in range(len(matrix[0])):
            if j % 3 == 0 and j != 0:
                print "  ",
            print "{: 5.5f}".format(matrix[i][j]),
        print
    print


def _print_vector(vector):
    """
    Prints a vector.

    Parameter
    ----------
    vector: vector containing floating-point number
    """
    for j in range(0, len(vector), 3):
        print "{5.8f}\t{5.8f}\t{5.8f}".format(
            vector[j], vector[j+1], vector[j+2])


def _tensofunction_0(vector1, vector2):
    """
    Evaluates 0th-order-tensor of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        Tensor 0th. order
    """
    norm = np.linalg.norm(vector1-vector2)
    if norm == 0.0:
        return 0.0
    return 1/(norm)


def _tensofunction_1x(vector1, vector2):
    """
    Evaluates first derivate of tensorfunction with respect to x
    of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        Tensor 1th. order
    """
    if np.array_equal(vector1, vector2):
        return 0.0
    assert pow(
        pow(vector1[0]-vector2[0], 2.0) +
        pow(vector1[1]-vector2[1], 2.0) +
        pow(vector1[2]-vector2[2], 2.0), 1.5) != 0, \
        "{} {}".format(vector1, vector2)
    r = -(vector1[0]-vector2[0])\
        / pow(
        pow(vector1[0]-vector2[0], 2.0) +
        pow(vector1[1]-vector2[1], 2.0) +
        pow(vector1[2]-vector2[2], 2.0), 1.5)
    return r


def _tensofunction_1y(vector1, vector2):
    """
    Evaluates first derivate of tensorfunction with respect to y
    of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        Tensor 1th. order
    """
    if np.array_equal(vector1, vector2):
        return 0.0
    r = -(vector1[1]-vector2[1]) / \
        pow(pow(vector1[0]-vector2[0], 2.0) +
            pow(vector1[1]-vector2[1], 2.0) +
            pow(vector1[2]-vector2[2], 2.0), 1.5)
    return r


def _tensofunction_1z(vector1, vector2):
    """
    Evaluates first derivate of tensorfunction with respect to z
    of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        Tensor 1th. order
    """
    if np.array_equal(vector1, vector2):
        return 0.0
    assert pow(
        pow(vector1[0]-vector2[0], 2.0) +
        pow(vector1[1]-vector2[1], 2.0) +
        pow(vector1[2]-vector2[2], 2.0), 1.5) != 0
    r = -(vector1[2]-vector2[2]) / \
        pow(pow(vector1[0]-vector2[0], 2.0) +
            pow(vector1[1]-vector2[1], 2.0) +
            pow(vector1[2]-vector2[2], 2.0), 1.5)
    return r


def _calculate_tensor2xx_numerator(vector1, vector2):
    """
    Evaluates the numerator of the second derivate of tensorfunction with
    respect to x and x of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result of the numerator of the second derivate of tensorfunction with
        respect to x and x of vector1 and vector2
    """
    if np.array_equal(vector1, vector2):
        return 0.0
    # 2*(x-x')^2 - (y-y')^2 - (z-z')^2)
    t = 2*(vector1[0]-vector2[0])**2.0 - \
        (vector1[1]-vector2[1])**2.0 - \
        (vector1[2]-vector2[2])**2.0
    return t


def _calculate_tensor2yy_numerator(vector1, vector2):
    """
    Evaluates the numerator of the second derivate of tensorfunction with
    respect to y and y of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result of the numerator of the second derivate of tensorfunction with
        respect to y and y of vector1 and vector2
    """
    if np.array_equal(vector1, vector2):
        return 0
    # -(x-x')^2 + 2*(y-y')^2 - (z-z')^2)
    r = -(vector1[0]-vector2[0])**2.0 \
        + 2*(vector1[1]-vector2[1])**2.0 \
        - (vector1[2]-vector2[2])**2.0
    return r


def _calculate_tensor2zz_numerator(vector1, vector2):
    """
    Evaluates the numerator of the second derivate of tensorfunction with
    respect to z and z of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result of the numerator of the second derivate of tensorfunction with
        respect to z and z of vector1 and vector2
    """
    if np.array_equal(vector1, vector2):
        return 0
    # -(x-x')^2 - (y-y')^2 +2*(z-z')^2)
    r = -(vector1[0]-vector2[0])**2.0 - \
        (vector1[1]-vector2[1])**2.0 + \
        2.0*(vector1[2]-vector2[2])**2.0
    return r


def _calculate_tensor2xy_numerator(vector1, vector2):
    """
    Evaluates the numerator of the second derivate of tensorfunction with
    respect to x and y of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result of the numerator of the second derivate of tensorfunction with
        respect to x and y of vector1 and vector2
    """
    if np.array_equal(vector1, vector2):
        return 0
    return 3.0*(vector1[0]-vector2[0])*(vector1[1]-vector2[1])


def _calculate_tensor2xz_numerator(vector1, vector2):
    """
    Evaluates the numerator of the second derivate of tensorfunction with
    respect to x and z of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result of the numerator of the second derivate of tensorfunction with
        respect to x and z of vector1 and vector2
    """
    if np.array_equal(vector1, vector2):
        return 0
    return 3.0*(vector1[0]-vector2[0])*(vector1[2]-vector2[2])


def _calculate_tensor2yz_numerator(vector1, vector2):
    """
    Evaluates the numerator of the second derivate of tensorfunction with
    respect to y and z of vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result of the numerator of the second derivate of tensorfunction with
        respect to y and z of vector1 and vector2
    """
    if np.array_equal(vector1, vector2):
        return 0
    return 3.0*(vector1[1]-vector2[1])*(vector1[2]-vector2[2])


def _calculate_tensor2_denominator(vector1, vector2):
    """
    Evaluates the denominator of the second derivates of tensorfunction
    for vector1 and vector2.

    Parameter
    ----------
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.

    Returns
    -------
    t: floating-point number
        result the denominator of the second derivates of tensorfunction
        for vector1 and vector2
    """
    # (x-x')^2
    x = pow(vector1[0]-vector2[0], 2)
    # (y-y')^2
    y = pow(vector1[1]-vector2[1], 2)
    # (z-z')^2
    z = pow(vector1[2]-vector2[2], 2)
    vector = x+y+z
    return pow(vector, 5.0/2.0)


def _calculate_tensor_second_order(t2, p1, p2, vector1, vector2):
    """
    Fills a 3x3 part of the matrix t2 at position (p1,p2) with values
    of the second order tensor for vector1 and vecto2.

    Parameter
    ----------
    t2:      np.matrix of type float
                matrix which represents second order interactions between
                atom positions.
    p1:      interger
                x-coordinate of position in matrix
    p2:      interger
                y-coordinate position in matrix
    vector1: np.vector of type float
                Atom position.
    vector2: np.vector of type float
                Atom position.
    """

    denominator_2 = _calculate_tensor2_denominator(vector1, vector2)

    t2[p1*3+0][p2*3+0] = _calculate_tensor2xx_numerator(vector1, vector2) /\
        denominator_2
    t2[p2*3+0][p1*3+0] = t2[p1*3+0][p2*3+0]

    t2[p1*3+0][p2*3+1] = _calculate_tensor2xy_numerator(vector1, vector2) /\
        denominator_2
    t2[p2*3+0][p1*3+1] = t2[p1*3+0][p2*3+1]

    t2[p1*3+0][p2*3+2] = _calculate_tensor2xz_numerator(vector1, vector2) /\
        denominator_2
    t2[p2*3+0][p1*3+2] = t2[p1*3+0][p2*3+2]

    t2[p1*3+1][p2*3+0] = t2[p1*3+0][p2*3+1]
    t2[p2*3+1][p1*3+0] = t2[p1*3+0][p2*3+1]

    t2[p1*3+1][p2*3+1] = _calculate_tensor2yy_numerator(vector1, vector2) /\
        denominator_2
    t2[p2*3+1][p1*3+1] = t2[p1*3+1][p2*3+1]

    t2[p1*3+1][p2*3+2] = _calculate_tensor2yz_numerator(vector1, vector2) /\
        denominator_2
    t2[p2*3+1][p1*3+2] = t2[p1*3+1][p2*3+2]

    t2[p1*3+2][p2*3+0] = t2[p1*3+0][p2*3+2]
    t2[p2*3+2][p1*3+0] = t2[p1*3+0][p2*3+2]

    t2[p1*3+2][p2*3+1] = t2[p1*3+1][p2*3+2]
    t2[p2*3+2][p1*3+1] = t2[p1*3+1][p2*3+2]

    t2[p1*3+2][p2*3+2] = _calculate_tensor2zz_numerator(vector1, vector2) /\
        denominator_2
    t2[p2*3+2][p1*3+2] = t2[p1*3+2][p2*3+2]


def _calc_f(atomPositions, pointCharge, ex):
    """
    F = - SUM_s'( T_0^(ss') * q^s' )

    Note: The order of atoms in the atomPositions has to be related
          to the order of point charges.

    Parameters
    ----------
    atomPositions: [[floating-point number]]
                3-dimensional vectors of coordinates
    pointCharge: [floating-point number]
                    corresponding point charges for each atom in atomPositions
    ex: [interger] Exclusion list

    Returns
    -------
    f: vector of length 3*number of atoms
    """
    assert pointCharge is not None, "pointCharge is None."
    assert isinstance(pointCharge, collections.Iterable), \
        "pointCharge is not a list. But {} {}]".format(
        pointCharge, type(pointCharge))
    assert atomPositions is not None, "atomPositions is None."
    assert all(isinstance(atom, collections.Iterable)
               for atom in atomPositions),\
        "atomPositions is not a 2D-Matrix.But {} {}]".format(
        atomPositions, type(atomPositions))
    assert len(atomPositions) == len(pointCharge), \
        "number of atom positions ({}) != " + \
        "number of point charges ({})".format(
        len(atomPositions), len(pointCharge))

    atomCnt = len(atomPositions)

    f_mul = np.zeros(shape=(atomCnt*3))

    for i in range(atomCnt):
        for j in range(atomCnt):
            if ex[i] != ex[j]:
                f_mul[i*3] += -1*_tensofunction_1x(
                    atomPositions[i], atomPositions[j])*pointCharge[j]
                f_mul[i*3 + 1] += -1*_tensofunction_1y(
                    atomPositions[i], atomPositions[j])*pointCharge[j]
                f_mul[i*3 + 2] += -1*_tensofunction_1z(
                    atomPositions[i], atomPositions[j])*pointCharge[j]

    return np.array(f_mul)


def _calc_alpha_inv(dispersionCoefficients):
    """
    Calulates the inverse of alpha.

    Parameters
    ----------
    dispersionCoefficients: Vektor (length: number of sites)
                            contains dispersion coefficients

    Returns
    -------
    alpha matrix: [floating-point number]
        contains square root of dispersion coefficients in diagonal
        otherwise 0
    """
    assert dispersionCoefficients is not None
    assert isinstance(dispersionCoefficients, collections.Iterable), \
        "dispersionCoefficients is not a list. But {}".format(
        dispersionCoefficients)

    siteCnt = len(dispersionCoefficients)

    alpha = np.zeros(shape=(3*siteCnt, 3*siteCnt), dtype="float64")

    for p1 in range(siteCnt):
        for innerpos in range(3):
            assert dispersionCoefficients[p1] > 0.0, \
                "{}".format(dispersionCoefficients[p1])

            alpha[3*p1+innerpos][3*p1+innerpos] = \
                1.0/np.sqrt(dispersionCoefficients[p1])

    return np.array(alpha)


def _calc_t2(positions, exclusionList):
    '''
    Calculates second order tensor

    Parameters
    ----------
    positions: List of Atompositions
               Matrix of shape ('number of atoms' x 3)

    exclusionList: contains molecule id of the current atom so that atoms
                   of the same molecule are not conciderd.

    Returns
    -------
    t2: matrix
            shape (n*3 x n*3)
    '''

    siteCnt = len(positions)

    assert siteCnt == len(exclusionList)
    t2 = np.zeros(shape=(3*siteCnt, 3*siteCnt), dtype="float64")

    for s1 in range(siteCnt):
        for s2 in range(s1+1, siteCnt):
            if exclusionList[s1] != exclusionList[s2]:
                _calculate_tensor_second_order(
                    t2,
                    s1, s2,
                    positions[s1], positions[s2])
    return np.array(t2)


def _invert(matrix):
    '''
    Inverts the given matrix.

    Parameters
    ----------
    matrix: which is inverted

    Returns
    -------
    matrix inverted
    '''
    return np.linalg.inv(matrix)


def _calc_mu_ind(alpha_inv, t2, F):
    '''
    mu_ind = (alpha^-1 - t2)^-1 * F

    Parameters
    ----------
    alpha: matrix
            shape (n*3 x n*3)
    t2: matrix
            shape (n*3 x n*3)
    F: vector of floating-point numbers

    Returns
    -------
    mu_ind: Vector of floating-point numbers, nunmberOfAtoms*3
    '''
    # Check Matrix dimensions
    assert alpha_inv.shape == t2.shape
    assert alpha_inv.shape[0] == F.shape[0], \
        "Sizes of matrix alpha and vector F dont't fit. \n{} != \n{}".format(
        alpha_inv.shape, F.shape, alpha_inv, F)
    assert len(alpha_inv.shape) == 2, "alpha is not a 2D-matrix."
    assert alpha_inv.shape[0] == alpha_inv.shape[1], "alpha is not a square."

    mu_ind = None

    s = alpha_inv - t2

    s_inv = _invert(s)

    mu_ind = np.dot(s_inv, F)
    # mu_ind = np.linalg.solve(s, F)

    assert mu_ind.shape[0] == len(alpha_inv), "mu_ind has wrong shape."\
        + " {}".format(mu_ind.shape)

    return mu_ind


def _calc_Epol(mu_ind, F):
    '''
    E_pol = -1/2 * (mu_ind)^T * F

    Parameters
    ----------
    mu_ind: [floating-point number] length: 3*Number_of_sites
    F: [floating-point number] length: 3*Number_of_sites

    Returns
    -------
    floating-point number: polarization energy
    '''

    e_pol = -0.5 * np.dot(np.transpose(mu_ind), F)

    assert isinstance(e_pol, types.FloatType), "No float type, but "\
        "{}.".format(type(e_pol))
    return e_pol


def calculatePolarisationEnergy(
        atomPositions,
        numberOfAtoms,
        pointCharge,
        dispersionCoefficient,
        exclusionLists=None):
    """
    This method _calculates the Energy which is gained by dipole-dipole
    interaction whithin a molecule.

    Energy = -1/2 * transpose(mu_ind) * F
    mu_ind and F are both vectors.

    mu_ind = (alpha^-1 - t2) * F
    alpha is a matrix which depends on dispersion coefficients.

    F depends on the pointcharges of the atoms and distences between these.
    Size 3*NoOfAtoms because of x,y,z directions.

    t2 is the second order tensor for the atom points.



    Parameter
    ---------
    atomPositions           : Matrix,
                                shape (number of atoms, 3),
                                type: floating-point number
                              list containing positions of atoms in the
                              corresponding molecule

    numberOfAtoms           : Number of atoms in file.


    pointCharge             : Vector,
                                shape (number of atoms),
                                type: floating-point number
                                point charges of atoms

    dispersionCoefficient   : Vector,
                                shape (number of atoms),
                                type: floating-point number
                              contains dispersion coefficients

    exclusionLists          : Molecule id

    Returns
    -------
    e   : floating-point number
            Energy value

    """
    e = 0
    if exclusionLists is None:
        exclusionLists = range(numberOfAtoms)

    assert isinstance(numberOfAtoms, types.IntType), \
        "numberOfAtoms is no Int-type, but {}.".format(type(numberOfAtoms))
    assert len(atomPositions) == numberOfAtoms, \
        "len(atomPositions) {} != numberOfAtoms {}".format(
            len(atomPositions), numberOfAtoms)
    assert dispersionCoefficient is not None, "dispersionCoefficient is None."
    assert len(dispersionCoefficient) == numberOfAtoms, \
        "len(dispersionCoefficient) {} != numberOfAtoms {}".format(
            (len(dispersionCoefficient), numberOfAtoms))
    assert pointCharge is not None, "pointCharge is None."
    assert len(pointCharge) == numberOfAtoms, \
        "len(pointCharge) {} != numberOfAtoms {}".format(
            len(pointCharge), numberOfAtoms)

    F = _calc_f(atomPositions, pointCharge, exclusionLists)

    alpha_inv = _calc_alpha_inv(dispersionCoefficient)

    t2 = _calc_t2(atomPositions, exclusionLists)

    # mu_ind = (alpha^-1 - t2) * F
    mu_ind = _calc_mu_ind(alpha_inv, t2, F)

    # Energy = -1/2 * (mu_ind)^T * F
    e = _calc_Epol(mu_ind, F)

    # Umwandlung in kcal/mol
    e = EnergyAUToKcalPerMol(e)

    assert isinstance(e, types.FloatType), "No float type, but {}.".format(
        type(e))
    return e


if __name__ == "__main__":
    """
    Testaufruf dieses Programms:
    ./polarisation.py -h
    ./polarisation.py ./testMolecularInteraction/pot.inp
    """

    inputFileName = "./data/02Waterdimer_c.xyz"
    atomPositions = np.array([[1.0, 1.0, 1.0], [1.0, 1.0, 0.0]])
    n = len(atomPositions)
    exclusionList = range(n)
    pointCharge = np.ones(n, dtype="float64")
    dispersionCoefficient = np.ones(n, dtype="float64")

    # Kommandozeilenargumente einlesen
    parser = argparse.ArgumentParser(description="Calculates Energy" +
                                     " term for molecular interactions.")
    parser.add_argument('mf',
                        type=str,
                        help="Paths to the xyz-molecule-file for which the" +
                        " Energy shall be calculated.")
    parser.add_argument('-xyz',
                        '--xyz', action='store_true',
                        help="If option -xyz is given, the read input file " +
                        "is treated as xyz-file. Default is .inp")

    args = parser.parse_args()

    inputFileName = args.mf
    isXYZ = args.xyz

    # Read file
    fin = open(inputFileName, "r")
    if isXYZ:
        atomPositions, atomTypes, n, comment = readXyzSimple(fin)
        n = len(atomPositions)
        exclusionList = range(n)
        pointCharge = np.ones(n, dtype="float64")
        dispersionCoefficient = np.ones(n, dtype="float64")
    else:
        exclusionList, atomPositions, pointCharge, n, \
            dispersionCoefficient = readInp(fin)
        # From dispersion coefficients in data file root is already
        # taken. Create initial status.
        dispersionCoefficient = map(lambda x: x**2, dispersionCoefficient)

    fin.close()

    print calculatePolarisationEnergy(
        map(convertAtomicToAngstrom, atomPositions),
        n,
        pointCharge,
        dispersionCoefficient,
        exclusionList)
