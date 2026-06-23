import os
import sys


def format_data(dest_file: str) -> None:
    """Format /tmp/raw.dags to .dags file."""
    filename = "/tmp/raw.dags"
    if not os.stat(filename):
        print(f"No such file {filename}")
        sys.exit(1)

    with open(filename) as f:
        lines = f.readlines()

    with open(dest_file, "w") as f:
        f.write(f"{len(lines)}\n")

        for raw_line in lines:
            line = raw_line.strip()
            node_i = 1
            node_map = {}
            raw_edges = []
            for el in line.split(" "):
                if "<-" not in el:
                    continue
                x = el.split("<-")[0]
                nodes = el.split("<-")[1].split(",")
                for node in nodes:
                    if node != "":
                        raw_edges.append((node, x))

                node_map[x] = node_i
                node_i += 1

            f.write(f"{node_i - 1} {len(raw_edges)}\n")
            f.writelines(f"{node_map[a]} {node_map[b]}\n" for a, b in raw_edges)
