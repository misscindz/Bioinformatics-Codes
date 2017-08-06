#!/usr/bin/python2.7
# -*- coding: utf-8 -*-


import numpy as np
import ctypes
import re
import sys
import math

from dispersion import readCov
from dispersion import read_coors
from dispersion import read_elem
from dispersion import read_coors
from dispersion import getCoordinationNumber
from calc_distance import calculateDistances
from calc_neighbour import getNeighborMatrix

sys.dont_write_bytecode = True

def calcDistOfPair(distance, atom_1, atom_2, atom_3):
    dist_ab = distance[atom_1][atom_2]
    dist_ha = distance[atom_1][atom_3]
    dist_hb = distance[atom_2][atom_3]

    if (dist_ha < dist_hb):
        dist_xh = dist_ha
    else:
        dist_xh = dist_hb

    return (dist_ab, dist_xh)


def getNextNeighbour(atom, natom, neighbour_matrix, distance):

    dist_r1 = 255
    dist_r2 = 255
    dist_r3 = 255
    r_1 = -1
    r_2 = -1
    r_3 = -1

    for i in range(0, natom):
        if (neighbour_matrix[atom][i] == 1):
            if (distance[atom][i] < dist_r1):
                dist_r3 = dist_r2
                r_3 = r_2
                dist_r2 = dist_r1
                r_2 = r_1
                dist_r1 = distance[atom][i]
                r_1 = i
            elif (distance[atom][i] < dist_r2):
                dist_r3 = dist_r2
                r_3 = r_2
                dist_r2 = distance[atom][i]
                r_2 = i
            elif (distance[atom][i] < dist_r3):
                dist_r3 = distance[atom][i]
                r_3 = i
            else:
                continue
    return r_1, r_2, r_3


def calcOptimalAngle(atom_x, r_1, r_2, r_3, atom_3, atomtypes, con_value):
    phi_optimal = 0.0
    psi_optimal = 0.0
    if (atomtypes[atom_x] == 'N'):
        if (con_value[atom_x] >= 1.0 and con_value[atom_x] <= 1.5):
            phi_optimal = math.pi/(180.0/120.0)
            psi_optimal = 0.0
        else:
            phi_optimal = math.pi/(180.0/109.48)
            psi_optimal = math.pi/(180.0/54.74)
    if (atomtypes[atom_x] == 'O'):
        if (con_value[atom_x] >= 2 and con_value[atom_x] <= 2.5):
            phi_optimal = math.pi/(180.0/120.0)
            psi_optimal = 0.0
        else:
            phi_optimal = math.pi/(180.0/120.0)
            psi_optimal = math.pi/(180.0/90.0)
    return phi_optimal, psi_optimal


def calcAngle(atom_1, atom_2, atom_3, coor):
    ab = 0.0
    bc = 0.0
    ac = 0.0
    skalar = 0.0
    angle = 0.0

    ab = ((coor[atom_1][0] - coor[atom_2][0])**2 +
          (coor[atom_1][1] - coor[atom_2][1])**2 +
          (coor[atom_1][2] - coor[atom_2][2])**2)
    bc = ((coor[atom_2][0] - coor[atom_3][0])**2 +
          (coor[atom_2][1] - coor[atom_3][1])**2 +
          (coor[atom_2][2] - coor[atom_3][2])**2)
    ac = ((coor[atom_1][0] - coor[atom_3][0])**2 +
          (coor[atom_1][1] - coor[atom_3][1])**2 +
          (coor[atom_1][2] - coor[atom_3][2])**2)

    skalar = math.sqrt((ab) * (bc))
    angle = 0.5 * (ab + bc - ac)/skalar
    angle = math.acos(angle)
    return angle


