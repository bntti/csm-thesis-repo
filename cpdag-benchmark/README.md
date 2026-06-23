# CPDAG benchmark

Benchmarking scripts stored in `./scripts/`, and algorithms stored in `./src/`

### Running scripts

Store dataset instances in `./inputs/[dataset]/`

| Script            | Description                                                                |
| ----------------- | -------------------------------------------------------------------------- |
| `run.py`          | Compiles c++ and runs the benchmark given the `DATASET=[dataset]` env.     |
| `bar_plot.py`     | Creates a bar plot from the common Bayesian network benchmark dag results, |
| `scatter_plot.py` | Creates a scatter plot given the `DATASET=[dataset]` env.                  |

### Running manually

To build, run

```
cmake -B build
cmake --build build
```

And then to run,

```
./cpdag-benchmark alarm.dags
```
