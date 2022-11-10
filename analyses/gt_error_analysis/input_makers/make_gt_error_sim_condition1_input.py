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
            inputfile.write("(sp2:12.5,(sp3:1.0,sp4:1.0):11.5);\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin genetrees;\n")
            inputfile.write("(sp2:13.5,(sp3:1.999,sp4:1.999):11.501);\n")
            inputfile.write("(sp2:13.833,(sp3:12.833,sp4:12.833):1.0);\n")
            inputfile.write("(sp3:13.833,(sp2:12.833,sp4:12.833):1.0);\n")
            inputfile.write("(sp3:13.833,(sp2:12.833,sp3:12.833):1.0);\n")
            inputfile.write("end;\n")
            inputfile.write("\n")
            inputfile.write("begin genetreefreqs;\n")
            inputfile.write("0.99999\n")
            inputfile.write("0.00001\n")
            inputfile.write("0.00001\n")
            inputfile.write("0.00001\n")
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

