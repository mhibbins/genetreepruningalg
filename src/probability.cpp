#define _USE_MATH_DEFINES

#include <cmath>
#include "clade.hpp"
#include "io.hpp"
#include "traits.hpp"

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

std::pair<double, double> bounds(std::vector<traits> t_range) {

    std::pair<double, double> dis_bounds (0, 0);
    std::pair<double, double> trait_range = get_trait_range(t_range);
    dis_bounds.first = 0.5*trait_range.first;
    dis_bounds.second = 1.5*trait_range.second;

    return dis_bounds;

}

std::vector<double> state_vector(int size, std::pair<double, double> bounds) {

    std::vector<double> s_vector;
    double step = bounds.first;
    float range = bounds.second - bounds.first;
    float stepsize = range/size;

    while(step < bounds.second) {
        s_vector.push_back(step);
        step = step + stepsize;
    }

    return s_vector;

}

std::set<std::pair<double, double>> get_all_bounds(const std::vector<double> states) {
    
    std::set<std::pair<double, double>> boundses;

    for (int i = 0; i < states.size(); i++) {
        std::pair<double, double> interval;
        interval.first = states[i];
        interval.second = states[i+1];
        boundses.insert(interval);
    };

    return boundses;
}