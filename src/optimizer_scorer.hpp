#include "io.hpp"
#include "user_data.hpp"
#include "clade.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"
#include "probability.hpp"
#include "likelihood.hpp"

class optimizer_scorer {
public: 
    virtual ~optimizer_scorer() {}

    virtual std::vector<double> initial_guesses() = 0;

    virtual double calculate_score(const double *values) = 0;
};


class inference_optimizer_scorer : public optimizer_scorer {
protected:
    virtual void prepare_calculation(const double *values) = 0;
    virtual void report_precalculation() = 0;

    double* _p_sigma;
    clade* _sp_tree;
    std::vector<clade*> _genetrees;
    std::vector<trait> _traits;

public: 
    inference_optimizer_scorer(double* p_sigma_2, clade* sp_tree, std::vector<clade*> genetrees, std::vector<trait> traits) : 
        _p_sigma(p_sigma_2),
        _sp_tree(sp_tree),
        _genetrees(genetrees), 
        _traits(traits) {}

    virtual ~inference_optimizer_scorer() {}

    double calculate_score(const double *values);

    virtual void finalize(double *result) = 0;

};

class sigma_optimizer_scorer : public inference_optimizer_scorer {

    double _tree_length; //might have to change this for gene trees
    double _species_variance;

public: 

    sigma_optimizer_scorer(double* p_sigma_2, clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits,
                           double tree_length, double species_variance) :
        inference_optimizer_scorer(p_sigma_2, sptree, genetrees, traits),
        _tree_length(tree_length), 
        _species_variance(species_variance) {}

    sigma_optimizer_scorer(double* p_sigma_2, clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits);
    //why is this declared twice? 

    std::vector<double> initial_guesses() override;

    virtual void finalize(double *results) override;

    virtual void prepare_calculation(const double *values) override;
    virtual void report_precalculation() override;

};