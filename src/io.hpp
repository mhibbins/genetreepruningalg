#include <iostream>
#include <vector>

using namespace std;

extern struct option longopts[];

struct input_parameters {
public:	
    std::string input_file_path;
    bool help = false;
    //void check_input();
};
