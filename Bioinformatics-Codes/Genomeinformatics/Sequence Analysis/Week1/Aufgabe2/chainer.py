#!/usr/bin/env python3

# Gruppe: Achoukhi, Breitschuh, Ching
# Irgendwo befindet sich noch ein Fehler, Ergebniss falsch!

import sys
import re

class Match:

  def __init__ (self,line):
    info = line.split('\t')
    self.fu_begin = int(info[0])
    self.fu_end   = int(info[1])
    self.fv_begin = int(info[2])
    self.fv_end   = int(info[3])
    self.weight   = int(info[4])

def parse (lines):
  f = []
  for line in lines:
    f.append(Match(line))
  f.sort(key=lambda x: x.fu_end)
  chainingMatch(f)


def calcOptChain (prec, pos, list):
  list.append(pos)
  if prec[pos] == None:
    return list
  calcOptChain(prec,prec[pos],list)

def covered (f, opt_chain):
    len_x, len_y = 0, 0
    reverse_optchain = opt_chain[::-1]
    for i in reverse_optchain:
      print("%s %s %s %s %s" %(f[i].fu_begin, f[i].fu_end,
                               f[i].fv_begin, f[i].fv_end, f[i].weight))
      len_x += (f[i].fu_end - f[i].fu_begin) + 1
      len_y += (f[i].fv_end - f[i].fv_begin) + 1
    
    coverage1 = round((len_x / int(lengthSeq1))*100,2)
    coverage2 = round((len_y / int(lengthSeq2))*100,2)
    return len_x, len_y, coverage1, coverage2

def chainingMatch (f):
  overallmaxscore = 0
  bestmatch = None
  prec, score = [], []
  for j in range(0,len(f)-1):
    maxscore = f[j].weight
    prec.append(None)
    for i in range(0,j-1):
      if (f[i].fu_end < f[j].fu_begin and f[i].fv_end < f[j].fv_begin and
          maxscore < (int(score[i]) + f[j].weight)):
        maxscore = score[i] + f[j].weight
        prec[j] = i
    score.append(maxscore)
    if overallmaxscore < maxscore:
      overallmaxscore = maxscore
      bestmatch = j
  opt_chain = []
  calcOptChain(prec,bestmatch,opt_chain)
  len_x, len_y, coverage1, coverage2 = covered(f,opt_chain)
  
  print("# optimal chain of length %s with score %s"
         %(len(opt_chain),overallmaxscore))
  print("# %s bp covered on sequence 1 (coverage %.2f%%)" %(len_x,coverage1))
  print("# %s bp covered on sequence 2 (coverage %.2f%%)" %(len_y,coverage2))
  

lengthSeq1 = sys.argv[1]
lengthSeq2 = sys.argv[2]
path = sys.argv[3]
try:
  file = open(path, 'r')
except IOError as err:
  sys.stderr.write("ERROR: Could not open %s: %s" %(path, err))
  sys.exit(1)
lines = file.readlines()
parse(lines)
file.close()