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

    std::cout << "Before time slicing:" << std::endl;
    std::vector<int> genetree_nodes_preslice = count_nodes_all_trees(genetrees);
    print_parent_daughter_nodes(genetrees[0]);

    //for (int i = 0; i < genetree_nodes_preslice.size(); i++) {std::cout << genetree_nodes_preslice[i] << " ";}
    std::cout << std::endl;
    clade slice_test;
    slice_test.insert_between_all_trees(sptree, genetrees);
    
    std::cout << "After time slicing:" << std::endl;
    //std::vector<int> genetree_nodes_postslice = count_nodes_all_trees(genetrees);
    //for (int i = 0; i < genetree_nodes_postslice.size(); i++) {std::cout << genetree_nodes_postslice[i] << " ";}

    print_parent_daughter_nodes(genetrees[0]);
    //inference test

    //std::unique_ptr<inference_optimizer_scorer> test_scorer(new sigma_optimizer_scorer(&sigma2_initial, sptree, genetrees, species_traits, genetreefreqs));
    //optimizer test_opt(test_scorer.get());
    //auto test_result = test_opt.optimize(user_input.optimizer_params); 
    //test_scorer->finalize(&test_result.values[0]);

    return 0;
}
