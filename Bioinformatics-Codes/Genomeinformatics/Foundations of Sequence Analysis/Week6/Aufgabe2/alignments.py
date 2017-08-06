#!/usr/bin/python

import sys
import editdist

class DPMatrix:
	
	def __init__(self,u,v):
		self.u = u
		self.v = v
		self.m = len(u)
		self.n = len(v)
	
	def cost(self,a,b):
		if a == b:
			return 0
		else:
			return 1	
		
	def calcEdist(self):
			
		self.eMat = [[[ 0 for z in range(2)] \
							for x in range(self.n+1)] for y in range(self.m+1)]

		self.eMat[0][0][0] = 0
		self.eMat[0][0][1] = bin(0)
		
		for i in range(1,self.m+1):
			self.eMat[i][0][0] = self.eMat[i-1][0][0] + 1
			self.eMat[i][0][1] = bin(1)

		for j in range(1,self.n+1):
			self.eMat[0][j][0] = self.eMat[0][j-1][0] + 1
			self.eMat[0][j][1] = bin(2)
		
			for i in range (1,self.m+1):
				self.eMat[i][j][0] = min(self.eMat[i-1][j][0]+1, self.eMat[i][j-1][0]+1, \
								self.eMat[i-1][j-1][0]+self.cost(u[i-1],v[j-1])) 
				
				if self.eMat[i][j][0] == self.eMat[i-1][j][0]+1: 	#insertion
					self.eMat[i][j][1] = bin(1)
				

				if self.eMat[i][j][0] == self.eMat[i][j-1][0]+1: 	#deletion
					self.eMat[i][j][1] = bin(2)
				
				if self.eMat[i][j][0] == self.eMat[i-1][j-1][0]+1:	#replacement
					self.eMat[i][j][1] = bin(3)
				
	def editcost(self,m=None,n=None):
		m=m or self.m
		n=n or self.n
		return self.eMat[m][n][0]

	def printMat(self,m,n):
		for i in range(m+1):
			for j in range(n+1):
				print "%3d" %self.eMat[i][j][0],
			else:
				print
			print "\n"
			
			
class Alignment:
	
	def __init__(self,u,v):
		self.u = u
		self.v = v
		self.m = len(u)
		self.n = len(v)
		self.eOps = []
			
	def add_edit_operation(self, operation, num_of_ops):
		if (not self.eOps) or self.eOps[-1][0] != operation:
			self.eOps.append([operation,num_of_ops])
		else:
			self.eOps[-1][1]+=num_of_ops

	def alignment_add_replacement(self, num_of_ops = 1):
		self.add_edit_operation('R',num_of_ops)		
		
	def alignment_add_deletion(self, num_of_ops = 1):
		self.add_edit_operation('D', num_of_ops)	

	def alignment_add_insertion(self, num_of_ops = 1):		
		self.add_edit_operation('I',num_of_ops)

	def alignment_show(self):
		uidx=0
		vidx=0
		top =""
		middle = ""
		bottom = ""
		loops = 0
		
		for element in self.eOps:
			operation = element[0]
			repetitions = element[1]

			for repeat in range(repetitions):

				if operation == 'R':					
					top += self.u[uidx]					
					if self.u[uidx] == self.v[vidx]:
						middle += "|"
					else:
						middle += " "				
					bottom += self.v[vidx]					
					uidx += 1
					vidx += 1
				
				if operation == 'I':
					top += '-'
					middle += " "	
					bottom += v[vidx]				
					vidx += 1
					
				if operation == 'D':
					top += u[uidx]
					middle += " "	
					bottom += "-"					
					uidx += 1	
		print str(top)
		print str(middle)
		print str(bottom)
		 	

	def alignment_evalcost(self,m=None,n=None):
		m=m or self.m
		n=n or self.n
		editmat = DPMatrix(self.u,self.v)
		editmat.calcEdist() 
		print "u = %s\nv = %s\t%d" % (u, v, editmat.editcost(m,n))
		
		
	def traceback_alignment(self):
		tracemat = DPMatrix(self.u,self.v)
		for i in reversed(range(m+1)): #count backwards
			for j in reversed(range(n+1)):
				if edit
				minimumCost = min(self[i-j][j][0],self[i][j-1][0], \
									self[i-1][j-1])
			
		
if __name__ == '__main__':

#	if len(sys.argv)<3:
#		print "USAGE: ./editdist.py <string1> <string2>"
#		sys.exit(1)
#		
#	seq1=sys.argv[1]
#	seq2=sys.argv[2]
	
	u = "acgtagatatatagat"
  	v = "agaaagaggtaagaggga"
  	  		
  	a = Alignment(u,v)
	a.alignment_add_replacement(7)
	a.alignment_add_insertion(2)
	a.alignment_add_replacement(2)
	a.alignment_add_deletion()
	a.alignment_add_replacement(3)
	a.alignment_add_insertion()
	a.alignment_add_replacement(3)	
	a.alignment_evalcost()
	a.alignment_show()
	

