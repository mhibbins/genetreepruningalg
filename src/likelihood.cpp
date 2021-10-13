#include <math.h>

#include "io.hpp"
#include "user_data.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"

extern int discretization_range;

double get_likelihood (std::vector<double> root_probs) { 

    double likelihood = *std::max_element(root_probs.begin(), root_probs.end());

    double log_likelihood = log(likelihood);

    return log_likelihood;
}

double infer_trait_likelihood(clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits, double sigma_2,
                             std::vector<double> genetree_freqs) {

    boundaries test_bounds = bounds(traits); //get upper and lower bounds of trait vector
    std::set<double> test_branch_intervals = get_branch_intervals(sptree, genetrees); //get branch length intervals for matrix cache 
    matrix_cache cache(discretization_range); //inititialize matrix cache
    cache.precalculate_matrices(sigma_2, test_bounds, test_branch_intervals); //fill matrix cache 
    std::vector<double> dis_vals = get_discretized_traits(test_bounds);
    std::vector<double> test_genetree_probs = inference_prune_genetrees(traits, cache, genetrees, genetree_freqs);
    double test_likelihood = get_likelihood(test_genetree_probs);

    return test_likelihood;
}