def calcTorsion(
        alpha_a, alpha_b, phi_a, phi_b, r_a1, r_b1, r_a2, r_b2, r_a3, r_b3):

    psi_a = 0.0
    psi_b = 0.0
    if (((math.sin(alpha_a) * math.sin(phi_a)) != 0) and
            (r_a1 != -1) and (r_a2 != -1)):
        psi_a = ((math.cos(alpha_a) * math.cos(phi_a)) - math.cos(alpha_a)) /\
                 (math.sin(alpha_a) * math.sin(phi_a))
    else:
        psi_a = 0.0
        if (psi_a < 0.0):
            psi_a = 2 * math.pi + psi_a
        elif (psi_a >= 6.28318530717959):
            psi_a = 0.0

    if (((math.sin(alpha_b) * math.sin(phi_b)) != 0) and (r_b1 != -1) and
            (r_b2 != -1)):
        psi_b = ((math.cos(alpha_b) * math.cos(phi_b)) - math.cos(alpha_b)) / \
                 (math.sin(alpha_b) * math.sin(phi_b))
    else:
        psi_b = 0.0
        if (psi_b < 0.0):
            psi_b = 2 * math.pi + psi_b
        elif (psi_b >= 6.28318530717959):
            psi_b = 0.0
    return (psi_a, psi_b)


def calcCosPhi(
        phi_a, op_phi_a, psi_a, op_psi_a, phi_b, op_phi_b, psi_b, op_psi_b,
        r_a1, r_a2, r_a3, r_b1, r_b2, r_b3, atom_1, atom_2, atom_3):

    cos_phi_a = math.cos(phi_a - op_phi_a)
    cos_phi_b = math.cos(phi_b - op_phi_b)
    cos_psi_a = math.cos(psi_a - op_psi_a)
    cos_psi_b = math.cos(psi_b - op_psi_b)

    if (r_a1 == -1 and r_a2 == -1):
        cos_phi_a = 1.0
        cos_psi_a = 1.0
    if (r_b1 == -1 and r_b2 == -1):
        cos_phi_b = 1.0
        cos_psi_b = 1.0
    if (r_a1 == -1 or r_a2 == -1 or r_a3 == -1):
        cos_psi_a = 1.0
    if (r_b1 == -1 or r_b2 == -1 or r_b3 == -1):
        cos_psi_b = 1.0
    if (atom_1 == r_a1 or atom_2 == r_a1 or atom_3 == r_a3):
        cos_phi_a = 1.0
    if (atom_1 == r_b1 or atom_2 == r_b1 or atom_3 == r_b3):
        cos_phi_b = 1.0
    if (atom_3 == r_a1 or atom_3 == r_a2 or atom_3 == r_a3):
        cos_psi_a = 1.0
        cos_psi_b = 1.0
    if (atom_3 == r_b1 or atom_3 == r_b2 or atom_3 == r_b3):
        cos_psi_a = 1.0
        cos_phi_a = 1.0
        cos_phi_b = 1.0
    if (r_a1 == -1 and r_a2 == -1 and r_a3 == -1):
        cos_psi_a = 1.0
        cos_phi_a = 1.0
    if (r_b1 == -1 and r_b2 == -1 and r_b3 == -1):
        cos_psi_b = 1.0
        cos_phi_b = 1.0
    return (cos_phi_a, cos_psi_a, cos_phi_b, cos_psi_b)


