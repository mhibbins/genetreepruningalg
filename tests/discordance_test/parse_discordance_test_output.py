import sys 


def read_results(out):

    for line in out:

        if "Best match" in line:
            estimate = float(line.split()[3])
        elif "Final" in line:
            likelihood = float(line.split()[2])

    return estimate, likelihood


estimate, likelihood = read_results(sys.stdin)
treestruct, condition, replicate = sys.argv[1], sys.argv[2], sys.argv[3]

print(estimate, likelihood, treestruct, condition, replicate)

