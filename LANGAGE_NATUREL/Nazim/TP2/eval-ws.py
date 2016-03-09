#!/usr/bin/env python

# Adds a column to WS datasets with system prediction

# Usage: ./eval-ws.py humanjudgnements.tsv similarities.tsv 

import sys  
import pdb

judgments = {}

judgementfile = open(sys.argv[1])
headerjudge = judgementfile.readline().strip()
for judgment in judgementfile:
  #print judgment
  w1w2,score = judgment.strip().split("\t")
  judgments[w1w2] = "0.0"
  
simfile =  open(sys.argv[2])
header = simfile.readline().strip()	 # ignore header  
  
for sim in simfile:
	w1,id1,w2,id2,score = sim.strip().split("\t")
	key = w1+","+w2
	if key in judgments.keys():
		judgments[key] = score
  
print "Word1Word2\tsystem"      
for (pair,score) in judgments.items():
  print pair + "\t" + score
