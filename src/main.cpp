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
    std::pair<double, double> test_bounds = bounds(species_traits); //get upper and lower bounds of trait vector

    std::vector<double> test_state_vector = state_vector(100, test_bounds); //get vector of possible character states to estimate probs over
    std::set<std::pair<double, double>> test_intervals = get_all_bounds(test_state_vector);

    
    /*
    for(auto it = test_intervals.begin(); it != test_intervals.end(); ++it) {
        std::cout << " " << it.first;
    } */

    return 0;
}
