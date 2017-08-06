#!/usr/bin/python

#Gruppe Ching, Achoukhi, Breitschuh
#GIK Blatt3, Aufgabe1

import sys
import os
import argparse


range1 = lambda start, end: range(start, end+1)

#reads sequences from file and stores them in list
def getTestSequences(filename):
    rnaseqs = []
    f = open(filename,'r')
    for line in f:
        if ("#" not in line) and (">" not in line):
            rnaseqs.append(line)
    return rnaseqs

#prints matrix, first row and first column are omitted
def printMatrix(Emat):
    for row in Emat[1:]:
        for value in row[1:]:
           sys.stdout.write('{:>4}'.format(value))
        print

#defines energyCost of base pair
def energyCost(base1,base2,option):
    #cost function used by default
    if option==1:
        if ((base1,base2) == ('G','C')) or((base1,base2) == ('C','G')):
            return -3
        if ((base1,base2) == ('A','U')) or((base1,base2) == ('U','A')):
            return -2
        if ((base1,base2) == ('G','U')) or ((base1,base2) ==('U','G')):
            return -1
        else:
            return float("inf")

    #cost function used in script example
    if option==2:
        if ((base1,base2) == ('G','C')) or((base1,base2) == ('C','G')):
            return -1
        if ((base1,base2) == ('A','U')) or((base1,base2) == ('U','A')):
            return -1
        if ((base1,base2) == ('G','U')) or ((base1,base2) ==('U','G')):
            return -1
        else:
            return float("inf")

def minimum(i,j,E):
    minvalue = 0
    firstIteration=True
    for k in range1(i+2,j-1):
        if firstIteration:
            minvalue = E[i][k-1]+E[k][j]
            firstIteration=False
        if not firstIteration:
            result = E[i][k-1]+E[k][j]
            minvalue=min(result,minvalue)
    return minvalue
    
# The nussinov-algorithm computes a matrix E.
# E(i,j) is the minimum free energy of all secondary structures for w[i...j].
def nussinov(sequence,l_min,cost_mode):

    n=len(sequence)
    #initalizes (n+1)x(n+1) matrix.
    #first row and first column are additional to avoid index confusion
    E = [[0 for x in range(n+1)] for y in range(n+1)]
    for l in range1(2,n):
        for i in range1(1,n-l+1):
            j=i+l-1
            if j-i-1 < l_min:
                E[i][j]=0
            else:
            
                E[i][j] = min(
                            E[i+1][j-1]+energyCost(sequence[i-1],sequence[j-1],cost_mode),
                            E[i+1][j],
                            E[i][j-1],
                            minimum(i+2,j-1,E))
    return E

#traceback function to print RNA structure with optimal energy value
def traceback(i,j,E,seq,cost_mode):
    if i<j:
        # print energyCost(seq[i-1],seq[j-1],cost_mode)
        if E[i][j] == E[i+1][j-1] + energyCost(seq[i-1],seq[j-1],cost_mode):
            print "(%d,%d)" %(i,j)
            traceback(i+1,j-1,E,seq,cost_mode)
        else:
            if E[i][j] == E[i+1][j]:
                traceback(i+1,j,E,seq,cost_mode)
            else: 
                if E[i][j] == E[i][j-1]:
                    traceback(i,j-1,E,seq,cost_mode)
                else:
                    for k in range1(i+2,j-1): 
                        if E[i][j] == E[i][k-1] + E[k][j]:
                            traceback(i,k-1,E,seq,cost_mode)
                            traceback(k,j,E,seq,cost_mode)
                            break
                                
    
if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(
							prog="./nussinov.py",
							usage="%(prog)s <rna sequence>",
							description="calculates ")
							
    parser.add_argument("rnaseq",type=str, nargs='?',
                            default = "GGGAAAUCC",
                            help= "rna sequence")
                            
    parser.add_argument("-pm","--printMatrix",
    	                    action="store_true",    
						    default = False,
       						help="prints E-Matrix.")
    parser.add_argument("-ls","--seqLen",
    	                    action="store_true",    
						    default = False,
       						help="prints length of sequence.")
    parser.add_argument("-lmin","--lMinHairpin",
                            type = int,   
						    default = 3,
       						help="define l_min")
    parser.add_argument("-cst","--cost",
                            type = int,   
						    default = 1,
       						help="define cost mode (-1,-1,-1,inf) or "
       						+ "(-3,-2,-1,inf)")
       						
    parser.add_argument("-tst","--test",
                            action="store_true",    
						    default = False,
       						help="performs test")
     
       						
    args = parser.parse_args()
    sequence = args.rnaseq
    pm = args.printMatrix
    ls = args.seqLen
    lmin = args.lMinHairpin
    cst = args.cost
    tst = args.test
    
    if tst:
        testseqs = getTestSequences("phenyalanines.txt")
        for testseq in testseqs:
            j=len(testseq) 
            print "<fold2Dmulti>\n<fold2D>"
            seqtab = "<seq>"+testseq.lower()+"</seq>"
            seqtab = (''.join(seqtab.split()))
            print seqtab
            print "<pairs>"
            # print "<seq>%s</seq>\n<pairs>" % testseqs[x].lower()
            E_Mat = nussinov(testseq,lmin,cst)
            traceback(1,j,E_Mat,testseq,cst) 
            print "</pairs>\n<mfe>%d</mfe>\n</fold2D>" % E_Mat[1][j]
            
            if pm:
                print "   ******  E-MATRIX  ******"
                printMatrix(E_Mat)
        pm=False
                
        
    else:
        j=len(sequence)
        print "<fold2Dmulti>\n<fold2D>"
        seqtab = "<seq>"+sequence.lower()+"</seq>"
        seqtab = (''.join(seqtab.split()))
        print seqtab
        print "<pairs>"
        
        E_Mat = nussinov(sequence,lmin,cst)
        traceback(1,j,E_Mat,sequence,cst)
        print "</pairs>\n<mfe>%d</mfe>\n</fold2D>" % E_Mat[1][j] #verstehe ich nicht, warum
    
    if pm:
        print "   ******  E-MATRIX  ******"
        printMatrix(E_Mat)
    if ls:
        print "\nlength of sequence: ",j