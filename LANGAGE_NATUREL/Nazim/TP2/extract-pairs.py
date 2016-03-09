#!/usr/bin/env python

# Extracts target-context pairs from corpus in window of K words left/right

# Usage: cat corpus.txt | ./extract-pairs.py targets.txt K 

import sys  
import pdb

targets = []
pairs = {}

def increment_pairs(target,context):
  key = "{} {}".format(target,context)
  pair_c = pairs.get(key,0)
  pair_c = pair_c + 1
  pairs[key] = pair_c

def print_pairs(sent,target):
  for w in sent:
    increment_pairs(target,w)

for target in open(sys.argv[1]):
  targets.append(target.strip())
  
k = int(sys.argv[2])  
  
for sentence in sys.stdin:
  sentenceL = sentence.split()
  for iword, word in enumerate(sentenceL):
    if word in targets:
      print_pairs(sentenceL[iword-k:iword] + sentenceL[iword+1:iword+k+1],word)
      
for (pair,count) in pairs.items():
  print pair + " " + str(count)
