#!/bin/bash
../gvizify $1.xml > $1.dot
dot -Tpdf $1.dot > $1.pdf