#!/bin/sh

#Compile pruning alg software
echo "Compiling pruning algorithm software..."
g++ -w -g -std=c++17 ../../src/*.cpp -o ../../src/test.exe


echo "Begin analyses..."
echo "Condition 1 simulations..."
Rscript simulators/sim_condition1.R 2>/dev/null | python3 input_makers/make_condition1_input.py condition1_test_input_
echo "Condition 2 simulations..."
Rscript simulators/sim_condition2.R 2>/dev/null | python3 input_makers/make_condition2_input.py condition2_test_input_
echo "Condition 3 simulations..."
Rscript simulators/sim_condition3.R 2>/dev/null | python3 input_makers/make_condition3_input.py condition3_test_input_
echo "Condition 4 simulations..."
Rscript simulators/sim_condition4.R 2>/dev/null | python3 input_makers/make_condition4_input.py condition4_test_input_
echo "Condition 5 simulations..."
Rscript simulators/sim_condition5.R 2>/dev/null | python3 input_makers/make_condition5_input.py condition5_test_input_

echo "Begin pruning algorithm inferences.."
 
for i in {1..100}
do
	echo "Doing condition 1 species tree inference..."
	../../src/test.exe condition1_test_input_$i.txt sptree | python3 parse_discordance_test_output.py sptree condition1 $i >> sigma1_discordance_test_results.txt
	echo "Doing condition 1 gene tree inference..."
	../../src/test.exe condition1_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition1 $i >> sigma1_discordance_test_results.txt
	rm condition1_test_input_$i.txt

	echo "Doing condition 2 species tree inference..."
	../../src/test.exe condition2_test_input_$i.txt sptree | python3 parse_discordance_test_output.py sptree condition2 $i >> sigma1_discordance_test_results.txt
	echo "Doing condition 2 gene tree inference..."
	../../src/test.exe condition2_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition2 $i >> sigma1_discordance_test_results.txt

	echo "Doing condition 3 species tree inference..."
	../../src/test.exe condition3_test_input_$i.txt sptree | python3 parse_discordance_test_output.py sptree condition3 $i >> sigma1_discordance_test_results.txt
	echo "Doing condition 3 gene tree inference..."
	../../src/test.exe condition3_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition3 $i >> sigma1_discordance_test_results.txt

	#Need to figure out why condition 4 gene tree inference doesn't work
	#echo "Doing condition 4 species tree inference..."
	#../../src/test.exe condition4_test_input_$i.txt sptree | python3 parse_discordance_test_output.py sptree condition4 $i >> sigma1_discordance_test_results.txt
	#echo "Doing condition 4 gene tree inference..."
	#../../src/test.exe condition4_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition4 $i >> sigma1_discordance_test_results.txt

	echo "Doing condition 5 species tree inference..."
	../../src/test.exe condition5_test_input_$i.txt sptree | python3 parse_discordance_test_output.py sptree condition5 $i >> sigma1_discordance_test_results.txt
	echo "Doing condition 5 gene tree inference..."
	../../src/test.exe condition5_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition5 $i >> sigma1_discordance_test_results.txt

	echo "Finished replicate $i out of 100..."
done
