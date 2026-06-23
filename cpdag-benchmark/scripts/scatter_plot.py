import os

import matplotlib.pyplot as plt
import numpy as np

from config import DATASET, PLOT_DIR
from file_manager import get_result_dir

ALGORITHMS = ["branch-and-bound", "median", "DAG-greedy", "threshold"]


def get_data(path: str) -> dict[str, list[int]]:
    """Read the data."""
    data = {}
    for file in os.listdir(path):
        file_path = f"{path}/{file}"
        with open(file_path) as f:
            lines = f.readlines()

        results = {}
        for line in lines:
            if not line.strip() or "Score" in line:
                continue

            alg = line.split()[0]
            score = int(line.split()[1])
            edges = int(line.split()[3])
            if edges > 0:
                results[alg] = score
            else:
                results[alg] = -1

        mn_score = 9999999999
        for score in results.values():
            if score >= 0:
                mn_score = min(mn_score, score)

        for alg, score in results.items():
            if alg not in ALGORITHMS:
                continue
            if alg not in data:
                data[alg] = []
            data[alg].append(score / mn_score)
    return data


def draw(
    data: dict[str, list[int]],
    filename: str,
) -> None:
    """Draw the scatter plot."""
    rng = np.random.default_rng(42)

    n = len(next(iter(data.values())))
    shared_jitter = rng.uniform(-0.15, 0.15, size=n)

    sorted_keys = sorted(data.keys(), key=ALGORITHMS.index)

    _, ax = plt.subplots(figsize=(12, 8))
    for col_idx, key in enumerate(sorted_keys):
        x = col_idx + shared_jitter
        ax.scatter(x, data[key], label=key, s=20)

    ax.set_xticks(range(len(data)))
    ax.set_xticklabels(sorted_keys)

    plt.ylim(0.98, None)
    plt.title("Edit Distance / Lower Bound (n=15)")
    plt.savefig(filename, dpi=150)
    plt.show()


def main() -> None:
    """Generate a scatter plot from DATA_TYPE."""
    path = get_result_dir(DATASET)
    data = get_data(path)
    draw(data, f"{PLOT_DIR}/scatter.png")


if __name__ == "__main__":
    main()
