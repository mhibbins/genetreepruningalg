#include <iostream>
#include <vector>
#include <string>

struct input_parameters;

class user_data {
public:
    void read_datafiles(const input_parameters &my_input_parameters);
    void read_speciestree(const input_parameters &my_input_parameters);
};
