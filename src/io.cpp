#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include <fstream>

#include "io.hpp"
#include "clade.hpp"

struct option longopts[] = {
  { "infile", required_argument, NULL, 'i' }
};

clade* read_data(std::istream& input_file){ //parses trees from input file 

  std::string line;
  std::string header;

  clade* sptree;

  std::vector<std::string> lines;

  while (getline(input_file, line)) {

    if (line.empty()) continue;
    lines.push_back(line);
    
    if (line.find("sptree") != std::string::npos){
      header = "sptree";
    }

    if (line.find("genetrees") != std::string::npos){
      header = "genetrees";
    }

    if (line.find("traits") != std::string::npos){
      header = "traits";
    }

    if (header == "sptree"){
      if (line.rfind("(", 0) == 0){
        sptree = parse_newick(line);
      }
    }
  
  }
    return sptree;  
}