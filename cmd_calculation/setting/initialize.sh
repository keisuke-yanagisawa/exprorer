. $HOME/.tsubame3.sh

module load amber/18up17
export PATH=$(echo -n $PATH |tr ':' '\n' |sed "/\/apps\/t3\/sles12sp2\/isv\/amber\/amber18up17\/anaconda3\/bin/d" |tr '\n' ':')
module load cuda/9.2.148
module load gromacs/2019.4

conda activate md_analysis

PACKMOL=packmol
TLEAP=tleap
CPPTRAJ=cpptraj
GMX=gmx_mpi
PYTHON=python
