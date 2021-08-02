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
    std::set<double> sp_times = sptree->get_speciation_times(); //gets species tree branch lengthsstd:;se

    std::set<std::pair<double, double>> trait_bounds; //set of bounds for each trait
    std::pair<double, double> test_bounds = bounds(species_traits); //get upper and lower bounds of trait vector
    trait_bounds.insert(test_bounds);

    //std::vector<double> test_state_vector = state_vector(100, test_bounds); //get vector of possible character states to estimate probs over
    //std::set<std::pair<double, double>> test_trait_intervals = get_all_bounds(test_state_vector); //get trait intervals to fill matrix cache 
    std::set<double> test_branch_intervals = get_branch_intervals(sptree, genetrees); //get branch length intervals for matrix cache 
    matrix_cache calc; //inititialize matrix cache
    calc.precalculate_matrices(trait_bounds, test_branch_intervals); //fill matrix cache 

    //Get a test matrix
    std::pair<double, double> trait_interval_test = *trait_bounds.begin();
    double branch_length_test = *test_branch_intervals.begin();
    const Eigen::MatrixXd& test_matrix = calc.get_matrix(branch_length_test, trait_interval_test);
    std::cout << test_matrix << std::endl;

    return 0;
}
