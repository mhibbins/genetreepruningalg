#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include <fstream>

#include "io.hpp"

struct option longopts[] = {
  { "infile", required_argument, NULL, 'i' }
};

clade* read_data(std::istream& input_file){ //parses trees from input file 

  std::string line;

  while (getline(input_file, line)) {

    if (line.empty()) continue;
    std::string header;

    if (line.find("sptree") != std::string::npos){
      header = "sptree";
    }

    if (line.find("genetrees") != std::string::npos){
      header = "genetrees";
    }

    if (line.find("traits") != std::string::npos){
      header = "traits";
    }
    
  }

}