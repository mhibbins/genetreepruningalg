#include <random>

#include "io.hpp"
#include "user_data.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"
#include "likelihood.hpp"
#include "optimizer_scorer.hpp"

extern std::mt19937 randomizer_engine;

double inference_optimizer_scorer::calculate_score(const double *values) {

    prepare_calculation(values);

    double score = infer_trait_likelihood(_sp_tree, _genetrees, _traits, *_p_sigma, _genetree_freqs);

    if (std::isnan(score)) score = -log(0);

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

    _species_variance = std::accumulate(traitvals.begin(), traitvals.end(), )
    }
