# EXPRORER
**EXPRORER** (**EX**tended **PRO**bes set construction by **RE**presentative **R**etrieval) is 
a systematic procedure to construct a set of cosolvent for drug discovery [Yanagisawa].

This repository contains two parts of it:
- **cosolv_generator**: Generation of cosolvent structures
  - Two sub-parts: compounds decomposition & substructure filtering
- **cmd_calculation**: Calculation scripts for Cosolvent MDs
- **scp_solver**: Construction of cosolvent sets
  - A minimum set cover problem solver based on [Beasley1990]
  
## Overall requirements
Versions are an examples of an author's environment.

- C++ (cosolv_generator, scp_solver)
  - [Boost](https://www.boost.org/) 1.69.0
  - [Open Babel](http://openbabel.org/wiki/Main_Page) 2.4.1
    - **Do not use the latest version, 3.0.0 or newer** since we found the code did not work with it.
- Python 3 (cosolv_generator)
  - [RDKit](https://www.rdkit.org/) 2020.03.1
  - OpenBabel 2.4.1 (Please DO NOT use 3.x.x)

As for `cmd_calculation`, requirements are shown in `cmd_calculation/README.md`.

## Environment construction
This section shows an example of an author (Keisuke Yanagisawa) without administrator privileges (i.e. supercomputing system), and we do not guarantee the procedure works well or not in your computational environment. 

### C++

#### boost 1.69.0

```bash
cd /PATH/TO/TMPDIR
wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
tar -zxvf boost_1_69_0.tar.gz
cd boost_1_69_0
./bootstrap.sh
./b2 install --prefix=PATH/TO/BOOST/1_69_0 --without-python

echo "export BOOST_INSTALL_PATH=/PATH/TO/BOOST/1_69_0" >> ~/.bashrc
source ~/.bashrc
echo "export LD_LIBRARY_PATH=$BOOST_INSTALL_PATH/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
source ~/.bashrc
```

#### Open Babel 2.4.1

```bash
cd /PATH/TO/TMPDIR
wget https://sourceforge.net/projects/openbabel/files/openbabel/2.4.1/openbabel-2.4.1.tar.gz/download -O openbabel-2.4.1.tar.gz
tar -zxvf openbabel-2.4.1.tar.gz
cd openbabel-2.4.1
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/PATH/TO/openbabel/2_4_1 .. 
make -j
make test
make install

echo "export OBABEL_INSTALL_PATH=/PATH/TO/openbabel/2_4_1" >> ~/.bashrc
source ~/.bashrc
echo "export LD_LIBRARY_PATH=$OBABEL_INSTALL_PATH/lib:$LD_LIBRARY_PATH" >> ~/.bashrc
source ~/.bashrc
```

### Python 3
This example uses Anaconda (It is possible to use `pip` instead of it).

```bash
cd /PATH/TO/TMPDIR
wget https://repo.anaconda.com/archive/Anaconda3-2020.02-Linux-x86_64.sh
bash ./Anaconda3-2020.02-Linux-x86_64.sh
source ~/.bashrc

conda install rdkit=2020.03.1
```

## Reference
[Beasley1990] Beasley JE. "A Lagrangian Heuristic for Set-Covering Problems", *Nav Res Logist* **37**: 151-164, 1990.

[Yanagisawa2021] Yanagisawa K, Moriwaki Y, Terada T, Shimizu K, "EXPRORER: Rational Cosolvent Set Construction Method for Cosolvent Molecular Dynamics Using Large-Scale Computation", *J Chem Inf Model* **61**: 2744-2753, 2021.
- doi: [10.1021/acs.jcim.1c00134](https://doi.org/10.1021/acs.jcim.1c00134) - open access
