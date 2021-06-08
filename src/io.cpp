#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>

#include "io.hpp"

using namespace std;

struct option longopts[] = {
  { "infile", required_argument, NULL, 'i' }
};
