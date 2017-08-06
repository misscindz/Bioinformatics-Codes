#!/usr/bin/python
# -*- coding: utf-8 -*-

import re, sys; sys.dont_write_bytecode = True
import numpy as np

#rcov_datei parsen

def readCov(fin):
    """
    Given a file name 
    Return a list of covalent radius
    """
    rcov_file = open(fin,"r")
    pre_rcov = []
    rcov = []
    for line in rcov_file.readlines():
        if re.match(r"^\s+\.",line):
            r_line = line.split()
            pre_rcov.append(r_line[1:6])
    for i in range (0,len(pre_rcov)):
        for j in range (0,len(pre_rcov[i])):
            new_rcov = pre_rcov[i][j].split(',')
            rcov.append(new_rcov[0])
    for n in range(0,len(rcov)):
        if re.match('\D',rcov[n]):
            rcov.pop(n)
    rcov_file.close()
    return rcov
