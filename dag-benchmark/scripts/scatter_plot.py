import os

import matplotlib.pyplot as plt
import numpy as np

from config import DATASET, PLOT_DIR
from file_manager import get_result_dir


def get_data(path: str, extra_ignored: list[str]) -> dict[str, list[float]]:
    """Read the data."""
    ignored = ["threshold", "empty", "original", *extra_ignored]
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
            score = float(line.split()[1])
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
            if alg in ignored or alg == "lower_bound":
                continue
            if alg not in data:
                data[alg] = []
            data[alg].append(score / mn_score)
    return data


def draw(
    data: dict[str, list[float]],
    filename: str,
) -> None:
    """Draw the scatter plot."""
    rng = np.random.default_rng(42)

    n = len(next(iter(data.values())))
    shared_jitter = rng.uniform(-0.15, 0.15, size=n)

    order = ["bitdp", "greedy", "median", "original"]
    sorted_keys = sorted(data.keys(), key=order.index)

    colors = {
        "bitdp": "#1f77b4",
        "greedy": "#ff7f0e",
        "median": "#2ca02c",
        "original": "#d62728",
        "lower_bound": "#9467bd",
    }

    _, ax = plt.subplots(figsize=(12, 8))
    for col_idx, key in enumerate(sorted_keys):
        x = col_idx + shared_jitter
        ax.scatter(x, data[key], label=key, s=20, color=colors[key])

    ax.set_xticks(range(len(data)))
    ax.set_xticklabels(sorted_keys)

    plt.ylim(0.998, None)
    plt.title("Edit Distance / Lower Bound (n=23)")
    plt.savefig(filename, dpi=150)
    plt.show()


def main() -> None:
    """Generate a scatter plot with the default arguments."""
    size = int(DATASET.split("-")[1])
    path = get_result_dir(DATASET)
    ignored = [] if int(size) < 30 else ["bitdp"]
    data = get_data(path, ignored)

    draw(data, f"{PLOT_DIR}/scatter.png")


if __name__ == "__main__":
    main()
