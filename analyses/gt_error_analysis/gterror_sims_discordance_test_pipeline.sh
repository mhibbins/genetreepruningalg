#!/bin/sh

#Compile pruning alg software
echo "Compiling pruning algorithm software..."
g++ -w -g -std=c++17 ../../src/*.cpp -o ../../src/test.exe


echo "Begin analyses..."
echo "Doing simulations..."
Rscript simulators/gterror_sim.R 2>/dev/null | tee >(python3 input_makers/make_gt_error_sim_condition1_input.py condition1_test_input_) >(python3 input_makers/make_gt_error_sim_condition2_input.py condition2_test_input) >(python3 input_makers/make_gt_error_sim_condition3_input.py condition3_test_input_) >(python3 input_makers/make_gt_error_sim_condition4_input.py condition4_test_input_) >(python3 input_makers/make_gt_error_sim_condition5_input.py condition5_test_input_) >(python3 input_makers/make_gt_error_sim_condition6_input.py condition6_test_input_) >(python3 input_makers/make_gt_error_sim_condition7_input.py condition7_test_input_) >(python3 input_makers/make_gt_error_sim_condition8_input.py condition8_test_input_) >(python3 input_makers/make_gt_error_sim_condition9_input.py condition9_test_input_) >(python3 input_makers/make_gt_error_sim_condition10_input.py condition10_test_input_) >(python3 input_makers/make_gt_error_sim_condition11_input.py condition11_test_input_) 
echo "Begin pruning algorithm inferences.."
for i in {1..100};
do
	echo "Doing condition 1 inference..."
	../../src/test.exe condition5_test_input_$i.txt sptree | python3 parse_discordance_test_output.py genetrees condition1 $i >> gterror_sim_discordance_test_results.txt
	echo "Doing condition 2 inference..."
	../../src/test.exe condition2_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition2 $i >> gterror_sim_discordance_test_results.txt
	rm condition1_test_input_$i.txt
	rm condition2_test_input_$i.txt

	echo "Doing condition 3 inference..."
	../../src/test.exe condition3_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition3 $i >> gterror_sim_discordance_test_results.txt
	echo "Doing condition 4 inference..."
	../../src/test.exe condition4_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition4 $i >> gterror_sim_discordance_test_results.txt
	rm condition3_test_input_$i.txt
	rm condition4_test_input_$i.txt

	echo "Doing condition 5 inference..."
	../../src/test.exe condition5_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition5 $i >> gterror_sim_discordance_test_results.txt
	echo "Doing condition 6 inference..."
	../../src/test.exe condition6_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition6 $i >> gterror_sim_discordance_test_results.txt
	rm condition5_test_input_$i.txt
	rm condition6_test_input_$i.txt

	echo "Doing condition 7 inference..."
	../../src/test.exe condition7_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition7 $i >> gterror_sim_discordance_test_results.txt
	echo "Doing condition 8 inference..."
	../../src/test.exe condition8_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition8 $i >> gterror_sim_discordance_test_results.txt
	rm condition7_test_input_$i.txt
	rm condition8_test_input_$i.txt

	echo "Doing condition 9 inference..."
	../../src/test.exe condition9_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition9 $i >> gterror_sim_discordance_test_results.txt
	echo "Doing condition 10 inference..."
	../../src/test.exe condition10_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition10 $i >> gterror_sim_discordance_test_results.txt
	rm condition9_test_input_$i.txt
	rm condition10_test_input_$i.txt
	
	echo "Doing condition 11 inference..."
	../../src/test.exe condition11_test_input_$i.txt genetrees | python3 parse_discordance_test_output.py genetrees condition11 $i >> gterror_sim_disocrdance_test_results.txt
	echo "Finished replicate $i out of 100..."
	rm condition11_test_input_$i.txt
done
