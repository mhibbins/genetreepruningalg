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
    cache.precalculate_matrices(10, test_bounds, test_branch_intervals); //fill matrix cache 

    /*
    double branch_length_test = *test_branch_intervals.begin();
    const matrix* test_matrix = cache.get_matrix(branch_length_test, test_bounds);
    double test_val = test_matrix->get(0,0);
    std::cout << test_val << std::endl;*/

    //Running pruning alg test 

    //pos_bounds test

    /*
    double test_trait = get_species_trait("1", species_traits);

    std::vector<double> pos_test_vector = pos_bounds(test_trait, discretization_range, test_bounds);

    for (int i = 0; i <= pos_test_vector.size(); i++) {
        std::cout << pos_test_vector[i] << " ";
    }*/

    std::vector<double> test_ancestral_probs = inference_prune(species_traits, cache, sptree);

    for (int i = 0; i <= test_ancestral_probs.size(); i++) {
        std::cout << test_ancestral_probs[i] << " ";
    }

    //Get matrix cache keys for debugging 
    /*
    std::map<double, boundaries> test_keys = cache.get_cache_keys();

    for (auto const& x : test_keys) {
        boundaries bounds = x.second;
        std::cout << x.first << " : " << bounds.first << " " << bounds.second << std::endl; //problem here is that the bounds are a pair
    }*/

    //Get matrix sizes for debugging

    /*
    std::vector<double> matrix_sizes = cache.get_cache_sizes();

    for (int i = 0; i <= matrix_sizes.size(); i++) {
        std::cout << matrix_sizes[i] << " ";
    }*/


    return 0;
}
