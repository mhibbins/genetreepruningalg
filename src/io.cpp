#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include <fstream>
#include <sstream>
#include <tuple>
#include <cctype>

#include "io.hpp"
#include "clade.hpp"
#include "traits.hpp"

std::string TREE_STRUCT = "genetrees";

struct option longopts[] = {
  { "infile", required_argument, NULL, 'i' },
  { "tree_struct", required_argument, NULL, 't'}
};

input_parameters read_arguments(int argc, char *const argv[])
{
    input_parameters my_input_parameters;
    if (argc == 1)
    {
        my_input_parameters.help = true;
        return my_input_parameters;
    }
    my_input_parameters.input_file_path = argv[1];
    my_input_parameters.tree_structure = argv[2];

    return my_input_parameters;
}

std::ostream& operator<<(std::ostream &out, clade* my_clade) {
  out << my_clade->is_root();
  return out;
}

std::string clade_to_string(clade* my_clade) {
  std::stringstream ss;
  ss << my_clade;
  return ss.str();
}

std::vector<std::string> simple_tokenizer(std::string s) {

  std::stringstream ss(s);
  std::string word;
  std::vector<std::string> line_vector;

  while (ss >> word) {
    line_vector.push_back(word);
  }

  return line_vector;
}

std::tuple<input_parameters, clade*, std::vector<clade*>, std::vector<double>, std::vector<trait>> read_data(int argc, char *const argv[]){ //parses data from input file 

  input_parameters user_input = read_arguments(argc, argv); //reads command line arguments
  std::ifstream input_file(user_input.input_file_path); //gets input file argument
    
  if (user_input.help == true) { //checks if input file is specified 
      std::cout << "No input file specified" << std::endl;
  }

  TREE_STRUCT = user_input.tree_structure;

  if ((TREE_STRUCT != "sptree") && (TREE_STRUCT != "genetrees")) { //checks if tree structure arg is valid
      std::cout << "Invalid argument to tree structure" << std::endl;
  }

  std::string line;
  std::string header;

  clade* sptree;
  std::vector<clade*> genetrees;
  std::vector<double> freqs;
  std::vector<trait> species_traits;

  while (getline(input_file, line)) {

    if (line.empty()) continue;
    
    if (line.find("sptree") != std::string::npos){
      header = "sptree";
    }

    if (line.find("genetrees") != std::string::npos){
      header = "genetrees";
    }

    if (line.find("genetreefreqs") != std::string::npos) {
      header = "genetreefreqs";
    }

    if (line.find("traits") != std::string::npos){
      header = "traits";
    }

    if (header == "sptree"){
      if (line.rfind("(", 0) == 0){
        sptree = parse_newick(line);
      }
    }

    if (header == "genetrees") {
      if (line.rfind("(", 0) == 0){
        clade* genetree = parse_newick(line);
        genetrees.push_back(genetree);
      }
    }

    if (header == "genetreefreqs") {
      if (isdigit(line[0])) {
        std::vector<std::string> line_vector = simple_tokenizer(line);
	      double freq = std::stod(line_vector[0]);
	      freqs.push_back(freq);
      }
    }

    if (header == "traits") {
      if (line.rfind("set", 0) == 0){
        trait species_trait = parse_traits(line);
        species_traits.push_back(species_trait);
      }
    }
  
  }
  return {user_input, sptree, genetrees, freqs, species_traits};  
}
