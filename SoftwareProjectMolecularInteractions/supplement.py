#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#Radii according to the MMFF94 Force Field Model: 
#For hydrogen a mean value was used

radii ={"H":1.38 , "C":2.15, "N":2.2, "O":1.85, "P":1.91, "S":2.1, 
		"F":1.5, "Cl": 2.1, "Br":2.3, "I":2.6, "C_pi":2.3,"N_pi":2.2, 
		"O_pi":1.56}
		
# empirical Coefficients according to the MMFF94 Force Field Model
#For hydrogen a mean value was used	
empiricalCoefficient ={"C":-1.1, "N":-5.48, "O":-1.67, 
					   "P":-6.13, "S":-0.32, "F":2.36, "Cl": 0.89, 
					   "Br":-0.56, "I":-4.47,"C_pi":-0.2,"N_pi":-2.08, 
					   "O_pi":3.95}

empiricalH = {"N": -1.16, "O": -1.99, "P": -7.73, "S": -1.16, "C": 0.12}
'''
rcoval = {"H":0.32 , "C":0.77, "N":0.71, "O":0.73, "P":1.06, "S":1.02, 
		"F":0.71, "Cl": 0.99, "Br":1.14, "I":1.33}

#Radii according to the AM1-BCC Charge Model: 
#For hydrogen a mean value was used

radii ={"H":1.15 , "C":1.8, "N":1.65, "O":1.35, "P":2.15, "S":1.95, 
		"F":1.5, "Cl": 1.8, "Br":2.4, "I":2.6, "C_pi":2.0,"N_pi":1.65, 
		"O_pi":5.0}
		
# empirical Coefficients according to the AM1-BCC Charge Model
#For hydrogen a mean value was used	
empiricalCoefficient ={"C":-0.28, "N":-4.04, "O":0.68, 
					   "P":-4.11, "S":-1.02, "F":1.93, "Cl": 0.01, 
					   "Br":-1.16, "I":0.08,"C_pi":-0.12,"N_pi":-0.62, 
					   "O_pi":3.95}

empiricalH = {"N": 1.04, "O": 2.24, "P": 1.43, "S": 0.87, "C": 0.24}

