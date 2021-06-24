#include <iostream>
#include <vector>
#include <string>

#include <getopt.h>

#include "user_data.hpp"
#include "io.hpp"

input_parameters read_arguments(int argc, char *const argv[])
{
    input_parameters my_input_parameters;
    if (argc == 1)
    {
        my_input_parameters.help = true;
        return my_input_parameters;
    }

    int args; // getopt_long returns int or char
    int prev_arg;

    switch (args) {

        case 'i':
            my_input_parameters.input_file_path = optarg;
            break;
        //default: // '?' is parsed (
        //    throw std::runtime_error(std::string("Unrecognized parameter: '") + (char)args + "'");
            
        }

    //if (optind < argc)
    //{
    //    throw std::runtime_error(std::string("Unrecognized parameter: '") + argv[optind] + "'");
    //}

    //my_input_parameters.check_input(); // seeing if options are not mutually exclusive              

    return my_input_parameters;
}


int main(int argc, char *const argv[]){ 
    
    input_parameters user_input = read_arguments(argc, argv);
    user_data data;
    data.read_datafile(user_input);
    //Need to figure out input to read_data
    //clade* sptree = read_data(user_input);
    
    //std::cout << "If you see this, the code runs\n";

    return 0;
}
