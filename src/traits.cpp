#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>

#include "io.hpp"
#include "traits.hpp"

traits::traits(std::string new_species, double new_trait) {
    _species = new_species;
    _trait = new_trait;
    _trait_map[new_species] = new_trait;
}

traits* parse_traits(std::string trait_string) {

    std::vector<std::string> line_vector = simple_tokenizer(trait_string);
    std::string species = line_vector[2];
    double trait = std::stod(line_vector[4]);
    traits traits_object(species, trait);
    traits* species_traits = &traits_object;

    return species_traits;

};

double traits::get_trait_val() const {return _trait;}
std::string traits::get_species() const {return _species;}
int traits::print_trait_map_size() const {return _trait_map.size();}

std::pair<double, double> get_trait_range(std::vector<traits*> t_vector) {
    
    std::pair<double, double> trait_range;
    std::vector<double> trait_values;

    if (!t_vector.empty()){
        for (int i=0; i < t_vector.size(); i++){
            traits* trait = t_vector[i];
                trait_values.push_back(trait->get_trait_val());
        }
    }

    trait_range.first = trait_values[0];
    trait_range.second = trait_values[1];
    //trait_range.first = *std::min_element(trait_values.begin(), trait_values.end());
    //trait_range.second = *std::max_element(trait_values.begin(), trait_values.end());

    return trait_range;
}
