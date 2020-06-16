# SCP solver

## Installation
### Build

```sh
make all
```

## Usage

### Command

```sh
./scp_solver < data/scp41.txt
```

### Input file format
The input file format is the same as [OR-Library](http://people.brunel.ac.uk/~mastjjb/jeb/info.html) [Beasley1990]

The homepage describes the format briefly:

```
The format of all of these 80 data files is:
number of rows (m), number of columns (n)
the cost of each column c(j),j=1,...,n
for each row i (i=1,...,m): the number of columns which cover
row i followed by a list of the columns which cover row i
```

## Reference
[Beasley1990] Beasley JE. "OR-Library: Distributing Test Problems by Electronic Mail", J Oper Res Soc 41(11): 1069-1072, 1990.
