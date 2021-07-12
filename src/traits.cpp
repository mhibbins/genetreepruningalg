#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "io.hpp"
#include "traits.hpp"


traits* traits::parse_traits(std::string trait_string) {

    traits* species_traits;
    std::vector<std::string> line_vector = simple_tokenizer(trait_string);
    std::string species = line_vector[2];
    double trait = std::stod(line_vector[4]);
    species_traits->set_species(species);
    species_traits->set_trait(trait);
    species_traits->set_species_trait(species_traits->_species, species_traits->_trait);

    return species_traits;

};