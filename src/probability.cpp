#define _USE_MATH_DEFINES

#include <cmath>
#include "clade.hpp"
#include "io.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"

int discretization_range = 50;

double bm_prob(double val1, double val2, double t, double sigma_2) {

    double result = 0.0; 

    double part1 = 1/(2*M_PI*t*sqrt(sigma_2));
    double part2 = -1*((pow(val2-val1, 2.0))/(2*sigma_2*t));

    result = part1*exp(part2);

    return result;

}

boundaries bounds(std::vector<trait> t_range) {

    std::pair<double, double> dis_bounds (0, 0);
    std::pair<double, double> trait_range = get_trait_range(t_range);
    dis_bounds.first = 0.5*trait_range.first;
    dis_bounds.second = 1.5*trait_range.second;

    return dis_bounds;

}

std::vector<double> get_discretized_traits(std::pair<double, double> bounds) {

    std::vector<double> dis_trait_vector;
    double step = bounds.first;
    float range = bounds.second - bounds.first;
    float stepsize = range/discretization_range;

    while(step < bounds.second) {
        dis_trait_vector.push_back(step + stepsize);
        step += stepsize;
    }


    return dis_trait_vector;
}

std::vector<double> pos_bounds(double traitval, int dis_size, boundaries bounds) {

    std::vector<double> X(dis_size, 0);

    if (traitval == bounds.first) {
        X[0] = 1;
    }
    else if (traitval == bounds.second) 
    {
        X[dis_size - 1] = 1;
    }
    else 
    {
        double nx = (dis_size - 1) * (traitval - bounds.first) / double(bounds.second - bounds.first);
        int ix = floor(nx);
        double ux = nx - ix;
        X[ix + 1] = ux;
        X[ix] = 1 - ux;
    }

    for (int i = 0; i <= X.size(); i++) { //I think this does the same thing as the lambda expression in CAGEE's DiffMat.cpp, but not 100%
        X[i] = X[i] * (dis_size-1) / double(bounds.second - bounds.first); 
    }

    return X;
}

std::vector<double> compute_node_probability(const clade* node, const std::vector<trait> traits, 
    clademap<std::vector<double>> probabilities, const matrix_cache& cache) 

    {
    std::vector<double> nodeprobs = probabilities[node];
    std::fill(nodeprobs.begin(), nodeprobs.end(), 1);
    boundaries trait_bounds = bounds(traits);

    if (node->is_leaf()) {
        double species_trait = get_species_trait(node->get_taxon_name(), traits);
        nodeprobs = pos_bounds(species_trait, discretization_range, trait_bounds);
    }
    else {
        for (auto it = node->descendant_begin(); it != node->descendant_end(); ++it) {

            const matrix* m = cache.get_matrix((*it)->get_branch_length(), trait_bounds);

            std::vector<double> result = matrix_multiply(m, probabilities[*it], discretization_range, bounds(traits));
            
            for (int i = 0; i < nodeprobs.size(); i++) {
                nodeprobs[i] *= result[i];
            }
        }
    }

    return nodeprobs;
}

std::vector<double> inference_prune(std::vector<trait> t, const matrix_cache& cache, const clade* p_tree) {

    /*Does the pruning alg calc over all nodes of the specified tree for a given input trait. Returns the vector of 
    probabilities for character states at the root of the tree. Currently doesn't have a multiplier for sigma^2, might
    have to work that in later*/ 

    clademap<std::vector<double>> probabilities;

    auto init_func = [&](const clade* node) { probabilities[node] = std::vector<double> (discretization_range, 0); };
    std::for_each(p_tree->reverse_level_begin(), p_tree->reverse_level_end(), init_func);

    auto compute_func = [&](const clade* c) { probabilities[c] = compute_node_probability(c, t, probabilities, cache); };
    std::for_each(p_tree->reverse_level_begin(), p_tree->reverse_level_end(), compute_func);

    return probabilities.at(p_tree);
}

std::vector<double> inference_prune_genetrees(std::vector<trait> t, const matrix_cache& cache, const std::vector<clade*> p_trees) {

    /*Does the pruning alg calc over all nodes of the specified tree for a given input trait. Returns the vector of 
    probabilities for character states at the root of the tree. Currently doesn't have a multiplier for sigma^2, might
    have to work that in later*/ 

    std::vector<double> summed_root_probs(discretization_range, 0);
    std::vector<double> genetree_freqs {0.451, 0.183, 0.183, 0.18}; //need to handle freqs in a general way later on

    for (int i = 0; i < p_trees.size(); i++) {

        clademap<std::vector<double>> probabilities;
        clade* p_tree = p_trees[i];

        auto init_func = [&](const clade* node) { probabilities[node] = std::vector<double> (discretization_range, 0); };
        std::for_each(p_tree->reverse_level_begin(), p_tree->reverse_level_end(), init_func);

        auto compute_func = [&](const clade* c) { probabilities[c] = compute_node_probability(c, t, probabilities, cache); };
        std::for_each(p_tree->reverse_level_begin(), p_tree->reverse_level_end(), compute_func);

        std::vector<double> rootprobs = probabilities.at(p_tree);
        std::transform(rootprobs.begin(), rootprobs.end(), rootprobs.begin(), [&k = genetree_freqs[i]](auto &c) { return c*k; });

        for (int j = 0; j < summed_root_probs.size(); j++) {
            summed_root_probs[j] += rootprobs[j];
        }
    }

    return summed_root_probs;
}