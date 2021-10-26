import sys 


def read_results(out):

    for line in out:

        if "Best match" in line:
            estimate = float(line.split()[3])
        elif "Final" in line:
            likelihood = float(line.split()[2])

    return estimate, likelihood


estimate, likelihood = read_results(sys.stdin)
treestruct, replicate = sys.argv[1], sys.argv[2]

print(estimate, likelihood, treestruct, replicate)

