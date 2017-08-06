#!/usr/bin/python
# -*- coding: utf-8 -*-

import re, sys; sys.dont_write_bytecode = True
import numpy as np
#reference_dispersion parsen (in a list of lists)

def readRC6(fin):
    """
        given the file name
        return a list of lists of reference c6 values
    """
    r_c6_file = open(fin)
    """
    this part get the number of lines in the file
    nlines = 0
    for line in r_c6_file.readlines():
    if  re.search('nlines',line):
        nline_attr = line.split('=')
        nlines = int(nline_attr[1].strip(' '))
    r_c6_file.seek(0)
    """
    m = 0
    r_c6_list = []
    for line in r_c6_file.readlines():
        if re.match(r'^\s+\.',line):
            if not re.match(r'^\s+\.\/',line):
                line = line.rstrip('\n')
                line = line.replace('D','E')
                line = line.split('.',1)[1]
                line = line.split(',')
                length = len(line)
                new = []
                if length == 5:
                    new.append(float(line[0]))
                    new.append(float(line[1]))
                    new.append(float(line[2]))
                    new.append(float(line[3]))
                    new.append(float(line[4]))
                if length == 6:
                    new.append(float(line[1]))
                    new.append(float(line[2]))
                    new.append(float(line[3]))
                    new.append(float(line[4]))
                    new.append(float(line[5]))
            
                r_c6_list.append(new)
    
    r_c6_file.close
    return r_c6_list
