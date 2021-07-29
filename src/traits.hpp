#ifndef traits_hpp
#define traits_hpp

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>

class traits;

class traits {
    friend traits parse_traits(std::string trait_string); //allows parse_traits to set class attributes
    std::string _species;
    double _trait;
    std::map<std::string, double> _trait_map;
public:
    traits() : _species(NULL), _trait(0) {}
    traits(std::string new_species, double new_trait);
    double get_trait_val() const;
    std::string get_species() const;
    int print_trait_map_size() const;
};

traits parse_traits(std::string trait_string);
std::pair<double, double> get_trait_range(std::vector<traits> t_vector);
#endif