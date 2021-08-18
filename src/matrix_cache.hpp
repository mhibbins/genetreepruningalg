#ifndef matrix_cache_hpp
#define matrix_cache_hpp

#include <map>
#include <vector> 
#include <set>
#include <cassert>


using boundaries = std::pair<double, double>;

class matrix
{
    std::vector<double> values;
    int _size;
public:
    matrix(int sz) : _size(sz)
    {
        values.resize(_size*_size);
    }
    void set(int x, int y, double val)
    {
        assert(x < _size);
        assert(y < _size);
        values[x*_size + y] = val;
    }
    double get(int x, int y) const
    {
        assert(x < _size);
        assert(y < _size);
        return values[x*_size + y];
    }
    int size() const {
        return _size;
    }
    bool is_zero() const;
    void multiply(const std::vector<double>& probs, int dis_range, double * result) const;

    int select_random_y(int x, int max) const;
};

class matrix_cache_key {
    std::pair<long, long> _bounds;
    long _branch_length;
public:
    matrix_cache_key(boundaries bounds, double some_branch_length) :
        _bounds(long(bounds.first * 1000000000), long(bounds.second * 1000000000)),    // keep 9 significant digits
        _branch_length(long(some_branch_length * 1000)) {} // keep 3 significant digits

    bool operator<(const matrix_cache_key& o) const {
        return std::tie(_bounds.first, _bounds.second, _branch_length) < std::tie(o._bounds.first, o._bounds.second, o._branch_length);
    }
    boundaries bounds() const {
        return boundaries(double(_bounds.first / 1000000000.0), double(_bounds.second / 1000000000.0));
    }
    double branch_length() const {
        return double(_branch_length) / 1000.0;
    }
};

//! Computation of the probabilities of moving from a family size (parent) to another (child)
/*!
Contains a map (_cache) that serves as a hash table to store precalculated values.
If the given parameters have already been calculated, will return the cached value rather than calculating the value again.
*/
class matrix_cache {
private:
    std::map<matrix_cache_key, matrix*> _matrix_cache; //!< nested map that stores transition probabilities for a given lambda and branch_length (outer), then for a given parent and child size (inner)
    double _sigma_squared;
    int _matrix_size; 
public:
    void precalculate_matrices(const double sigma2, const boundaries bounds, const std::set<double>& branch_lengths);
    const matrix* get_matrix(double branch_length, boundaries bounds) const;
    std::map<double, boundaries> get_cache_keys();

    int get_cache_size() const {
        return _matrix_cache.size();
    }

    matrix_cache();
    ~matrix_cache();

    friend std::ostream& operator<<(std::ostream& ost, matrix_cache& c);

};

#endif