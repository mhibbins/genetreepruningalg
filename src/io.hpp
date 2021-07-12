#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>

extern struct option longopts[];

class clade;
class traits;

std::tuple<clade*, clade*, std::vector<traits*>> read_data(std::istream& input_file);

std::ostream& operator<<(std::ostream &out, clade* my_clade);
std::string clade_to_string(clade* my_clade);
std::vector<std::string> simple_tokenizer(std::string s);

struct input_parameters {
public:	
    std::string input_file_path;
    bool help = false;
    //void check_input();
};
