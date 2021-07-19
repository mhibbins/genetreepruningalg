#define _USE_MATH_DEFINES

#include <cmath>
#include "clade.hpp"
#include "io.hpp"

double bm_prob(double x, double x_0, double t, double sigma_2) {

    double result = 0.0; 

    double part1 = 1/(2*M_PI*t*sqrt(sigma_2));
    double part2 = -1*((pow(x-x_0, 2.0))/(2*sigma_2*t));

    result = part1*exp(part2);

    return result;

}

std::vector<double> node_prob(std::vector<double> x_vector, std::vector<double> x_0_vector, double t, double sigma_2) { 

    std::vector<double> result_vector; 

    for (int i=0; i < x_vector.size(); i++){
        result_vector.push_back(bm_prob(x_vector[i], x_0_vector[i], t, sigma_2));
    }

    return result_vector;

}