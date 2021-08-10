#define _USE_MATH_DEFINES

#include <cmath>
#include "clade.hpp"
#include "io.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"

int discretization_range = 100;

double bm_prob(std::pair<double, double> boundses, double t, double sigma_2) {

    double result = 0.0; 

    double part1 = 1/(2*M_PI*t*sqrt(sigma_2));
    double part2 = -1*((pow(boundses.second-boundses.first, 2.0))/(2*sigma_2*t));

    result = part1*exp(part2);

    return result;

}

std::vector<double> node_prob(std::pair<std::vector<double>, std::vector<double>> v_boundses, double t, double sigma_2) { 

    std::vector<double> result_vector; 

    for (int i=0; i < v_boundses.second.size(); i++){
        std::pair<double, double> bounds (v_boundses.second[i], v_boundses.first[i]);
        result_vector.push_back(bm_prob(bounds, t, sigma_2));
    }

    return result_vector;

}

std::pair<double, double> bounds(std::vector<trait> t_range) {

    std::pair<double, double> dis_bounds (0, 0);
    std::pair<double, double> trait_range = get_trait_range(t_range);
    dis_bounds.first = 0.5*trait_range.first;
    dis_bounds.second = 1.5*trait_range.second;

    return dis_bounds;

}

std::set<boundaries> get_discretized_traits(std::pair<double, double> bounds) {

    std::set<boundaries> dis_traits;
    double step = bounds.first;
    float range = bounds.second - bounds.first;
    float stepsize = range/discretization_range;

    while(step < bounds.second) {
        boundaries bound = {step - stepsize, step + stepsize};
        dis_traits.insert(bound);
        step = step + stepsize;
    }

    return dis_traits;
}

void compute_node_probability(const clade* node, const std::vector<trait> traits, 
    std::map<const clade*, std::vector<double>> probabilities,
    const double sigma2, const matrix_cache& cache) 
{
    if (node->is_leaf()) {
        double species_trait = get_species_trait(node->get_taxon_name(), traits);
        probabilities[node][species_trait] = 1.0;
    }
    else {
        std::vector<double>& node_probs = probabilities[node];
        std::fill(node_probs.begin(), node_probs.end(), 1);

        for (auto it = node->descendant_begin(); it != node->descendant_end(); ++it) {
            double result[discretization_range];
            std::fill(result, result + discretization_range, 0);
            const matrix* m = cache.get_matrix((*it)->get_branch_length(), bounds(traits));
            m->multiply(probabilities[*it], discretization_range, result);
            for (size_t i = 0; i < node_probs.size(); i++) {
                node_probs[i] *= result[i];
            }
        }
    }
}