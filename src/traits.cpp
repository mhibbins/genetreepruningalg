#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "io.hpp"
#include "traits.hpp"


traits* parse_traits(std::string trait_string) {

    traits* species_traits;
    std::vector<std::string> line_vector = simple_tokenizer(trait_string);
    std::string species = line_vector[2];
    double trait = std::stod(line_vector[4]);
    species_traits->set_species(species);
    species_traits->set_trait(trait);
    species_traits->set_species_trait(species_traits->_species, species_traits->_trait);

    return species_traits;

};

void traits::set_species(std::string sp) {_species = sp;}
    
void traits::set_trait(double tr) {_trait = tr;}

void traits::set_species_trait(std::string species, double trait_value) {_trait_map[species] = trait_value;}

double traits::print_trait_val() const {return _trait;}
    
int traits::print_trait_map_size() const {return _trait_map.size();}