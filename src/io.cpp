#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include <fstream>

#include "io.hpp"

struct option longopts[] = {
  { "infile", required_argument, NULL, 'i' }
};

clade* read_trees(std::istream& input_file){ //parses trees from input file 

  std::string line;

  while (getline(input_file, line)) {
    if (line.empty()) continue;
    std::cout << line << "\n";
  }

}