#include <iostream>
#include <vector>
#include <string>


struct input_parameters;

class user_data {
public:
    void read_datafile(const input_parameters &my_input_parameters);
    void read_inputfile(const input_parameters &my_input_parameters);
};