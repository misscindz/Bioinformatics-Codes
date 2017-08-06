#!/usr/bin/python

import sys

def cost(a,b):
	if a == b:
		return 0
	else:
		return 1

def fillDPtable(E, u, v, m, n):
	
	E[0][0] = 0
	
	for i in range(1,m+1):
		E[i][0] = E[i-1][0] + 1

	for j in range(1,n+1):
		E[0][j] = E[0][j-1] + 1
		for i in range (1,m+1):
			E[i][j] = min(E[i-1][j]+1, E[i][j-1]+1, E[i-1][j-1]+cost(u[i-1],v[j-1])) 

	print "%s\t%s\t%d" % (u, v, E[m][n])

	return


if __name__ == '__main__':

	seq1=sys.argv[1]
	seq2=sys.argv[2]
	m = len(seq1)
	n = len(seq2)
	assert (m <= 100 or n <= 100 ),"sequence maximum length = 100"

	E_Matrix = [[0 for x in range(n+1)] for y in range(m+1)]

	fillDPtable(E_Matrix, seq1, seq2, m, n)
