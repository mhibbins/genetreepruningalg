#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <random>
#include <memory>

#include <getopt.h>

#include "io.hpp"
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


int main(int argc, char *const argv[]){ 
    

    auto [user_input, sptree, genetrees, genetreefreqs, species_traits] = read_data(argc, argv); //parses input file 
    //std::vector<double> genetree_freqs {0.451, 0.183, 0.183, 0.183};
    double sigma2_initial = 0;

    //inference test

    std::unique_ptr<inference_optimizer_scorer> test_scorer(new sigma_optimizer_scorer(&sigma2_initial, sptree, genetrees, species_traits, genetreefreqs));
    optimizer test_opt(test_scorer.get());
    auto test_result = test_opt.optimize(user_input.optimizer_params); 
    test_scorer->finalize(&test_result.values[0]);

    return 0;
}
