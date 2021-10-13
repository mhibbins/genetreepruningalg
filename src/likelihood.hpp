double get_likelihood (std::vector<double> root_probs);

double infer_trait_likelihood(clade* sptree, std::vector<clade*> genetrees, std::vector<trait> traits, double sigma_2,
                             std::vector<double> genetree_freqs);