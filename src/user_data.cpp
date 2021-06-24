#include <iostream>
#include <vector>
#include <string>


#include "user_data.hpp"
#include "io.hpp"

void user_data::read_inputfile(const input_parameters &my_input_parameters) {
    //This function calls the file parsers, which are in io.cpp
    std::ifstream input_file(my_input_parameters.input_file_path);
    //clade* sptree = read_data(input_file);
}

void user_data::read_datafile(const input_parameters &my_input_parameters)
{
    /* -i */
    if (!my_input_parameters.input_file_path.empty()) {
        // Populates (pointer to) species tree, vector of gene trees, vector of trait values 
        read_inputfile(my_input_parameters);
    }

}