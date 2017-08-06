''' convert en.dat to dictionary containing electronegativities
'''

from read_en import enToDict
import sys; sys.dont_write_bytecode = True

fin = 'data/sw_project_data/en.dat'
en_dict = {'BE': 1.47, 'C': 2.5, 'B': 2.01, 'CO': 1.7, 'BR': 2.74, 'F': 4.17, 'ZN': 1.66, 'H': 2.2, 'CA': 1.04, 'AL': 1.47, 'O': 3.5, 'N': 3.07, 'P': 2.06, 'S': 2.44, 'FE': 1.64, 'GA': 1.82, 'V': 1.45, 'CR': 1.56, 'CU': 1.75, 'AS': 2.2, 'NI': 1.75, 'MG': 1.23, 'NA': 1.01, 'CL': 2.83, 'GE': 2.02, 'LI': 0.97, 'SI': 1.74, 'MN': 1.6, 'TI': 1.32, 'SC': 1.2, 'K': 0.91, 'SE': 2.48}


'''	dielectricity constants. 
	e_0 for in vacuo, e_r for water 
 	e_0 = 8.84*10^-12 	---> dielektrische Feldkonstante
 	e_r = 81 			---> stoffspezifischer Wert
'''
e_0 = 0.00000000000884
e_r = 81


''' constants from CM5 paper supplementary info
	DZ is atom dependant parameter saved as dictionary
	alpha is gained by parametrization process
'''

alpha = 2.474
DZ_params = {	"H"	: 0.0056,
				"HE":-0.543,
				"LI": 0,
				"BE": 0.0333,
				"B"	:-0.1030,
				"C" :-0.0446,
				"N" :-0.1072,
				"O" :-0.0802,
				"F" :-0.0629,
				"NE":-0.1088,
				"NA": 0.0184,
				"MG": 0,
				"AL":-0.0726,
				"SI":-0.0790,
				"P" :-0.0756,
				"S" :-0.0565,
				"CL":-0.0444,
				"AR":-0.0767,
				"K" : 0.0130,
				"CA": 0,
				"SE": 0,
				"TI": 0,
				"V" : 0,
				"CR": 0,
				"MN": 0,
				"FE": 0,
				"CO": 0,
				"NI": 0,
				"CU": 0,
				"ZN": 0,
				"GA":-0.0512,
				"GE":-0.0557,
				"AS":-0.0533,
				"SE":-0.0399,
				"BR":-0.0313,
				"KR":-0.0541,
				"RB": 0.0092,
				"SR": 0,
				"Y" : 0,
				"ZR": 0,
				"NB": 0,
				"MO": 0,
				"TC": 0,
				"RU": 0,
				"RH": 0,
				"PD": 0,
				"AG": 0,
				"CD": 0,
				"IN":-0.0361,
				"SN":-0.0393,
				"SB":-0.0376,
				"TE":-0.0281,
				"I" :-0.0220,
				"XE":-0.0381,
				"CS": 0.0065,
				"BA": 0,
				"LA": 0,
				"HF": 0,
				"TA": 0,
				"W" : 0,
				"RE": 0,
				"OS": 0,
				"IR": 0,
				"PT": 0,
				"AU": 0,
				"HG": 0,
				"TI":-0.0255,
				"PB":-0.0277,
				"BI":-0.0265,
				"PO":-0.0198,
				"AT":-0.0155,
				"RN":-0.0269,
				"FR": 0.0046,
				"RN": 0,
				"AC": 0,
				"RF": 0,
				"DB": 0,
				"SG": 0,
				"BH": 0,
				"HS": 0,
				"MT": 0,
				"DS": 0,
				"UUB": 0,
				"UUT":-0.0179,
				"UUQ":-0.0195,
				"UUP":-0.0187	}
				
def get_T(atom1, atom2):
    		#exceptions for specific atomic combinations return fixed values
	if atom1 == 'H' and atom2 == 'C':
		T = 0.0502
	elif atom1 == 'H' and atom2 == 'N':
		T = 0.1747
	elif atom1 == 'H' and atom2 == 'O':
		T = 0.1671
	elif atom1 == 'C' and atom2 == 'N':
			T = 0.0556
	elif atom1 == 'C' and atom2 == 'O':
		T = 0.0234
	elif atom1 == 'N' and atom2 == 'O':
		T = -0.0346
	elif atom1 == 'C' and atom2 == 'H':
		T = -0.0502
	elif atom1 == 'N' and atom2 == 'H':
		T = -0.1747
	elif atom1 == 'O' and atom2 == 'H':
			T = -0.1671
	elif atom1 == 'N' and atom2 == 'C':
		T = -0.0556
	elif atom1 == 'O' and atom2 == 'C':
		T = -0.0234
	elif atom1 == 'O' and atom2 == 'N':
		T = 0.0346
	elif atom1 == atom2:
		T = 0.0000
	
	else:	
		T=DZ_params[atom1]-DZ_params[atom2]
	
	return T

				
''' valences of elements
	the valences of electrons are saved into dictionary
'''				
valence_dict = {"H":1,
				"C":4,
				"N":5,
				"O":6,
				"S":6,
				"P":5,
				"CL":7
			}
		

				
				
