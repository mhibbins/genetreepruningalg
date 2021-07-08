#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "clade.hpp"

class traits {
private:
    std::map<std::string, int> _trait_map;
public:
    void set_species_trait(std::string species, int trait_value) {
        _trait_map[species] = trait_value;
    }
    std::tuple<std::string, double> parse_traits(std::string trait_string);
};