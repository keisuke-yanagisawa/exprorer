# Cosolvent generator

It consists of two sub-parts:

- `cmpd_decomposition`
  - Compounds decomposition
  - Partly modified from Spresso decomposition algorithm [Yanagisawa2017]
- `subst_filtering.ipynb`
  - Filtering of decomposed substructures
  - Modifiable as you like via IPython Notebook 

## Procedure of cosolvent structure generation

First, compounds are decomposed with `cmpd_decomposition`.

```bash
./cmpd_decomposition/exprorer_decompose -l cmpd.sdf -f fragment.sdf -o cmpd_annotated.sdf
```

Secondly, substructure (fragment) information is gathered by below commands.

```bash
grep fragment_info fda_annotated.sdf -A 1 | grep -v -e fragment_info -e "--" | sed "s/,/\n/g" | sort | uniq -c | sort -nr | cut -c9- > fragments.smi
grep fragment_info fda_annotated.sdf -A 1 | grep -v -e fragment_info -e "--" | sed "s/,/\n/g" | sort | uniq -c | sort -nr | cut -c1-8 > counts
paste -d "," fragments.smi counts | sed "s/ //g" > fragments.csv
```

Finally, substructures were filtered ( `subst_filtering.ipynb` ). Selected substructures will be treated as an initial set of cosolvents in the EXPRORER. 


## Reference
[Yanagisawa2017] Yanagisawa K, Komine S, Suzuki SD, Ohue M, Ishida T, Akiyama Y. "Spresso: an ultrafast compound pre-screening method based on compound decomposition", Bioinformatics 33(23): 3836-3843, 2017.
