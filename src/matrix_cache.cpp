#include <algorithm>
#include <omp.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <random>

#include <Eigen/Dense>

#include "matrix_cache.hpp"
#include "probability.hpp"

std::ostream& operator<<(std::ostream& ost, const matrix_cache_key& k)
{
    ost << "(" << k.branch_length() << ", (" << k.bounds().first << "," << k.bounds().second << "));";
    return ost;
}

matrix_cache::matrix_cache()
{
    //constructor is currently empty as we don't have a lambda parameter
}

matrix_cache::~matrix_cache() {}

const matrix* matrix_cache::get_matrix(double branch_length, boundaries bounds) const
{
    // cout << "Matrix request " << size << "," << branch_length << "," << lambda << endl;

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

void matrix_cache::precalculate_matrices(const double sigma2, const std::set<boundaries>& boundses, const std::set<double>& branch_lengths)
{
    // build a list of required matrices
    std::vector<matrix_cache_key> keys;
    for (auto bounds : boundses)
    {
        for (double branch_length : branch_lengths)
        {
            matrix_cache_key key(bounds, branch_length);
            if (_matrix_cache.find(key) == _matrix_cache.end())
            {
                keys.push_back(key);
            }
        }
    }

    // calculate matrices in parallel
    std::vector<matrix*> matrices(keys.size());
    generate(matrices.begin(), matrices.end(), [this] { return new matrix(this->_matrix_size); });

    int s = 0;
    size_t i = 0;
    size_t num_keys = keys.size();


    for (i = 0; i < num_keys; ++i) 
    {
        for (s = 1; s < _matrix_size; s++) { 
            boundaries bounds = keys[i].bounds();
            double branch_length = keys[i].branch_length();

            matrix* m = matrices[i];
            m->set(bounds.first, bounds.second, bm_prob(bounds, branch_length, sigma2)); //I think this will work? Make sure to check though
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