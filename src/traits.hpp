#ifndef traits_hpp
#define traits_hpp

#include <string>
#include <map>
#include <vector>
#include <tuple>

class traits;

class traits {
    friend traits* parse_traits(std::string trait_string); //allows parse_traits to set class attributes
    std::string _species;
    double _trait;
    std::map<std::string, double> _trait_map;
public:

    void set_species(std::string sp);
    
    void set_trait(double tr);

    void set_species_trait(std::string species, double trait_value);

    double print_trait_val() const;

    int print_trait_map_size() const;
};

traits* parse_traits(std::string trait_string);
#endif