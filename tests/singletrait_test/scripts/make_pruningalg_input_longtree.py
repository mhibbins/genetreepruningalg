import sys 


def read_traits(Routput):

    split_traits = Routput.split()
    sp4_trait, sp3_trait, sp2_trait = split_traits[1], split_traits[2], split_traits[3]
    trait_vector = [sp4_trait, sp3_trait, sp2_trait]
    
    return trait_vector

def write_input(traits, filename):

    with open(filename, 'w') as inputfile:

        inputfile.write("NEXUS\n")
        inputfile.write("begin sptree;\n")
        inputfile.write("(sp2:12.0,(sp3:6.0,sp4:6.0):6.0);\n")
        inputfile.write("end;\n")
        inputfile.write("\n")
        inputfile.write("begin genetrees;\n")
        inputfile.write("(sp2:2.2,(sp3:0.87,sp4:0.87):1.33);\n")
        inputfile.write("(sp2:2.532,(sp3:1.532,sp4:1.532):1);\n")
        inputfile.write("(sp3:2.532,(sp2:1.532,sp4:1.532):1);\n")
        inputfile.write("(sp4:2.532,(sp2:1.532,sp3:1.532):1);\n")
        inputfile.write("end;\n")
        inputfile.write("\n")
        inputfile.write("begin traits;\n")
        inputfile.write("set trait sp2 = " + str(traits[2]) + "\n")
        inputfile.write("set trait sp3 = " + str(traits[1]) + "\n")
        inputfile.write("set trait sp4 = " + str(traits[0]) + "\n")
        inputfile.write("end;")



Rout = sys.stdin.read()
traits = read_traits(Rout)
write_input(traits, sys.argv[1])

