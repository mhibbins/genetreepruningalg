#!/bin/bash/

cd ../src/
g++ -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -g -std=c++17 *.cpp -o gtpruningalg.exe

for filename in ../tomato_test/*input.txt; do
	echo "Doing $filename"
	./gtpruningalg.exe "$filename" sptree > "../tomato_test/$(basename "$filename" .txt)_sptree_output.txt"
	./gtpruningalg.exe "$filename" genetrees > "../tomato_test/$(basename "$filename" .txt)_genetrees_output.txt"
done
