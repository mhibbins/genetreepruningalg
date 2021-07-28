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
    std::cout << species_traits[3]->get_species();

    //std::pair<double, double> test_bounds = get_trait_range(species_traits);
    //std::cout << test_bounds.first << ", " << test_bounds.second << std::endl;

    return 0;
}
