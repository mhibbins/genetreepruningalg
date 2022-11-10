import sys 


def read_traits(Routput):

    traits = []
 
    for line in Routput:
        if len(line.split()) == 4: 
        
            split_traits = line.split()
            sp4_trait, sp3_trait, sp2_trait = split_traits[1], split_traits[2], split_traits[3]
            traits.append([sp4_trait, sp3_trait, sp2_trait])
   

    return traits

def write_input(traits, file_prefix):

    for i in range(len(traits)):
        with open((file_prefix + str(i) + ".txt"), 'w') as inputfile:

            inputfile.write("NEXUS\n")
            inputfile.write("begin sptree;\n")
            inputfile.write("(sp2:0.781,(sp3:0.062,sp4:0.062):0.719);\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin genetrees;\n")
            inputfile.write("(sp2:1.781,(sp3:0.378,sp4:0.378):1.403);\n")
            inputfile.write("(sp2:2.116,(sp3:1.116,sp4:1.116):1.0);\n")
            inputfile.write("(sp3:2.116,(sp2:1.116,sp4:1.116):1.0);\n")
            inputfile.write("(sp4:2.116,(sp2:1.116,sp3:1.116):1.0);\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin genetreefreqs;\n")
            inputfile.write("0.514\n")
            inputfile.write("0.162\n")
            inputfile.write("0.162\n")
            inputfile.write("0.162\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin traits;\n")
            inputfile.write("set trait sp2 = " + str(traits[i][2]) + "\n")
            inputfile.write("set trait sp3 = " + str(traits[i][1]) + "\n")
            inputfile.write("set trait sp4 = " + str(traits[i][0]) + "\n")
            inputfile.write("end;")



Rout = sys.stdin
traits = read_traits(Rout)
write_input(traits, sys.argv[1])

