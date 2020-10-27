### 必要なツールなど

- python 3
  - Condaを用いたPython環境構築を後述
- openbabel 2.4.1
  - Condaでついでに入れることも可能（後述）
- AmberTools 18
- Gromacs 2019.4
- Gaussian 16.B01
  - 現在のこのコードにはGaussianを使ったprobeの構造最適化・部分電荷付与は含まれていない
  - 新たなprobeが必要になった場合のみ
- packmol 18.169



### CondaによるPython環境構築

```sh
conda create -n ENVIRONMENT_NAME
conda install -n ENVIRONMENT_NAME -c conda-forge rdkit pandas numpy biopython jinja2 griddataformats parmed
conda install -n ENVIRONMENT_NAME -c conda-forge openbabel==2.4.1
```


前提条件として、packageディレクトリ以下で実行を行う必要がある（相対パスが含まれているため）
### テスト実行のために必要な作業

1. `setting/initialize.sh` を書き換える。これは計算対象となるタンパク質やProbeによらず一定のものが記載される場所である。
2. `bash msmd_job.sh example/msmd_config.sh` を実行すると、（全ての環境変数がうまく行っていれば）系を自動で構築し、Gromacsによる計算を開始する。現時点の設定
では40nsのproduction runを20 runs行うように指定されている。

### 異なるタンパク質を用いた計算を行う場合

1. `example/protein.conf` を参考に、タンパク質のpdbファイルへの絶対パス、ジスルフィド結合に関する情報を記述したconfigファイルを作成する。（現時点ではbinding_site_residuesは利用していない）
  - なお、コードの内部で自動的に残基番号は1からに修正されるので注意。（入力pdbの残基番号と出力pdbの残基番号は異なっている可能性がある）
2. `example/msmd_config.sh`を参考に、新しいconfigファイルを作成、1.で作成したconfigファイルを使うように指定する。

###　異なるProbeを用いた計算を行う場合

1. `example/probe.conf` を参考に、Probeのmol2ファイルおよびpdbファイルへの絶対パス、そのprobeのIDを指定する。現時点では原子タイプやProbe濃度はGAFF2, 0.25Mで固定としているので、その部分は修正する必要はない。
2. `example/msmd_config.sh`を参考に、新しいconfigファイルを作成、1.で作成したconfigファイルを使うように指定する。