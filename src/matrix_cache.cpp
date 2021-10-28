#include <algorithm>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <random>

#include "matrix_cache.hpp"
#include "probability.hpp"

std::ostream& operator<<(std::ostream& ost, const matrix_cache_key& k)
{
    ost << "(" << k.branch_length() << ", (" << k.bounds().first << "," << k.bounds().second << "));";
    return ost;
}

matrix_cache::~matrix_cache() {}

std::vector<double> matrix_multiply(const matrix* m, const std::vector<double>& probs, int dis_range, boundaries bounds) {

    std::vector<double> result(dis_range, 0);

    std::vector<double> dis_vals = get_discretized_traits(bounds);

    for (int s = 0; s < dis_vals.size(); s++) {
        for (int c = 0; c < dis_vals.size(); c++) {
            result[s] += m->get(dis_vals[s], dis_vals[c]) * probs[c];
        }
    }
   
    return result;
};


const matrix* matrix_cache::get_matrix(double branch_length, boundaries bounds) const
{
    //std::cout << "Matrix request " << bounds << "," << branch_length << "," << lambda << endl;

    matrix_cache_key key(bounds, branch_length);
    if (_matrix_cache.find(key) != _matrix_cache.end())
    {
       return _matrix_cache.at(key);
    }
    else
    {
        std::ostringstream ost;
        ost << "Failed to find matrix for " << key;
        throw std::runtime_error(ost.str());
    }
}

std::map<double, boundaries> matrix_cache::get_cache_keys() { //debugging function to get matrix keys 

    std::map<double, boundaries> cache_keys;

    for (auto const& x : _matrix_cache) {
        matrix_cache_key keys = x.first;
        boundaries bounds = keys.bounds(); 
        double branch = keys.branch_length();
        cache_keys[branch] = bounds;
    }

    return cache_keys;
} 

std::vector<double> matrix_cache::get_cache_sizes() { //debugging function to get matrix sizes

    std::vector<double> cache_sizes;

    for (auto const& x : _matrix_cache) {
        matrix_cache_key keys = x.first;
        boundaries bounds = keys.bounds(); 
        double branch = keys.branch_length();
        const matrix* matrix_x = get_matrix(branch, bounds);
        cache_sizes.push_back(matrix_x->size());
    }

    return cache_sizes;
} 

void matrix_cache::precalculate_matrices(const double sigma2, const boundaries bounds, const std::set<double>& branch_lengths)
{
    // build a list of required matrices
    std::vector<double> dis_vals = get_discretized_traits(bounds);
    std::vector<matrix_cache_key> keys;

    for (double branch_length : branch_lengths)
    {
        matrix_cache_key key(bounds, branch_length);
        if (_matrix_cache.find(key) == _matrix_cache.end())
        {
            keys.push_back(key);
        }
    }

  
    // calculate matrices in parallel
    std::vector<matrix*> matrices(dis_vals.size()); //the matrix size is initialized up here. So I should discretize before this step
    generate(matrices.begin(), matrices.end(), [this] { return new matrix(this->_matrix_size); });

    int s = 0;
    size_t i = 0;
    size_t num_keys = keys.size();

    for (i = 0; i < num_keys; ++i) 
    {
        for (s = 0; s < _matrix_size; s++) { 

            //Need to write another loop in here to go over each discretized interval. 
            boundaries bounds = keys[i].bounds();
            double branch_length = keys[i].branch_length();

            matrix* m = matrices[i];
            m->set(0, 0, bm_prob(0, 0, branch_length, sigma2));

            for (int j = 0; j < m->size(); ++j) {
                m->set(0, j, bm_prob(0, dis_vals[j], branch_length, sigma2));
            }

            for (int c = 0; c < _matrix_size; c++) {
                m->set(s, c, bm_prob(dis_vals[s], dis_vals[c], branch_length, sigma2));
            }
        }
    }

    for (i = 0; i < num_keys; ++i)
    {
        _matrix_cache[keys[i]] = matrices[i];
    }

}

std::ostream& operator<<(std::ostream& ost, matrix_cache& c)
{
    ost << c.get_cache_size() << " matrices. Keys: ";
    for (auto& kv : c._matrix_cache)
    {
        ost << kv.first;
    }
    return ost;
}