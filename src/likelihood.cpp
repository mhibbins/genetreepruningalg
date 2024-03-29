#include <math.h>

#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"

extern int discretization_range;
extern std::string TREE_STRUCT;

double get_likelihood (std::vector<double> root_probs) { 

    double likelihood = *std::max_element(root_probs.begin(), root_probs.end());

    double log_likelihood = -log(likelihood);

    return log_likelihood;
}


double infer_trait_likelihood_genetrees(clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits, double sigma_2,
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

double infer_trait_likelihood_sptree(clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits, double sigma_2,
                             std::vector<double> genetree_freqs) {

    boundaries test_bounds = bounds(traits); //get upper and lower bounds of trait vector
    std::set<double> test_branch_intervals = get_branch_intervals(sptree, genetrees); //get branch length intervals for matrix cache 
    matrix_cache cache(discretization_range); //inititialize matrix cache
    cache.precalculate_matrices(sigma_2, test_bounds, test_branch_intervals); //fill matrix cache 
    std::vector<double> dis_vals = get_discretized_traits(test_bounds);
    std::vector<double> test_genetree_probs = inference_prune(traits, cache, sptree);
    double test_likelihood = get_likelihood(test_genetree_probs);

    return test_likelihood;
}

double infer_trait_likelihood(clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits, double sigma_2,
                             std::vector<double> genetree_freqs) { 
    
    double likelihood;

    if (TREE_STRUCT == "sptree") {
        likelihood = infer_trait_likelihood_sptree(sptree, genetrees, traits, sigma_2, genetree_freqs);
    }
    else if (TREE_STRUCT == "genetrees") {
        likelihood = infer_trait_likelihood_genetrees(sptree, genetrees, traits, sigma_2, genetree_freqs);
    }

    return likelihood;
    
}

std::pair<std::vector<double>, std::vector<double>> get_likelihood_surface(clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits,
                                            std::pair<double, double> sigma2_range, std::vector<double> genetree_freqs) {
    
    std::pair<std::vector<double>, std::vector<double>> surface;
    std::vector<double> sigma2_vector;
    double step = sigma2_range.first;
    float range = sigma2_range.second - sigma2_range.first;
    float stepsize = range/100;

    while(step < sigma2_range.second) {
        sigma2_vector.push_back(step + stepsize);
        step += stepsize;
    }

    surface.first = sigma2_vector;

    std::vector<double> likelihoods;

    if (TREE_STRUCT == "sptree") {
        for (int i = 0; i < sigma2_vector.size(); i++) {
            likelihoods.push_back(infer_trait_likelihood_sptree(sptree, genetrees, traits, sigma2_vector[i], genetree_freqs));
        }
    }
    else if (TREE_STRUCT == "genetrees") {
        for (int i = 0; i < sigma2_vector.size(); i++) {
            likelihoods.push_back(infer_trait_likelihood_genetrees(sptree, genetrees, traits, sigma2_vector[i], genetree_freqs));
        }
    }

    surface.second = likelihoods;

    return surface;
}
