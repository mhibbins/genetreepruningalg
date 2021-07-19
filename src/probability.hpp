#include "clade.hpp"
#include "traits.hpp"

double bm_prob(double x, double x_0, double t, double sigma_2); 
std::vector<double> node_prob(std::vector<double> x_vector, std::vector<double> x_0_vector, double t, double sigma_2);