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

    //time slicer test
    /*
    std::vector<int> genetree_nodes_preslice = count_nodes_all_trees(genetrees);
    for (int i = 0; i < genetree_nodes_preslice.size(); i++) {std::cout << genetree_nodes_preslice[i] << " ";}
    clade slice_test;
    slice_test.insert_between_all_trees(sptree, genetrees);
    std::vector<int> genetree_nodes_postslice = count_nodes_all_trees(genetrees);
    for (int i = 0; i < genetree_nodes_postslice.size(); i++) {std::cout << genetree_nodes_postslice[i] << " ";}*/

    std::unique_ptr<inference_optimizer_scorer> test_scorer(new sigma_optimizer_scorer(&sigma2_initial, sptree, genetrees, species_traits, genetreefreqs));
    optimizer test_opt(test_scorer.get());
    auto test_result = test_opt.optimize(user_input.optimizer_params); 
    test_scorer->finalize(&test_result.values[0]);

    /*
    std::pair<double, double> sigma2_range(0.01, 2);

    std::pair<std::vector<double>, std::vector<double>> likelihood_surface;

    likelihood_surface = get_likelihood_surface(sptree, genetrees, species_traits, sigma2_range, genetree_freqs);

    for (int i = 0; i < likelihood_surface.first.size(); i++) {
        std::cout << likelihood_surface.first[i] << " " << likelihood_surface.second[i] << std::endl;
    }*/

    return 0;
}
