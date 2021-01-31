## Requirements

- python 3 (`$PYTHON`)
  - Several modules are needed (an example of installation is shown below).
- openbabel 2.4.1
  - **Do not use the latest version, 3.0.0 or newer** since we have not tested yet.
  - It is possible to install it along with python 3 via conda.
- AmberTools 18 (`$TLEAP`, `$CPPTRAJ`)
  - It is needed for use of `tleap` and `cpptraj`
- Gromacs 2019.4 (`$GMX`)
- packmol 18.169 (`$PACKMOL`)
- (Gaussian 16.B01)
  - So far we have not included the codes for QM calculation of probes (structural optimization and RESP charge calculation)
  - It is needed **only** when new probes are introduced.



### Construction of Python3 environment with Conda

```sh
conda create -n ENVIRONMENT_NAME
conda install -n ENVIRONMENT_NAME -c conda-forge rdkit pandas numpy biopython jinja2 griddataformats parmed
conda install -n ENVIRONMENT_NAME -c conda-forge openbabel==2.4.1
```


### Preparation

Please modify `setting/initialize.sh` to correctly use all executables and GPUs.

### Test execution
If all settings goes well, below commands run MD calculations.
```
cd /PATH/TO/exprorer/cmd_calculation    # MUST RUN THEM AT THIS DIRECTORY
bash msmd_job.sh example/msmd_config.sh
```
The commands will run 20 runs of Gromacs, each of which includes 40 ns of production run as a default.

### 異なるタンパク質を用いた計算を行う場合

1. `example/protein.conf` を参考に、タンパク質のpdbファイルへの絶対パス、ジスルフィド結合に関する情報を記述したconfigファイルを作成する。（現時点ではbinding_site_residuesは利用していない）
  - なお、コードの内部で自動的に残基番号は1からに修正されるので注意。（入力pdbの残基番号と出力pdbの残基番号は異なっている可能性がある）
2. `example/msmd_config.sh`を参考に、新しいconfigファイルを作成、1.で作成したconfigファイルを使うように指定する。

###　異なるProbeを用いた計算を行う場合

1. `example/probe.conf` を参考に、Probeのmol2ファイルおよびpdbファイルへの絶対パス、そのprobeのIDを指定する。現時点では原子タイプやProbe濃度はGAFF2, 0.25Mで固定としているので、その部分は修正する必要はない。
2. `example/msmd_config.sh`を参考に、新しいconfigファイルを作成、1.で作成したconfigファイルを使うように指定する。
