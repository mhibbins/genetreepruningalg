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
    

    auto [user_input, sptree, genetrees, species_traits] = read_data(argc, argv); //parses input file 
    std::vector<double> genetree_freqs {0.451, 0.183, 0.183, 0.183};
    double sigma2_guess = 0;


    /*
    std::unique_ptr<inference_optimizer_scorer> test_scorer(new sigma_optimizer_scorer(&sigma2_guess, sptree, genetrees, species_traits, genetree_freqs));
    optimizer test_opt(test_scorer.get());
    auto test_result = test_opt.optimize(user_input.optimizer_params); 
    test_scorer->finalize(&test_result.values[0]);*/

    std::pair<double, double> sigma2_range(0, 20);

    std::pair<std::vector<double>, std::vector<double>> likelihood_surface;

    likelihood_surface = get_likelihood_surface(sptree, genetrees, species_traits, sigma2_range, genetree_freqs);

    for (int i = 0; i < likelihood_surface.first.size(); i++) {
        std::cout << likelihood_surface.first[i] << " " << likelihood_surface.second[i] << std::endl;
    }




    return 0;
}
