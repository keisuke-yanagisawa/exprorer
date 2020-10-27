import argparse

VERSION = "1.0.0"


def position_restraint_include(molname):
    ret_str = "; Include Position restraint file\n"
    for i in [1000, 500, 200, 100, 50, 20, 10, 0]:
        ret_str += '#ifdef POSRES{}\n#include "{}{}.itp"\n#endif\n'.format(i, molname, i)
    return "\n"+ret_str+"\n"


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="modify topology records")
    parser.add_argument("-v,--version", action="version", version=VERSION)
    parser.add_argument("-i", required=True, help="input topology file")
    parser.add_argument("-o", required=True, help="output topology file")
    parser.add_argument("-prefix", default="posrePROTEIN")
    args = parser.parse_args()

    with open(args.i) as fin:
        with open(args.o, "w") as fout:
            curr_section = None
            mol_count = 0
            for line in fin:
                if line.startswith("["):
                    curr_section = line[line.find("[")+1:line.find("]")].strip()
                    if curr_section == "moleculetype":
                        if mol_count == 1:
                            fout.write(position_restraint_include(args.prefix))
                        mol_count += 1

                fout.write(line)
