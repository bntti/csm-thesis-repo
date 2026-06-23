# data-gen

## Preliminaries for running

Download the DAG sampler from [openreview.net/forum?id=eq2rjvKbaG](https://openreview.net/forum?id=eq2rjvKbaG) by downloading the "Supplementary Material" zip. In the uncompressed folder, run

```
g++ reject_random.cpp -march=native -O2 -o reject_random
```

And then place `reject_input` in this folders root. Setting up pygobnilp repo in this folders root is also necessary. This project assumes the usage of `miniconda3` and an env named `bayesian`. Change `./run.sh` if necessary

## Running Bayesian network benchmark datasets

### Getting Bayesian network benchmark datasets

Download DSC format `.dsc.gz` files from [www.bnlearn.com/bnrepository/](https://www.bnlearn.com/bnrepository/).

### Running benchmark datasets

```
python3 src/generate_dags.py data/sachs.dsc(.gz)
```

## Sampling DAGs from generated Bayesian networks

```
python3 src/generate_dags.py [num_nodes]
```
