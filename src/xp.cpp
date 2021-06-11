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

    while (prev_arg = optind, (args = getopt_long(argc, argv, "c:v:i:e::o:t:y:n:f:E:R:L:P:I:l:m:k:a:g:s::p::zbh", longopts, NULL)) != -1) {
        if (optind == prev_arg + 2 && optarg && *optarg == '-') {
            std::cout << "You specified option " << argv[prev_arg] << " but it requires an argument. Exiting..." << "\n";
            exit(EXIT_FAILURE);
        }

        switch (args) {

        case 'i':
            my_input_parameters.input_file_path = optarg;
            break;
        default: // '?' is parsed (
            throw std::runtime_error(std::string("Unrecognized parameter: '") + (char)args + "'");
            
        }
    }

    if (optind < argc)
    {
        throw std::runtime_error(std::string("Unrecognized parameter: '") + argv[optind] + "'");
    }

    //my_input_parameters.check_input(); // seeing if options are not mutually exclusive              

    return my_input_parameters;
}