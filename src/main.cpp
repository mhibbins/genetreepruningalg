#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include <getopt.h>

#include "io.hpp"
#include "user_data.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"

extern int discretization_range;

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

    auto [sptree, genetrees, species_traits] = read_data(input_file); //parses input file 
    boundaries test_bounds = bounds(species_traits); //get upper and lower bounds of trait vector
    std::set<double> test_branch_intervals = get_branch_intervals(sptree, genetrees); //get branch length intervals for matrix cache 
    matrix_cache cache(discretization_range); //inititialize matrix cache
    cache.precalculate_matrices(1, test_bounds, test_branch_intervals); //fill matrix cache 

    //std::vector<double> test_ancestral_probs = inference_prune(species_traits, cache, sptree);

    /*
    for (int i = 0; i < test_ancestral_probs.size(); i++) {
        std::cout << test_ancestral_probs[i] << " ";
    }*/

    return 0;
}
