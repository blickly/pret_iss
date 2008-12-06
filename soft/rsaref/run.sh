#! /bin/bash
if [ "$1" = "pret" ]; then
../../src/pret . -1
#echo "Entered pret"
fi
if [ "$1" = "csim" ]; then
../../scripts/csim.py .
#echo "Entered csim"
fi

