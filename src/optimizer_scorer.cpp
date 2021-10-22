#include <random>
#include <iostream>

#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"
#include "likelihood.hpp"
#include "optimizer_scorer.hpp"

extern std::mt19937 randomizer_engine;

double inference_optimizer_scorer::calculate_score(double *values) {

    prepare_calculation(values);

    report_precalculation();

    double score = infer_trait_likelihood(_sp_tree, _genetrees, _traits, *_p_sigma, _genetree_freqs);

    if (std::isnan(score)) score = -log(0);

    std::cout << score << std::endl;

    return score;
};

sigma_optimizer_scorer::sigma_optimizer_scorer(double* p_sigma_2, clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits,
                                               std::vector<double> genetree_freqs) :
    inference_optimizer_scorer(p_sigma_2, sptree, genetrees, traits, genetree_freqs) {
    
    _tree_length = sptree->distance_from_root_to_tip();
    std::vector<double> traitvals;

    for (int i = 0; i < traits.size(); i++) {traitvals.push_back(traits[i].get_trait_val());}
    double sz = traitvals.size();
    double mean = std::accumulate(traitvals.begin(), traitvals.end(), 0.0) / sz;

    double var = 0;

    for (int n = 0; n < sz; n++) {var += (traitvals[n] - mean) * (traitvals[n] - mean);}
    var /= sz-1;

    _species_variance = var;
};

double sigma_optimizer_scorer::initial_guesses() {

    double distmean = sqrt(_species_variance / _tree_length); 
    std::normal_distribution<double> distribution(distmean,0.2);
    double result(distribution(randomizer_engine));

    return result;
};

void sigma_optimizer_scorer::prepare_calculation(double *values) {
    _p_sigma = values;
};

void sigma_optimizer_scorer::report_precalculation() {
    std::cout << "Sigma^2: " << *_p_sigma << std::endl;
}

void sigma_optimizer_scorer::finalize(double *results) {
    _p_sigma = results;
}

