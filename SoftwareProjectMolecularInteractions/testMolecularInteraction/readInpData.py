#!/usr/bin/python
# -*- coding: utf-8 -*-

import argparse
import sys
import numpy as np

def readInp(fin):

    # Skip comment line.
    fin.readline()
    # Read unit.
    fin.readline()
    # Cnt_of_sites 
    # | method 
    # | (POINT:0|DIPOLE:1|QUADRUPOLE:2|OCTUPOLE:3) 
    # | order for polarisation (NONE|ISO|ANSITROPIC).
    l = fin.readline()
    natoms = int(l[0])
    pointCharge = []
    disp = []
    moleculeOfAtom = []
    
    # create table of 'size number of atoms'x3
    coords = np.empty([natoms, 3], dtype="float64")
    for x in coords:
        line = fin.readline().split()
        moleculeOfAtom.append(int( line[0]) )
        x[:] = map(float, line[1:4])
        pointCharge.append(float( line[4]) )
        disp.append(float( line[5]))

    return (moleculeOfAtom, coords, pointCharge, natoms, disp)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Reads "\
    " inp-files")
    parser.add_argument('files', metavar='files', nargs='*',type=str,
        help="Paths to the xyz-files which shall be read.")


    args = parser.parse_args()
    files = args.files
    disp = None
    
    for f in files:
        fin = open(f, "r")
        coords, pointCharge, natoms, disp = readInp(fin)
        fin.close()
    
    print natoms
    print coords
    print pointCharge
