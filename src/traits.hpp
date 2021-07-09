#ifndef traits_hpp
#define traits_hpp

#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "clade.hpp"

class traits;

class traits {
    friend traits parse_traits(std::string trait_string); //allows parse_traits to set param values
private:
    std::string _species;
    double _trait;
    std::map<std::string, double> _trait_map;
public:

    void set_species(std::string sp) {
        _species = sp;
    }
    void set_trait(double tr) {
        _trait = tr;
    }
    void set_species_trait(std::string species, double trait_value) {
        _trait_map[species] = trait_value;
    };
    void parse_traits(std::string trait_string);
};
#endif