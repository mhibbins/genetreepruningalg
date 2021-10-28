#!/bin/sh

#Compile pruning alg software
echo "Compiling pruning algorithm software..."
g++ -w -g -std=c++17 ../../../src/*.cpp -o ../../../src/test.exe


echo "Begin analyses..."
for i in {1..100}
do
	#Generate simulated traits and create input file 
	Rscript simulate_test_traits_genetrees.R 2>/dev/null | python3 make_pruningalg_input.py test_input_$i.txt

	#Run inference and parse out the results 
	echo "Doing species tree inference..."
	../../../src/test.exe test_input_$i.txt sptree | python3 parse_pruningalg_output.py sptree $i >> singletrait_sigma10_test_results.txt
	echo "Doing gene tree inference..."
	../../../src/test.exe test_input_$i.txt genetrees | python3 parse_pruningalg_output.py genetrees $i >> singletrait_sigma10_test_results.txt
	rm test_input_$i.txt

	echo "Finished replicate $i out of 100..."
done
