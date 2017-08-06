#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys; sys.dont_write_bytecode = True
import argparse
import numpy as np
from change_unit import convertAngstromToAtomic

def readXYZ(fin):
    """ read a xyz file from file handle
    
    Parameters
    ----------
    fin : file handle
        file to read from
    
    Returns
    -------
    coords      : ndarray,  
                    shape (number of atoms, 3), 
                    dtype="float64"
                  list containing positions of atoms in the 
                  corresponding molecule
    atomTypes   : array,
                    type String
                  contains the atomTypes of all atoms in the given molecule
    natoms      : Number of atoms in file.
    comment     : String,
                    comment line of the given file
    """
    # read first line which schould contain the number of atoms
    natoms = int(fin.readline())
    # read comment line without newline
    comment = fin.readline()[:-1]
    # create table of 'size number of atoms'x3
    coords = np.zeros([natoms, 3], dtype="float64")
    atomTypes = []
    moleculeIds = []
    
    for x in coords:
        line = fin.readline().split()
        moleculeIds.append(line[0])
        atomTypes.append(line[1])
        x[:] = map(float, line[2:5])
        x[:] = map(convertAngstromToAtomic, x)
    return (coords, atomTypes, natoms, moleculeIds, comment)

def readXYZSplit(fin):
    """ read a xyz file from file handle
    
    Parameters
    ----------
    fin : file handle
        file to read from
    
    Returns
    -------
    coords      : ndarray,  
                    shape (number of atoms, 3), 
                    dtype="float64"
                  list containing positions of atoms in the 
                  corresponding molecule
    atomTypes   : array,
                    type String
                  contains the atomTypes of all atoms in the given molecule
    natoms      : Number of atoms in file.
    comment     : String,
                    comment line of the given file
    """
    # read first line which schould contain the number of atoms
    natoms = int(fin.readline())
    # read comment line without newline
    comment = fin.readline()[:-1]
    # create table of 'size number of atoms'x3
    coords = np.zeros([natoms, 3], dtype="float64")
    atomTypes = []
    moleculeIds = []
    coords = np.zeros([natoms, 3], dtype="float64")
    atomTypes = []
    atomPositionsDict = {0:[]}
    atomTypesDict = {0:[]}
    moleculeIdDict = {0:[]}
    
    for i in range(natoms):
        line = fin.readline().split()
        moleculeId = int(line[0])
        #moleculeIds.append(line[0])
        #atomTypes.append(line[1])
        v = map(float, line[2:5])
        v[:] = map(convertAngstromToAtomic, v)
        # in case molecule id appears the first time
        if not moleculeId in atomPositionsDict.keys():
            atomPositionsDict[moleculeId] = []
            atomTypesDict[moleculeId] = []
            moleculeIdDict[moleculeId] = []
        # positions
        atomPositionsDict[0].append(v)
        atomPositionsDict[moleculeId].append(v)
        # atomtypes
        atomTypesDict[0].append(line[1])
        atomTypesDict[moleculeId].append(line[1])
        # molecule is
        moleculeIdDict[0].append(moleculeId)
        moleculeIdDict[moleculeId].append(moleculeId)
    n = {}
    for k in atomTypesDict.keys():
        n[k] = len(atomTypesDict[k])
            
    return (
        atomPositionsDict, 
        atomTypesDict, 
        n, 
        moleculeIdDict, 
        comment)

def readXyzSimple(fin):
    """ read a xyz file from file handle
    
    Parameters
    ----------
    fin : file handle
        file to read from
    
    Returns
    -------
    coords      : ndarray,  
                    shape (number of atoms, 3), 
                    dtype="float64"
                  list containing positions of atoms in the 
                  corresponding molecule
    atomTypes   : array,
                    type String
                  contains the atomTypes of all atoms in the given molecule
    natoms      : Number of atoms in file.
    comment     : String,
                    comment line of the given file
    """
    # read first line which schould contain the number of atoms
    natoms = int(fin.readline())
    # read comment line without newline
    comment = fin.readline()[:-1]
    # create table of 'size number of atoms'x3
    coords = np.zeros([natoms, 3], dtype="float64")
    atomTypes = []
    for x in coords:
        line = fin.readline().split()
        atomTypes.append(line[0])
        x[:] = map(float, line[1:4])
        x[:] = map(convertAngstromToAtomic, x)
        
    return (coords, atomTypes, natoms, comment)
    
if __name__ == "__main__":
    '''
    argument parser to read command line arguments
    example calls:
    ./read_xyz.py ./fileOne ./folder/fileTwo
    ./read_xyz.py -h
    '''
    parser = argparse.ArgumentParser(description="Reads "\
    " xyz-files and prints coordinates, comment line and atomTypes.")
    parser.add_argument('files', metavar='files', nargs='*',type=str,
        help="Paths to the xyz-files which shall be read.")


    args = parser.parse_args()
    files = args.files
    
    for f in files:
        fin = open(f, "r")
        coords, title, atomTypes = readXYZ(fin)
        fin.close()
    
        print coords
        print title
        print atomTypes
