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

    auto [sptree, genetrees, species_traits] = read_data(input_file); //parses input file 
    std::set<double> sp_times = sptree->get_speciation_times(); //gets species tree branch lengths
    std::pair<double, double> test_bounds = bounds(species_traits); //get upper and lower bounds of trait vector

    std::vector<double> test_state_vector = state_vector(100, test_bounds); //get vector of possible character states to estimate probs over
    std::set<std::pair<double, double>> test_trait_intervals = get_all_bounds(test_state_vector); //get trait intervals to fill matrix cache 
    std::set<double> test_branch_intervals = get_branch_intervals(sptree, genetrees); //get branch length intervals for matrix cache 

    std::set<double>::iterator it = test_branch_intervals.begin();

        while (it != test_branch_intervals.end()) {
            std::cout << *it << " ";
            it++;
        }


    return 0;
}
