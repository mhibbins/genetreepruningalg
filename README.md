# Gene tree pruning algorithm

Code related to the implementation of Felsenstein's pruning algorithm over a gene tree set, as described in Hibbins et al. 2023. The primary function related to our new implementation can be found beginning on line 140 of `src/probability.cpp`. Much of the C++ source code in `src/` is drawn from previous implementations in [CAFE](https://github.com/hahnlab/CAFE) and [CAGEE](https://github.com/hahnlab/CAGEE). 

## Installation 

See `src/gtpruningalg.exe` for a compiled executable. To re-compile, clone this repository, `cd` into `genetreepruningalg/`, and then run the following: 

    g++ -w -g -std=c++17 ./src/*.cpp -o ./src/gtpruningalg.exe

## Usage

The basic usage of the software is as follows: 

    gtpruningalg.exe inputfile.txt mode
    
Where `inputfile.txt` is NEXUS-formatted and specifies both species trees and a gene tree set, and trait values (see `basic_test_input.txt`). The `mode` argument will accept `sptree` for a standard inference using the input phylogeny, or `genetrees` for our implementation over the specified gene tree set. The software will print the outcome of the maximum-likelihood optimization of the evolutionary rate. Basic usage example: 

    src/gtpruningalg.exe basic_test_input.txt sptree #species tree inference
    src/gtpruningalg.exe basic_test_input.txt genetrees #gene tree inference
    
## Analysis 

The `analyses` directory contains code for three analyses conducted in Hibbins et al. 2022. `accuracy_analysis/` contains code for the simulation study of the accuracy of our gene tree approach, and generated the results shown in Figure 4B. `gt_error_analysis/` contains code for the simulation study of gene tree estimation error, and generated the results shown in Figure 5B. `tomato_analysis/` contains code for the empirical analysis of tomato floral traits, and was used to generate the results in Figure 6B. 


