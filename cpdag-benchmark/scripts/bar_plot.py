import os

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

from config import PLOT_DIR
from file_manager import get_result_dir

ALGORITHMS = ["branch-and-bound", "median", "DAG-greedy", "threshold"]

NUM_NODES = {
    "cancer": 5,
    "earthquake": 5,
    "survey": 6,
    "asia": 8,
    "sachs": 11,
    "child": 20,
    "hailfinder": 56,
    "andes": 223,
    "pigs": 441,
}


def get_num_nodes(path: str) -> int:
    """Get number of nodes for the instance result path."""
    short = os.path.basename(path).replace(".dags.txt", "")
    assert short in NUM_NODES
    return NUM_NODES[short]


def get_data_instances() -> list[str]:
    """Get result paths sorted by node count."""
    paths = []
    path = get_result_dir("data")
    for file in os.listdir(path):
        file_path = f"{path}/{file}"
        paths.append(file_path)

    return sorted(paths, key=get_num_nodes)


def get_data() -> dict[str, list[int]]:
    """Read the data."""
    data = {}
    for file_path in get_data_instances():
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

        # Normalize with minimum score (edit distance)
        mn_score = 9999999999
        for score in results.values():
            if score >= 0:
                mn_score = min(mn_score, score)

        for alg, score in results.items():
            if alg not in data:
                data[alg] = []
            data[alg].append(score / mn_score)
    return data


def draw(
    data: dict[str, list[int]],
    filename: str,
) -> None:
    """Draw the bar plot."""
    data = get_data()

    instances = []
    for file_path in get_data_instances():
        instances.append(os.path.basename(file_path).replace(".dags.txt", ""))

    df = pd.DataFrame(data)
    df["Instance"] = instances

    df_long = df.melt(id_vars="Instance", var_name="algorithm", value_name="Edit Distance / Lower Bound")

    sns.set_theme(rc={"figure.figsize": (12, 8)})
    sns.set_style("whitegrid")
    sns.barplot(data=df_long, x="Instance", y="Edit Distance / Lower Bound", hue="algorithm", hue_order=ALGORITHMS)

    plt.ylim(0.98, None)
    plt.title("Edit Distance / Lower Bound")
    plt.ylabel("")
    plt.savefig(filename, dpi=150)
    plt.show()


def main() -> None:
    """Generate the bar plot from the common Bayesian network benchmark dags."""
    data = get_data()
    draw(data, f"{PLOT_DIR}/bar.png")


if __name__ == "__main__":
    main()
