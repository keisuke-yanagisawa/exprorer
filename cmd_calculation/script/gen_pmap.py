import argparse
import configparser
from subprocess import getoutput as gop

import parmed as pmd


def gen_parm7(in_top, in_gro, out_parm7):
    gromacs = pmd.load_file(in_top, xyz=in_gro)
    gromacs.save(out_parm7, overwrite=True)

def gen_pdb(in_gro, out_pdb, gmx):
    ret = gop(f"""{gmx} trjconv -s {in_gro} -f {in_gro} -o {out_pdb} << EOF
0
EOF""")
    print(ret)

def do_cpptraj(top, resi_st, resi_ed, traj, ref, ref_resi_st, ref_resi_ed, cid, prefix,
               n=100, d=1, traj_start=1, traj_stop="last", traj_offset=1, cpptraj_exe="cpptraj"):

    log = gop(f"""{cpptraj_exe} -p {top} << EOF
trajin {traj} {traj_start} {traj_stop} {traj_offset}
parm {ref}
reference {ref} parm {ref} [REF]

unwrap :{resi_st}-{resi_ed}@CA
center :{resi_st}-{resi_ed}@CA
fiximagedbonds
autoimage

rms ToREF ref [REF] :{resi_st}-{resi_ed}@CA :{ref_resi_st}-{ref_resi_ed}@CA out rmsd.dat


# "center" maybe changed AFTER grid generation. be careful...
# file name must not start with a number (i.e. 1L2S_A11_nV.dx)
# grid map_{prefix}_nV.dx  {n} {d} {n} {d} {n} {d} center :{resi_st}-{resi_ed}@CA :{cid}&(!@VIS)
# grid map_{prefix}_nVH.dx {n} {d} {n} {d} {n} {d} center :{resi_st}-{resi_ed}@CA :{cid}&(!@VIS)&(!@H*)
# grid map_{prefix}_V.dx   {n} {d} {n} {d} {n} {d} center :{resi_st}-{resi_ed}@CA :{cid}@VIS
# grid map_{prefix}_O.dx   {n} {d} {n} {d} {n} {d} center :{resi_st}-{resi_ed}@CA :{cid}@O*
grid map_{prefix}_nV.dx  {n} {d} {n} {d} {n} {d} :{cid}&(!@VIS)
grid map_{prefix}_nVH.dx {n} {d} {n} {d} {n} {d} :{cid}&(!@VIS)&(!@H*)
grid map_{prefix}_V.dx   {n} {d} {n} {d} {n} {d} :{cid}@VIS
grid map_{prefix}_O.dx   {n} {d} {n} {d} {n} {d} :{cid}@O*

trajout {prefix}_position_check.pdb start 1 stop 1 offset 1
trajout {prefix}_position_check2.pdb offset 100
go
exit
EOF""")
    print(log)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="gen probability map")
    parser.add_argument("-p, --topology", dest="topology", required=True)
    parser.add_argument("-y, --trajin", dest="trajin", required=True)
    parser.add_argument("-c, --reference", dest="reference", required=True)
    parser.add_argument("--outprefix", dest="outprefix", default="PMAP")
    parser.add_argument("--start", dest="start", default=1)
    parser.add_argument("--stop", dest="stop", default="last")
    parser.add_argument("--offset", dest="offset", default=1)
    parser.add_argument("--cpptraj", default="cpptraj", help="path to cpptraj executable")
    parser.add_argument("--gmx", default="gmx_mpi", help="path to gromacs executable")
    parser.add_argument("prot_param")
    parser.add_argument("cosolv_param")

    args = parser.parse_args()
    params = configparser.ConfigParser()
    params.read(args.prot_param)
    params.read(args.cosolv_param)
    if "ReferenceStructure" not in params:
        print("INFO: [ReferenceStructure] is not set in any config file. [Protein] will be used alternatively.")
        params["ReferenceStructure"] = params["Protein"]

    #temp_parm7 = ".temp.parm7"
    #gen_parm7(args.topology,
    #          args.reference,
    #          temp_parm7)
    tmp_pdb = ".tmp.pdb"
    gen_pdb(args.reference,
            tmp_pdb,
            args.gmx)

    do_cpptraj(tmp_pdb, #temp_parm7,
               params["Protein"]["resi_st"],
               params["Protein"]["resi_ed"],
               args.trajin,
               params["ReferenceStructure"]["pdb"],
               params["ReferenceStructure"]["resi_st"],
               params["ReferenceStructure"]["resi_ed"],
               params["Cosolvent"]["cid"],
               args.outprefix,
               traj_start=args.start,
               traj_stop=args.stop,
               traj_offset=args.offset,
               cpptraj_exe=args.cpptraj)
