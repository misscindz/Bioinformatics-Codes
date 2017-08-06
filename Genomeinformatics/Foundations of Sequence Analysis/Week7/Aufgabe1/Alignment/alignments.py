#!/usr/bin/python

import sys
from collections import defaultdict

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
				self.eMat[i][j][0] = min(self.eMat[i-1][j][0]+1,
					self.eMat[i][j-1][0]+1,
					self.eMat[i-1][j-1][0]+self.cost(u[i-1],v[j-1])) 
				
				if self.eMat[i][j][0] == self.eMat[i-1][j][0]+1: 	#insertion
					self.eMat[i][j][1] = bin(1)			

				if self.eMat[i][j][0] == self.eMat[i][j-1][0]+1: 	#deletion
					self.eMat[i][j][1] = bin(2)
				
				if self.eMat[i][j][0] == self.eMat[i-1][j-1][0]+self.cost(u[i-1],v[j-1]):	#replacement
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
		for i in range(m+1):
			for j in range(n+1):
				print "%4s" %str(self.eMat[i][j][1]),
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
		self.traceback_sum = 0
			
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
		self.editmat = DPMatrix(self.u,self.v)
		self.editmat.calcEdist() 
		print "u = %s\nv = %s\t%d" % (u, v, self.editmat.editcost(m,n))
		
		
	def traceback_alignment(self,i=None,j=None):
				
		i=i or self.m
		j=j or self.n
		tracedict = defaultdict(list)
		
		tracedict = {	1:self.editmat.eMat[i-1][j-1][0],	#replacement
				2:self.editmat.eMat[i][j-1][0],		#deletion	
				3:self.editmat.eMat[i-1][j][0]		#insertion
				}
#		if tracedict[1] == 0: 
#			print "traceback:",self.traceback_sum
#			return
#		if tracedict[2] == 0:
#			print "traceback:",self.traceback_sum
#			return
#		if tracedict[3] == 0:
#			print "traceback:",self.traceback_sum
#			return
		
		if tracedict[1] == 0: 
			self.traceback_sum += self.editmat.cost(self.u[i-2], self.v[j-2])
			print "traceback:",self.traceback_sum
			return
		if tracedict[2] == 0:
			print "traceback:",self.traceback_sum
			return
		if tracedict[3] == 0:
			print "traceback:",self.traceback_sum
			return
		if tracedict[1] <= tracedict[2]:
			if tracedict[1] <= tracedict[3]:
				print 'R'
				self.traceback_sum += self.editmat.cost(self.u[i-2], self.v[j-2])
				return self.traceback_alignment(i-1,j-1)
			self.traceback_sum += 1
			print 'I'			
			return self.traceback_alignment(i-1,j)
		if tracedict[2] <= tracedict[3]:
			print 'D'
			self.traceback_sum += 1
			return self.traceback_alignment(i,j-1)	
		self.traceback_sum += 1	
		print 'I'		
		return self.traceback_alignment(i-1,j)	

				
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
	a.traceback_alignment()
	a.alignment_show()
	

