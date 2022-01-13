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

    clade* test_clade = new clade("sp1sp2", 0); //test_clade with two taxa
    clade* sp1 = new clade("sp1", 0.6);
    clade* sp2 = new clade("sp2", 0.6); 
    test_clade->add_descendant(sp1);
    test_clade->add_descendant(sp2);
    std::vector<clade*> test_clade_v {test_clade};
    //std::cout << "Before time slicing:" << std::endl; 
    //std::vector<int> genetree_nodes_preslice = count_nodes_all_trees(test_clade_v);
    //for (int i = 0; i < genetree_nodes_preslice.size(); i++) {std::cout << genetree_nodes_preslice[i] << " ";}
    //print_parent_daughter_nodes(test_clade);
    //std::cout << std::endl;

    clade slice_test;
    slice_test.insert_between_all_trees(sptree, test_clade_v);
    
    //std::cout << "After time slicing:" << std::endl;
    //std::vector<int> genetree_nodes_postslice = count_nodes_all_trees(test_clade_v);
    //for (int i = 0; i < genetree_nodes_postslice.size(); i++) {std::cout << genetree_nodes_postslice[i] << " ";}
    //print_parent_daughter_nodes(test_clade);


    //inference test

    //std::unique_ptr<inference_optimizer_scorer> test_scorer(new sigma_optimizer_scorer(&sigma2_initial, sptree, genetrees, species_traits, genetreefreqs));
    //optimizer test_opt(test_scorer.get());
    //auto test_result = test_opt.optimize(user_input.optimizer_params); 
    //test_scorer->finalize(&test_result.values[0]);

    return 0;
}
