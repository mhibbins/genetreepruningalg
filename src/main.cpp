#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include <getopt.h>

#include "user_data.hpp"
#include "io.hpp"
#include "clade.hpp"
#include "traits.hpp"

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
    
    input_parameters user_input = read_arguments(argc, argv);
    std::ifstream input_file(user_input.input_file_path);
    
    if (user_input.help == true) {
        std::cout << "No input file specified" << std::endl;
        return 0;
    }
    user_data data;
    data.read_datafile(user_input);
    auto [sptree, genetree, species_traits] = read_data(input_file);

    /*Print statements for debugging
    std::string sptree_string = clade_to_string(sptree);
    std::string genetree_string = clade_to_string(genetree);
    //std::cout << "If you see this, the code runs\n";
    //std::cout << sptree;
    std::cout << sptree_string << std::endl << genetree_string << std::endl;*/
    std::cout << species_traits.size();

    return 0;
}
