def getCovVector(
			atomtypes,
			elements):
	cov_vec = []
	for atom in atomtypes:
		cov_vec.append(r_in_AU[elements.index(atom)+1])	#r_in_AU from CM5radii.py
	return cov_vec
	
''' constants from CM5 paper supplementary info
	DZ is atom dependant parameter saved as dictionary
	alpha is gained by parametrization process
'''

alpha = 2.474/1.8897 #original unit 1/Angstroem, now a.u.
DZ_params = {	"H"	: 0.0056,
				"C" :-0.0446,
				"N" :-0.1072,
				"O" :-0.0802
			}
				
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
		T = 0.0
	
	else:	
		T=DZ_params[atom1]-DZ_params[atom2]
	
	return T

def getProcuctTB(
			atom1_type,
			atom2_type,
			distance_atom1_atom2,
			covalenceRadius_atom1,
			covalenceRadius_atom2):

	T=get_T(atom1_type,atom2_type)
	B=math.exp((-1.0)*(alpha)*(float(distance_atom1_atom2)\
									- float(covalenceRadius_atom1)\
									- float(covalenceRadius_atom2)))

def cm5correction(
			partialCharges,
			natoms,
			atomTypes,		
			distanceMatrix, #in a.u.
			elements):
			
	corrPartialCharges = []
	covRadii = getCovVector(atomTypes,elements) #in a.u.
	for atom1 in range(natoms):
		sum_TB = 0.0
		for atom2 in range(natoms):
			if atom1 != atom2:
				sum_TB += getProcuctTB(atomTypes[atom1],atomTypes[atom2],\
							distanceMatrix[atom1][atom2],covRadii[atom1],\
							covRadii[atom2])
		corrPartialCharges.append(partialCharges[atom1]+sum_TB)
	return corrPartialCharges
	

	
