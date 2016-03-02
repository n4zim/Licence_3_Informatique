cd bin/mwetoolkit
make
cd ../..
cd bin/treetagger
sh install-tagger.sh
sed 's@^\$TOKENIZER.*$@sed "s/ /\\n/g" $*\|@g' cmd/tree-tagger-french-utf8 > tmp; mv tmp cmd/tree-tagger-french-utf8
chmod a+x cmd/tree-tagger-french-utf8
cd ../..
