#ifndef io_hpp
#define io_hpp

#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>

extern struct option longopts[];

class clade;

std::tuple<clade*, clade*> read_data(std::istream& input_file);

std::ostream& operator<<(std::ostream &out, clade* my_clade);
std::string clade_to_string(clade* my_clade);
std::vector<std::string> simple_tokenizer(std::string s);

struct input_parameters {
public:	
    std::string input_file_path;
    bool help = false;
    //void check_input();
};
#endif
