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

## Reference
[Beasley1990] Beasley JE. "A Lagrangian Heuristic for Set-Covering Problems", Nav Res Logist 37: 151-164, 1990.  
