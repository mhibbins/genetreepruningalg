import sys 

with open(sys.argv[1], 'r') as output_file:
    for line in output_file:
        if "Best match" in line:
            sigma_squared = line.split()[3]

try:
    print(sigma_squared, sys.argv[2], sys.argv[3], sys.argv[4])
except:
    print("Unsuccessful search")
