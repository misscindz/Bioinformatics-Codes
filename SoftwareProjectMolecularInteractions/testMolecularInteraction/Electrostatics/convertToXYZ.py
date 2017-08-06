#!/usr/bin/python
# -*- coding: utf-8 -*-

#Datei liesst die ganze Scheisse aus "myq_vs_cm5.out" ein und macht xyz Files daraus

import sys
import argparse
import os

def convert2XYZ(fin):
    file = open(fin,"r")
    firstFile = True;
    newFile = ""
    atoms = 0
    
    for line in file:
        if line.strip():
            words = line.split()
            if words[0] == "===>":
                if firstFile == False:
                    generatedFile = open(newFile,"a")
                    tempfile = open(temp, "r+")
                    generatedFile.write(str(atoms)+"\n\n" + tempfile.read())
                    tempfile.close()
                    os.remove(temp)
                    generatedFile.close()
                    atoms = 0
                newFile = words[1]
                temp = "temp.xyz"
                tempfile = open(temp, "a")
                firstFile = False
            else:
                # startNewFile = False
                word3 = words[3]
                tempfile.write(words[0]+"\t"+words[2]+"\t"+word3[:-1]+"\t"+words[4]+"\n")
                atoms += 1
    tempfile.close()
    os.remove(temp)
    generatedFile.close()    

def getPartialCharges(fin):
    file = open(fin,"r")
    partialCharges = []
    isFirstRound = True
    for line in file:
       if line.strip():
            words = line.split()
            if words[0] != "===>":
                partialCharges.append(float(words[7]))
                isFirstRound = False
            else:
                molecule = words[1]
                if isFirstRound == False:
                    print "partialCharge"+molecule[:-4],"= ",partialCharges,"\n"
                    partialCharges = []
                    
                

if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(description="convert to xyz file from reference file")
    parser.add_argument("filename")
    parser.parse_args()
    fin = sys.argv[1]
    # convert2XYZ(fin)
    getPartialCharges(fin)
    