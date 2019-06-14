#!/bin/bash

bench=./Benchmark
filename=disconnected.txt
F_IN=$bench/$filename

make clean
make

# Only run a specified file in Benchmark
#time ./CPP.exe ${F_IN}

# Sequentially run all file in Benchmark
for file in ./Benchmark/*
	do
	if test -f $file
	then
		arr=(${arr[*]} $file)
	fi
	done

for ((i=0;i< ${#arr[@]};i++))
do
	time ./CPP.exe ${arr[$i]}
	echo "===================================="
done
