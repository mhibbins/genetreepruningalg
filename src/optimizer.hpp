#include <vector>
#include <map>
#include <chrono>
#include <iosfwd>
#include <functional>
#include <deque>
#include <stdexcept>

class optimizer_scorer;

enum strategies { RangeWidely, InitialVar, Perturb, Standard, SimilarityCutoff, NLOpt, LBFGS };

struct optimizer_parameters {
    double neldermead_expansion;
    double neldermead_reflection;
    int neldermead_iterations = 300;
    strategies strategy;
    optimizer_parameters();
};

struct candidate {
    std::vector<double> values;
    double score;
    candidate(int size);
};

struct FMinSearch {

    int maxiters;
    int bymax; 
    double rho, chi, psi, sigma;
    double tolx, tolf;
    double delta, zero_delta;

    int variable_count, variable_count_plus_one;
    int iters;
    std::vector<candidate *> candidates;
    double* x_mean;
    double* x_r;
    double* x_tmp;
    int* idx;

    optimizer_scorer* scorer;
};

FMinSearch* fminsearch_new();
void fminsearch_free(FMinSearch* pfm);
FMinSearch* fminsearch_new_with_eq(optimizer_scorer* eq, int Xsize);
void fminsearch_set_equation(FMinSearch* pfm, optimizer_scorer* eq, int Xsize);
candidate *get_best_result(FMinSearch* pfm);
void __fminsearch_sort(FMinSearch* pfm);
void** calloc_2dim(int row, int col, int size);
void free_2dim(void** data, int row, int col);
int __fminsearch_checkV(FMinSearch* pfm);
int __fminsearch_checkF(FMinSearch* pfm);
void __fminsearch_min_init(FMinSearch* pfm, double* X0);
void __fminsearch_x_mean(FMinSearch* pfm);
double __fminsearch_x_reflection(FMinSearch* pfm);
double __fminsearch_x_expansion(FMinSearch* pfm);
double __fminsearch_x_contract_outside(FMinSearch* pfm);
double __fminsearch_x_contract_inside(FMinSearch* pfm);
void __fminsearch_x_shrink(FMinSearch* pfm);
void __fminsearch_set_last_element(FMinSearch* pfm, double* x, double f);
bool threshold_achieved(FMinSearch* pfm);
int fminsearch_min(FMinSearch* pfm, double* X0, std::function<bool(FMinSearch*)> threshold_func = threshold_achieved);

class optimizer {
    FMinSearch* pfm;
    optimizer_scorer *_p_scorer;
public:
    optimizer(optimizer_scorer *scorer);
    ~optimizer();

    struct result {
        std::vector<double> values;
        double score;
        int num_iterations;
        std::chrono::seconds duration;
    };

    result optimize(const optimizer_parameters& params);

    std::vector<double> get_intial_guesses();

    OptimizerStrategy* get_strategy(const optimizer_parameters& params); //not sure where this class comes from
};