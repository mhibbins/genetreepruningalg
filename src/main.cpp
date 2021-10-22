#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <random>
#include <memory>

#include <getopt.h>

#include "io.hpp"
#include "user_data.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"
#include "likelihood.hpp"
#include "optimizer_scorer.hpp"
#include "optimizer.hpp"

extern int discretization_range;
std::random_device rd;
std::mt19937 randomizer_engine(rd());

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
    std::vector<double> genetree_freqs {0.451, 0.183, 0.183, 0.18};

    double sigma2_guess = 0;

    std::unique_ptr<inference_optimizer_scorer> test_scorer(new sigma_optimizer_scorer(&sigma2_guess, sptree, genetrees, species_traits, genetree_freqs));
    optimizer test_opt(test_scorer.get());
    auto test_result = test_opt.optimize(user_input.optimizer_params); 
    test_scorer->finalize(&test_result.values[0]);
    
    
    //double test_likelihood = infer_trait_likelihood(sptree, genetrees, species_traits, 0.5, genetree_freqs);

    //std::cout << test_likelihood << std::endl;



    return 0;
}
