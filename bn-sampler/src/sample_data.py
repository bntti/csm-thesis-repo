import gzip
import random
from collections import deque


def read_and_sample(filename: str) -> None:
    """Read Bayesian network and call sample_data()."""
    if filename.endswith(".gz"):
        with gzip.open(filename, "r") as f:
            b_lines = f.readlines()
            lines = (s.decode("ascii") for s in b_lines)
    else:
        with open(filename) as f:
            lines = f.readlines()

    block = None
    var = None
    nodes: list[str] = []
    options: dict[str, int] = {}
    prob: dict[str, list[tuple[list[int], list[float]]]] = {}
    deps: dict[str, list[str]] = {}
    for raw_line in lines:
        line = raw_line.strip()
        if "node" in line:
            var = line.split(" ")[1]
            nodes.append(var)
            block = "var"
            continue
        if "probability" in line:
            items = line.split("( ")[1].split(" )")[0]
            items = items.replace(" |", "").replace(",", "")
            items = items.split(" ")
            var = items[0]
            deps[var] = items[1:]
            prob[var] = []
            block = "probability"
            continue
        if line == "}":
            var = None
            block = None
            continue

        if block == "var":
            assert var is not None
            num = int(line.split("[ ")[1].split(" ]")[0])
            options[var] = num
            continue
        if block == "probability":
            assert var is not None
            if len(deps[var]) == 0:
                vals = []
            else:
                vals = line.split("(")[1].split(")")[0].split(", ")
                vals = list(map(int, vals))
                line = line.split(":")[1].strip()
            probs = line.replace(";", "").split(", ")
            probs = list(map(float, probs))
            prob[var].append((vals, probs))

    sample_data(nodes, options, prob, deps)


def find_topo_order(
    nodes: list[str],
    deps: dict[str, list[str]],
) -> list[str]:
    """Find topological order to the nodes and their deps."""
    order = []
    rem_deps = {}
    queue = deque()
    for node in nodes:
        rem_deps[node] = deps[node][:]
        if len(rem_deps[node]) == 0:
            queue.append(node)
    while len(queue) > 0:
        node = queue.popleft()
        order.append(node)
        # Inoptimal but dags are small
        for node2 in nodes:
            if node in rem_deps[node2]:
                rem_deps[node2].remove(node)
                if len(rem_deps[node2]) == 0:
                    queue.append(node2)
    return order


def sample_data(
    nodes: list[str],
    options: dict[str, int],
    prob: dict[str, list[tuple[list[int], list[float]]]],
    deps: dict[str, list[str]],
) -> None:
    """Sample data from given Bayesian network."""
    order = find_topo_order(nodes, deps)

    with open("/tmp/data.txt", "w") as f:
        f.write(" ".join(order))
        f.write("\n")
        f.writelines(f"{options[node]} " for node in order)
        f.write("\n")

        for _ in range(200):
            values = {}
            for node in order:
                node_deps = deps[node]
                for option in prob[node]:
                    ok = True
                    for i in range(len(node_deps)):
                        if values[node_deps[i]] != option[0][i]:
                            ok = False
                    if not ok:
                        continue
                    node_probs = option[1]
                    break

                r = random.random()
                for i in range(len(node_probs)):
                    r -= node_probs[i]
                    if r <= 1e-9:
                        f.write(f"{i} ")
                        values[node] = i
                        break
                assert node in values
            f.write("\n")
