import random

import sample_data


def build_prob(
    node: str,
    deps: list[str],
    options: dict[str, int],
    states: list[int],
    node_prob: list[tuple[list[int], list[float]]],
) -> None:
    """Build the probability list for a node."""
    # End of recursion, build node_prob
    if len(states) == len(deps):
        stops = [0.0, 1.0]
        for _ in range(options[node] - 1):
            stops.append(random.random())
        stops.sort()

        probs = []
        for i in range(options[node]):
            probs.append(stops[i + 1] - stops[i])
        node_prob.append((states[:], probs))
        return

    # Build all possible states arrays by recursively calling self
    x = deps[len(states)]
    for i in range(options[x]):
        states.append(i)
        build_prob(node, deps, options, states, node_prob)
        states.pop()


def create_and_sample(n: int) -> None:
    """Build Bayesian network and call sample_data()."""
    nodes: list[str] = []
    options: dict[str, int] = {}
    prob: dict[str, list[tuple[list[int], list[float]]]] = {}
    deps: dict[str, list[str]] = {}

    for i in range(n):
        nodes.append(str(i))
    for i in range(n):
        options[str(i)] = random.choice([2, 2, 3, 4])
    for i in range(n):
        if i == 0:
            deps["0"] = []
            continue
        num_deps = min(i, random.choice([1, 2, 2, 3, 4]))
        node_deps = random.sample(range(i), num_deps)
        deps[str(i)] = [str(x) for x in node_deps]
    for i in range(n):
        node_prob = []
        build_prob(str(i), deps[str(i)], options, [], node_prob)
        prob[str(i)] = node_prob

    sample_data.sample_data(nodes, options, prob, deps)
