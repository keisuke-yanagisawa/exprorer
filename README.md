# EXPRORER
**EXPRORER** (**EX**tended **PRO**bes set construction by **RE**presentative **R**etrieval) is 
a systematic procedure to construct a set of cosolvent for drug discovery.

This repository consists of two parts:
- **cosolv_generator**
  - For generation of cosolvent structures
  - Two sub-parts: compounds decomposition & substructure filtering
- **scp_solver**
  - A minimum set cover problem solver based on [Beasley1990]
  - For construction of cosolvent sets
  
## Overall requirements

- C++
  - [Boost](https://www.boost.org/) 1.69.0
    - We haven't confirmed more recent versions
  - [Open Babel](http://openbabel.org/wiki/Main_Page) 2.4.1
    - **Do not use the latest version, 3.1.1 or newer** since we found the code did not work with it.
- Python 3

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

echo "export BOOST_INSTALL_PATH=/PATH/TO/BOOST/1_69_0" > ~/.bashrc
echo "export LD_LIBRARY_PATH=$BOOST_INSTALL_PATH/lib:$LD_LIBRARY_PATH" > ~/.bashrc
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

echo "export OBABEL_INSTALL_PATH=/PATH/TO/openbabel/2_4_1" > ~/.bashrc
echo "export LD_LIBRARY_PATH=$OBABEL_INSTALL_PATH:lib:$LD_LIBRARY_PATH" > ~/.bashrc
source ~/.bashrc
```

### Python 3


## Reference
[Beasley1990] Beasley JE. "A Lagrangian Heuristic for Set-Covering Problems", Nav Res Logist 37: 151-164, 1990.  
