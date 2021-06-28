#!/usr/bin/env bash

# make neccesary file
make;

largest=0.0
smallest=1000000000000.0


while true; do
	./testcase > in;
	./final < in > out;
	current=$(./checker in out)
	
	if [[ $(echo "$current < $smallest" | bc) -eq 1 ]]; then
		cat in > final.smallest.testcase.in 
		smallest=$current
		echo "smallest" $smallest
	fi

	if [[ $(echo "$current > $largest" | bc) -eq 1 ]]; then 
		cat in > final.largest.testcase.in
		largest=$current
		echo "largest" $largest
	fi
done
