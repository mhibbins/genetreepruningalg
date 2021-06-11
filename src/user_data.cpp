#include <iostream>
#include <vector>
#include <string>

#include "user_data.hpp"
#include "io.hpp"

void user_data::read_speciestree(const input_parameters &my_input_parameters) {

}

void user_data::read_datafiles(const input_parameters &my_input_parameters)
{
    /* -i */
    if (!my_input_parameters.input_file_path.empty()) {
        // Populates (pointer to) vector of gene family instances, max_family_size and max_root_family_size (last two passed by reference)
        read_speciestree(my_input_parameters);
    }

}