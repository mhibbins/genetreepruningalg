#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "io.hpp"
#include "traits.hpp"

traits::traits(std::string new_species, double new_trait) {
    _species = new_species;
    _trait = new_trait;
    _trait_map[new_species] = new_trait;
}

double traits::print_trait_val() const {return _trait;}
    
int traits::print_trait_map_size() const {return _trait_map.size();}

traits* parse_traits(std::string trait_string) {

    std::vector<std::string> line_vector = simple_tokenizer(trait_string);
    std::string species = line_vector[2];
    double trait = std::stod(line_vector[4]);
    traits traits_object(species, trait);
    traits* species_traits = &traits_object;

    return species_traits;

};