def calcHbonds(
        natom, atomtypes, distance, elements, neighbour_matrix, rcov, coor,
        con_value):

    sum_hb_energy = 0.0

    for i in range(0, natom):
        if (atomtypes[i] == 'O' or atomtypes[i] == 'N'):
            atom_1 = i
        else:
            continue
        for j in range(atom_1 + 1, natom):
            if (atomtypes[j] == 'O' or atomtypes[j] == 'N'):
                atom_2 = j
            else:
                continue
            for k in range(0, natom):
                if (atomtypes[k] == 'H'):
                    if (neighbour_matrix[atom_1][k] == 1 or
                            neighbour_matrix[atom_2][k] == 1):
                        atom_3 = k

                        dist_ab, dist_xh = calcDistOfPair(distance, atom_1,
                                                          atom_2, atom_3)

                        r_a1, r_a2, r_a3 = getNextNeighbour(
                            atom_1, natom, neighbour_matrix, distance)

                        r_b1, r_b2, r_b3 = getNextNeighbour(
                            atom_2, natom, neighbour_matrix, distance)

                        theta = calcAngle(atom_2, atom_3, atom_1, coor)

                        phi_a = calcAngle(atom_3, atom_1, r_a1, coor)

                        phi_b = calcAngle(atom_3, atom_2, r_b1, coor)

                        alpha_a = calcAngle(atom_1, r_a1, r_a2, coor)

                        alpha_b = calcAngle(atom_2, r_b1, r_b2, coor)

                        psi_a, psi_b = calcTorsion(
                            alpha_a, alpha_b, phi_a, phi_b,
                            r_a1, r_b1, r_a2, r_b2, r_a3, r_b3)

                        op_phi_a, op_psi_a = calcOptimalAngle(
                            atom_1, r_a1, r_a2, r_a3, atom_3, atomtypes,
                            con_value)

                        op_phi_b, op_psi_b = calcOptimalAngle(
                            atom_2, r_b1, r_b2, r_b3, atom_3, atomtypes,
                            con_value)

                        cos_phi_a, cos_psi_a, cos_phi_b, cos_psi_b = calcCosPhi(
                            phi_a, op_phi_a, psi_a, op_psi_a, phi_b, op_phi_b,
                            psi_b, op_psi_b, r_a1, r_a2, r_a3, r_b1, r_b2,
                            r_b3, atom_1, atom_2, atom_3)

                        cos_theta = -math.cos(theta)

                        if (cos_theta < 0):
                            continue

                        sum_hb_energy = calcHBondEnergy(
                            atom_1, atom_2, atom_3, dist_xh, dist_ab,
                            cos_theta, cos_phi_a, cos_phi_b, cos_psi_a,
                            cos_psi_b, sum_hb_energy, atomtypes)
                        
    sum_hb_energy *= 627.51

    return sum_hb_energy


def calcHBondEnergy(
        atom_1, atom_2, atom_3, dist_xh, dist_ab, cos_theta, cos_phi_a,
        cos_phi_b, cos_psi_a, cos_psi_b, sum_hb_energy, atomtypes):

    covcut = 2.268      # 1.2 Angstrom
    shortcut = 4.535    # 2.4 Angstrom
    longcut = 13.228    # 7.0 Angstrom

    if (atomtypes[atom_1] == 'N' and atomtypes[atom_2] == 'N'):
        scale_ca = -0.11
        scale_cb = -0.11
    elif (atomtypes[atom_1] == 'N' and atomtypes[atom_2] == 'O'):
        scale_ca = -0.11
        scale_cb = -0.12
    elif (atomtypes[atom_1] == 'O' and atomtypes[atom_2] == 'N'):
        scale_ca = -0.12
        scale_cb = -0.11
    elif (atomtypes[atom_1] == 'O' and atomtypes[atom_2] == 'O'):
        scale_ca = -0.12
        scale_cb = -0.12

    scale = (scale_ca + scale_cb)/2.0

    # hb_correction
    f_bond = 1.00 - 1.00 / (1.00 + math.exp(-60.00 * (dist_xh/covcut - 1.00)))

    f_damp = (1.00 / (1.00 + math.exp(-100.00 * (dist_ab/shortcut - 1.00)))) \
             * (1.00 - 1.00
                / (1.00 + math.exp(-10.00 * (dist_ab/longcut - 1.00))))

    f_geom = cos_theta**2 * cos_phi_a**2 * cos_psi_a**2 * cos_phi_b**2 \
                * cos_psi_b**2

    f_damping = f_bond * f_damp * f_geom

    hb_correction = (scale / dist_ab**2) * f_damping

    # hb_energy
    sum_hb_energy += hb_correction

    return (sum_hb_energy)


if __name__ == "__main__":
    print "Test"

    f_elem = sys.argv[1]                          # chemical elements file name
    f_rcov = sys.argv[2]                          # covalent radius file name
    f_coor = sys.argv[3]                          # coordination file name
    elements = read_elem(f_elem)
    rcov = readCov(f_rcov)
    coor, atomtypes, natom = read_coors(f_coor)
    distance = calculateDistances(coor)
    neighbour_matrix = getNeighborMatrix(
        natom, atomtypes, distance, rcov, elements)
    con_value = getCoordinationNumber(
        natom, atomtypes, distance, rcov, elements)
    sum_hb_energy = calcHbonds(
        natom, atomtypes, distance, elements, neighbour_matrix, rcov, coor,
        con_value)
    print 'HBOND-ENERGY in Kcal/mol: ', sum_hb_energy
