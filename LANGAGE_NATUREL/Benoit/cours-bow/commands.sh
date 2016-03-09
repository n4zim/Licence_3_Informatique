#!/bin/bash

# Commands for minimantics tutorial - distributional semantics   
   
# Step 0: install minimantics

cd minimantics/src
make
cd ../..
   
# Step 1: select targets   
   
cut -f1 wordsim353.tsv | 
tail -n +2 |      # remove header
sed 's/,/\n/g' |  # 1 word per line
sed 's/./\L&/g' | # lowercase
sort |            # remove duplicates
uniq > targets.txt

# Step 2: extract target-context pairs

zcat corpus/en.gz | 
./extract-pairs.py targets.txt 4 | # takes a while...
gzip > targets-contexts.gz

# Step 3: filter contexts

zcat targets-contexts.gz |
grep -v " 1$" |                # remove hapax
sort -t " " -k 1,1 -k 3,3nr |  # TAB separator
awk -f thresh-fk.awk THRESHOLD=500 |
cat > targets-contexts-f.txt 

# Step 4: measure association strength

minimantics/src/build_profiles targets-contexts-f.txt | 
cat > profiles-filter500.txt

# Step 5: calculate similarities
minimantics/src/calculate_similarity -T 8 -a pmi -s cosine -t targets.txt -n targets.txt profiles-filter500.txt > targets-sim.txt
./eval-ws.py wordsim353.tsv targets-sim.txt > wordsim353-system.tsv

# Step 6: evaluation
./minimantics/scripts/csv_evalrank.py --gold-threshold=5 wordsim353.tsv wordsim353-system.tsv


