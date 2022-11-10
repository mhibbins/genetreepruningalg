# -*- coding: utf-8 -*-
"""
Created on Thu Sep 29 14:23:24 2022

@author: Mark
"""

from ete3 import Tree
import copy
import pandas as pd
import math

highILS = Tree("(((((((S.galapagense:60000.0,S.cheesmaniae:60000.0):140000.0,(S.pim1269:100000.0,S.pim1589:100000):100000):1500000.0,(S.neorickii:600000.0,S.arcanum:600000.0):1100000.0):400000.0,(((S.huaylasense:1400000.0,S.peruvianum:1400000.0,S.corneliomulleri:1400000.0):300000.0):1.0,S.chilense:1700000.0):400000.0):300000.0,(S.habrochaites:2000000.0,(S.pen3778:700000.0,S.pen0716:700000):1300000):400000.0):5600000.0):1.0,S.tuberosum:8000000.0);")
lowILS = copy.deepcopy(highILS)

#Get knots 

highILS.prune(["S.galapagense", "S.cheesmaniae", "S.pim1269"], preserve_branch_length=True)
lowILS.prune(["S.pen3778", "S.pen0716", "S.pim1589"], preserve_branch_length=True)

#Get relevant branch lengths

highILS_root = highILS.get_distance("S.galapagense", "S.pim1269")
highILS_split = highILS.get_distance("S.galapagense", "S.cheesmaniae")
highILS_internal = highILS_root - highILS_split

lowILS_root = lowILS.get_distance("S.pen3778", "S.pim1589")
lowILS_split = lowILS.get_distance("S.pen3778", "S.pen0716")
lowILS_internal = lowILS_root - lowILS_split


#Convert to coalescent units

length_list = [highILS_root, highILS_split, highILS_internal,
               lowILS_root, lowILS_split, lowILS_internal]
length_list = [value/400000 for value in length_list]

#Get trait values
trait_df = pd.read_csv(
    'C:/Users/18126/OneDrive - University of Toronto/Projects/pruning_alg/genetreepruningalg/tomato_test/flower morphometrics2.csv')

accessions = ["LA0436", "LA3124", "LA1269", "LA3778", "LA0716", "LA1589"]
trait_df = trait_df.loc[trait_df['AccessionID'].isin(accessions)]
trait_df = trait_df.groupby('AccessionID').mean()

### Function to write input file; need one for each knot and trait

def get_genetree_branch_lengths(knot_branch_lengths): 
    
    t1 = knot_branch_lengths[1]
    t2 = knot_branch_lengths[0]
    
    LStree_split1 = t1 + (1 - ((t2-t1)/(math.exp(t2-t1)-1)))
    LStree_split2 = t2 + 1
    LStree_internal = LStree_split2 - LStree_split1
    
    ILS_split1 = t2 + (1/3)
    ILS_split2 = t2 + (1/3) + 1
    ILS_internal = 1
    
    return [LStree_split2, LStree_split1, LStree_internal,
            ILS_split2, ILS_split1, ILS_internal]

def create_tree_string(accessions, branch_lengths): 
    
    tree_string = ["(", str(accessions[2]), ":", 
                     str(branch_lengths[0]), ",(", str(accessions[1]),
                     ":", str(branch_lengths[1]), ",", 
                     str(accessions[0]), ":", str(branch_lengths[1]),
                     "):", str(branch_lengths[2]), ");"]
    
    tree_string = ''.join(tree_string)
    
    return tree_string
    

def write_pruningalg_input(knot_accessions, knot_branch_lengths, trait_df,
                           trait, filename):
    
    disc_accessions_1 = [knot_accessions[1], knot_accessions[2], 
                         knot_accessions[0]]
    disc_accessions_2 = [knot_accessions[2], knot_accessions[0],
                         knot_accessions[1]]
    
    lineage_sorting = 1 - math.exp(-(knot_branch_lengths[2]))
    ILS = (1/3)*math.exp(-(knot_branch_lengths[2]))
    
    genetree_branch_lengths = get_genetree_branch_lengths(knot_branch_lengths)
    
    sptree_string = create_tree_string(knot_accessions, knot_branch_lengths)
    LS_tree_string = create_tree_string(knot_accessions, 
                                        genetree_branch_lengths[0:3])
    ILS_conc_tree_string = create_tree_string(knot_accessions,
                                              genetree_branch_lengths[3:6])
    ILS_disc1_tree_string = create_tree_string(disc_accessions_1,
                                               genetree_branch_lengths[3:6])
    ILS_disc2_tree_string = create_tree_string(disc_accessions_2,
                                               genetree_branch_lengths[3:6])
    
    
    trait_vals = {knot_accessions[0] : trait_df.loc[knot_accessions[0]][trait],
                  knot_accessions[1] : trait_df.loc[knot_accessions[1]][trait],
                  knot_accessions[2] : trait_df.loc[knot_accessions[2]][trait]}
    
    with open(filename, 'w') as input_file:
        
        input_file.write("#NEXUS\n")
        input_file.write("begin sptree;\n")
        input_file.write(sptree_string + "\n")
        input_file.write("end\n")
        input_file.write("\n")
        input_file.write("begin genetrees;\n")
        input_file.write(LS_tree_string + "\n")
        input_file.write(ILS_conc_tree_string + "\n")
        input_file.write(ILS_disc1_tree_string + "\n")
        input_file.write(ILS_disc2_tree_string + "\n")
        input_file.write("end;\n")
        input_file.write("\n")
        input_file.write("begin genetreefreqs;\n")
        input_file.write(str(lineage_sorting) + "\n")
        for i in range(3): input_file.write(str(ILS) + "\n")
        input_file.write("end'\n")
        input_file.write("\n")
        input_file.write("begin traits;\n")
        input_file.write("set trait " + knot_accessions[0] + " = " +
                         str(trait_vals[knot_accessions[0]]) + "\n")
        input_file.write("set trait " + knot_accessions[1] + " = " +
                         str(trait_vals[knot_accessions[1]]) + "\n")
        input_file.write("set trait " + knot_accessions[2] + " = " +
                         str(trait_vals[knot_accessions[2]]) + "\n")
        input_file.write("end;")
        
    return

trait_names = list(trait_df.columns)

for trait in trait_names:
    
    highILS_filename = "high_ILS_" + trait + "_input.txt"
    lowILS_filename = "low_ILS_" + trait + "_input.txt"
    
    write_pruningalg_input(accessions[0:3], length_list[0:3],
                           trait_df, trait, highILS_filename)
    write_pruningalg_input(accessions[3:6], length_list[3:6],
                           trait_df, trait, lowILS_filename)
        
#write_pruningalg_input(accessions[0:3], length_list[0:3], trait_df,
#                       "SE_PlantMean", "test_file.txt")
        
        
        
    
    
    
    
    


