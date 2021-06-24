#include <iostream>
#include <vector>
#include <fstream>

extern struct option longopts[];

class clade;

clade* read_data(std::istream& input_file);

struct input_parameters {
public:	
    std::string input_file_path;
    bool help = false;
    //void check_input();
};
