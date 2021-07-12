#ifndef traits_hpp
#define traits_hpp

#include <string>
#include <map>
#include <vector>
#include <tuple>

class traits;

class traits {
    friend traits* parse_traits(std::string trait_string); //allows parse_traits to set class attributes
private:
    std::string _species;
    double _trait;
    std::map<std::string, double> _trait_map;
public:

    void set_species(std::string sp) {_species = sp;}
    
    void set_trait(double tr) {_trait = tr;}

    void set_species_trait(std::string species, double trait_value) {_trait_map[species] = trait_value;}

    traits* parse_traits(std::string trait_string);

    double print_trait_val() const {return _trait;}
    int print_trait_map_size() const {return _trait_map.size();}
};
#endif