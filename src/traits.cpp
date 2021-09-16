#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>

#include "io.hpp"
#include "traits.hpp"
#include "matrix_cache.hpp"

trait::trait(std::string new_species, double new_trait) {
    _species = new_species;
    _trait = new_trait;
    _trait_map[new_species] = new_trait;
}

trait parse_traits(std::string trait_string) {

    std::vector<std::string> line_vector = simple_tokenizer(trait_string);
    std::string species = line_vector[2];
    double sp_trait = std::stod(line_vector[4]);
    trait species_traits(species, sp_trait);

    return species_traits;

};

double trait::get_trait_val() const {return _trait;}
std::string trait::get_species() const {return _species;}
int trait::print_trait_map_size() const {return _trait_map.size();}

double get_species_trait(std::string species, std::vector<trait> traits) {

    for (int i = 0; i < traits.size(); i++) {
        if (traits[i].get_species() == species) {
            return traits[i].get_trait_val();
        }
    }
}

boundaries get_trait_range(std::vector<trait> t_vector) {
    
    //std::pair<double, double> trait_range;
    std::vector<double> trait_values;

    if (!t_vector.empty()){
        for (int i=0; i < t_vector.size(); i++){
            trait sp_trait = t_vector[i];
                trait_values.push_back(sp_trait.get_trait_val());
        }
    }

    auto minmax = std::minmax_element(trait_values.begin(), trait_values.end());

    boundaries trait_range(*minmax.first, *minmax.second);

    return trait_range;
}
