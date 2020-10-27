import argparse
from scipy import constants

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="generate position restraints files")
    parser.add_argument("-v", action="store_true", help="invert selection")
    parser.add_argument("-res", nargs="+", help="residue names focused on")
    parser.add_argument("-target", choices=["protein", "molecule"], default="protein",
                        help="protein or molecule")
    parser.add_argument("-i", required=True, help="input .gro file")
    parser.add_argument("-oprefix", required=True, help="prefix of output files")
    args = parser.parse_args()

    INPUT_FILE = args.i
    RES = args.res
    PROTEIN = args.target == "protein"
    MOLECULE = args.target == "molecule"
    INV = args.v
    PREFIX = args.oprefix

    mol_resi = -1
    mol_first_atom = -1 if MOLECULE else 1
    WEIGHTS = [0, 10, 20, 50, 100, 200, 500, 1000]

    itps = [open("{}{}.itp".format(PREFIX, i), "w") for i in WEIGHTS]
    for itp in itps:  # init
        itp.write("[ position_restraints ]\n")
        itp.write("; atom  type      fx      fy      fz\n")

    gro_strings = open(INPUT_FILE).readlines()[2:-2]
    for line in gro_strings:
        resi = int(line[:5])
        resn = line[5:10].strip()
        atom = line[10:15].strip()
        nr = int(line[15:20])
        if MOLECULE and (resn in RES) != INV:
            if mol_resi == -1:
                mol_resi = resi
                mol_first_atom = nr
            if mol_resi != resi:
                break
        if not atom.strip().startswith("H"):
            if (resn in RES) != INV:
                for weight, itp in zip(WEIGHTS, itps):
                    c = int(constants.calorie * weight)
                    itp.write("{: 6d}{: 6d}{: 6d}{: 6d}{: 6d}\n"
                              .format(nr-mol_first_atom+1, 1, c, c, c))
    [itp.close() for itp in itps]
