import contextlib
import os
import shutil
import sys
from pathlib import Path

from tqdm import tqdm

import create_bayesian
import format_data
import sample_data

VERSION = "v1"


def generate(n: int) -> None:
    """Generate Bayesian networks and sample data from them."""
    for i in tqdm(range(100)):
        print("\u001b[94m---------- Generating Bayesian network and sampling data ----------\u001b[0m")
        create_bayesian.create_and_sample(n)

        print("\u001b[94m---------- Running gobnilp ----------\u001b[0m")
        ret = os.system("./run.sh > /tmp/gobnilp.log")
        assert ret == 0

        print("\u001b[94m---------- Generating dags ----------\u001b[0m")
        ret = os.system("./reject_input /tmp/data.jkl > /tmp/raw.dags 2> /tmp/sampler.log")
        assert ret == 0

        if not os.path.exists(f"sampled-{n}-{VERSION}"):
            os.mkdir(f"sampled-{n}-{VERSION}")

        print("\u001b[94m---------- Formatting dags ----------\u001b[0m")
        format_data.format_data(f"sampled-{n}-{VERSION}/sampled-{n}-{i}.dags")


def main() -> None:
    """Run all relevant functions to generate data."""
    if len(sys.argv) < 2:
        print(f"Usage python3 {sys.argv[0]} [data.dsc(.gz)]")
        print(f"Usage python3 {sys.argv[0]} [data.dat]")
        print(f"Usage python3 {sys.argv[0]} [number of nodes in dag]")
        sys.exit(1)

    n = None
    with contextlib.suppress(BaseException):
        n = int(sys.argv[1])
    if n is not None:
        generate(n)
        return

    filename = sys.argv[1]
    if not os.stat(filename):
        print(f"No such file {filename}")
        sys.exit(1)

    if ".dsc" in filename:
        print("\u001b[94m---------- Generating data ----------\u001b[0m")
        sample_data.read_and_sample(filename)
    else:
        shutil.copy(filename, "/tmp/data.txt")

    print("\u001b[94m---------- Running gobnilp ----------\u001b[0m")
    ret = os.system("./run.sh")
    assert ret == 0

    print("\u001b[94m---------- Generating dags ----------\u001b[0m")
    ret = os.system("./reject_input /tmp/data.jkl > /tmp/raw.dags")
    assert ret == 0

    # Remove all extensions from original filename
    path = Path(filename)
    while path.stem != str(path):
        path = Path(path.stem)

    if not os.path.exists(f"dags-{VERSION}"):
        os.mkdir(f"dags-{VERSION}")

    print("\u001b[94m---------- Formatting dags ----------\u001b[0m")
    format_data.format_data(f"dags-{VERSION}/{path}.dags")


if __name__ == "__main__":
    main()
