#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>

#include "optimizer.hpp"

extern struct option longopts[];

class clade;
class trait;

std::tuple<clade*, std::vector<clade*>, std::vector<trait>> read_data(std::istream& input_file);

std::ostream& operator<<(std::ostream &out, clade* my_clade);
std::string clade_to_string(clade* my_clade);
std::vector<std::string> simple_tokenizer(std::string s);

struct input_parameters {
public:	
    std::string input_file_path;
    optimizer_parameters optimizer_params;
    bool help = false;
    //void check_input();
};
