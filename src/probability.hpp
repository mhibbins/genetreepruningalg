#include "clade.hpp"
#include "traits.hpp"

double bm_prob(std::pair<double, double> boundses, double t, double sigma_2); 

std::vector<double> node_prob(std::vector<double> x_vector, std::vector<double> x_0_vector, double t, double sigma_2);

std::pair<double, double> bounds(std::vector<traits> t_range);

std::vector<double> state_vector(int size, std::pair<double, double> bounds);

std::set<std::pair<double, double>> get_all_bounds(const std::vector<double> states);