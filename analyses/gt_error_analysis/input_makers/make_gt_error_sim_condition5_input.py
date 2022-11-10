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
            inputfile.write("(sp2:1.5625,(sp3:0.125,sp4:0.125):1.4375);\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin genetrees;\n")
            inputfile.write("(sp2:2.5625,(sp3:0.677,sp4:0.677):1.885);\n")
            inputfile.write("(sp2:2.895,(sp3:1.895,sp4:1.895):1.0);\n")
            inputfile.write("(sp3:2.895,(sp2:1.895,sp4:1.895):1.0);\n")
            inputfile.write("(sp4:2.895,(sp2:1.895,sp3:1.895):1.0);\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin genetreefreqs;\n")
            inputfile.write("0.763\n")
            inputfile.write("0.079\n")
            inputfile.write("0.079\n")
            inputfile.write("0.079\n")
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

