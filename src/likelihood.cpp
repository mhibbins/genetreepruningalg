#include <math.h>

#include "io.hpp"
#include "user_data.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"

double get_likelihood (std::vector<double> root_probs) { 

    double likelihood = 0;
    double prior = 1/(root_probs.size());

    for (int i = 0; i < root_probs.size(); i++) {
        likelihood += root_probs[i]*prior;
    }

    double log_likelihood = log(likelihood);

    return log_likelihood;
}