#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys; sys.dont_write_bytecode = True
import os

def enToDict(fin):
#Reads en.dat and translates electronagativities into en_dict
	en_dict = {}
	dictelem = [0 for x in range(2)]
	linecount = 0
	f = open(fin,'r')
	for line in f:
		words = line.split()

		if words[0] != '#':
			en_dict.update({str(words[0]).upper():float(words[1])})
	f.close()
	
	return en_dict
	
