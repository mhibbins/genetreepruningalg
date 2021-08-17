#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"

double bm_prob(std::pair<double, double> boundses, double t, double sigma_2); 

std::vector<double> node_prob(std::vector<double> x_vector, std::vector<double> x_0_vector, double t, double sigma_2);

std::pair<double, double> bounds(std::vector<trait> t_range);

std::set<boundaries> get_discretized_traits(std::pair<double, double> bounds);

std::vector<double> pos_bounds(double traitval, int dis_size, boundaries bounds);

void compute_node_probability(const clade* node, const trait trait, const double sigma2, const matrix_cache& cache);

std::vector<double> inference_prune(const std::vector<trait> t, const matrix_cache& cache, const double sigma_2, const clade* p_tree);