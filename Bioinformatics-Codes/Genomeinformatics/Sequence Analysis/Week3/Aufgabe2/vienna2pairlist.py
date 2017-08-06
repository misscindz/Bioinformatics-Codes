#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import argparse

def getViennaNotationsFromFile(filename):
    vienna_notations = []
    f = open(filename,'r')
    for line in f:
        vienna_notations.append(line)
    return vienna_notations
    
def vienna2pairlist(vienna_notation):
    
    return
    
if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(
							prog="./vienna2pairlist.py",
							usage="%(prog)s <filename>",
							description="converts vienna notation to pairlist")
							
    parser.add_argument("viennaFile",type=str, nargs='?',
                            default = "Vienna-examples.txt",
                            help= "path to file containing vienna notation")
    args = parser.parse_args()
    filename = args.viennaFile
    vienna_examples = getViennaNotationsFromFile(filename)
    
    for example in vienna_examples:
        print example    
    
   
        