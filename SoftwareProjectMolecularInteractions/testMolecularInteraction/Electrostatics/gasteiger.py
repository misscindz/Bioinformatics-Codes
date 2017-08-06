def calcElecnum(alpha):
	sum1 = 0
	for beta in range(natoms):
		if beta != alpha:
			sum1 += (enVector[alpha]-enVector[beta])/pow(distanceMatrix[alpha][beta],6)
	return (valenceVector[alpha] + sum1)

def calculateGasteigerCharges(natoms,enVector,valenceVector,distanceMatrix,atomtypes,covalenceRadii):
	""" Calculates partial charges
	
	Parameters
    ----------
	natoms			: total number of atoms (vector)
	enVector		: electronegativities (vector)
	valenceVector	: valence electrons (vector)
	distanceMatrix	: distances between atoms (2-dim matrix)
	
    Returns
    -------
	partialCharges	: partial charges (vector)
	"""
	partialCharges = []
	total_electrons = sum(valenceVector)
	
	for element in partialCharges:
		element = 0
	
	for alpha in range(natoms):
		elecnum_alpha = calcElecnum(alpha)
		print elecnum_alpha
		sum_elecnum_beta = 0
		for beta in range(natoms):
			if alpha != beta:
				elecnum_beta = calcElecnum(beta)
				sum_elecnum_beta += elecnum_beta
		partialCharges.append(((elecnum_alpha-valenceVector[alpha])*total_electrons)/sum_elecnum_beta)		
	return partialCharges
