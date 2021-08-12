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
#include "matrix_cache.hpp"

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
    std::set<boundaries> trait_bounds; //set of bounds for each trait
    boundaries test_bounds = bounds(species_traits); //get upper and lower bounds of trait vector
    trait_bounds.insert(test_bounds);
    std::set<double> test_branch_intervals = get_branch_intervals(sptree, genetrees); //get branch length intervals for matrix cache 
    matrix_cache cache; //inititialize matrix cache
    cache.precalculate_matrices(1, trait_bounds, test_branch_intervals); //fill matrix cache 

    //Get a test matrix
    boundaries trait_interval_test = *trait_bounds.begin();
    double branch_length_test = *test_branch_intervals.begin();
    const matrix* p_test_matrix = cache.get_matrix(branch_length_test, trait_interval_test);
    //std::cout << trait_interval_test.first << " " << trait_interval_test.second << " " << branch_length_test << std::endl;
    double test_val = p_test_matrix->get(trait_interval_test.second - trait_interval_test.first, branch_length_test);
    std::cout << test_val << std::endl;

    return 0;
}
