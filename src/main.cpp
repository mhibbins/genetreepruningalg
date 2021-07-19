#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include <getopt.h>

#include "user_data.hpp"
#include "io.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "probability.hpp"

input_parameters read_arguments(int argc, char *const argv[])
{
    input_parameters my_input_parameters;
    if (argc == 1)
    {
        my_input_parameters.help = true;
        return my_input_parameters;
    }
    my_input_parameters.input_file_path = argv[1];
    return my_input_parameters;
}


int main(int argc, char *const argv[]){ 
    
    input_parameters user_input = read_arguments(argc, argv); //reads command line arguments
    std::ifstream input_file(user_input.input_file_path); //gets input file argument
    
    if (user_input.help == true) { //checks if input file is specified 
        std::cout << "No input file specified" << std::endl;
        return 0;
    }

    auto [sptree, genetree, species_traits] = read_data(input_file); //parses input file 
    std::set<double> sp_times = sptree->get_speciation_times(); //gets species tree branch lengths 

    std::vector<double> x_test_vector = {-2, 1, 0, 1, 2};
    std::vector<double> x_0_test_vector = {0, 0, 0, 0, 0};

    std::vector<double> test_probs = node_prob(x_test_vector, x_0_test_vector, 1, 1);

    for (int i=0; i < test_probs.size(); i++) {
        std::cout << test_probs.at(i) << std::endl;
    }

    return 0;
}
