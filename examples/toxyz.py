import os

# spherical   200.0e-6
# polymerase  500 7.45e-9
# ribosome    3750    11.0e-9
# metabolism  8950    2.0e-9
# atp 64383   7.0e-10
# metabolites 6054980 5.0e-10

sizes = []
with open("mycoplasmaPacked.dat") as f:
    for l in f:
        sizes.append(float(l.rstrip().split("\t")[3]))
sizes = sorted(list(set(sizes)))
atoms = ["B","C","O","N","Ne","K","Cl","Al"]
print(sizes)

with open("mycoplasmaPacked.xyz","w") as of:
    of.write("6132563\n")
    of.write("Mycoplasma\n")
    with open("mycoplasmaPacked.dat") as f:
        for l in f:
            ls = l.rstrip().split("\t")
            atom = atoms[sizes.index(float(ls[3]))]
            of.write("%s\t%f\t%f\t%f\n"%(atom, float(ls[0]), float(ls[1]), float(ls[2])))
