#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "traits.hpp"
#include "io.hpp"

std::tuple<std::string, double>* parse_traits(std::string trait_string) {

    std::vector<std::string> line_vector = simple_tokenizer(trait_string);
    std::string species = line_vector[2];
    double trait = std::stod(line_vector[4]);

    return {species, trait};

};