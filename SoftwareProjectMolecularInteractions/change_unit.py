#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
sys.dont_write_bytecode = True

_CONVERT_AA_TO_AU = 1.889725989

def convertAngstromToAtomic(e):
	return e * _CONVERT_AA_TO_AU
	
def convertAtomicToAngstrom(e):
	return e / _CONVERT_AA_TO_AU

def EnergyAUToKcalPerMol(e):
	return e * 627.51
