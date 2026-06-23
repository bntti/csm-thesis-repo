import os
import subprocess
from concurrent.futures import ProcessPoolExecutor, as_completed

import tqdm

from config import DATASET, N_WORKERS
from file_manager import get_input_dir, get_result_dir
from util import error_log, log


def compile_cpp() -> None:
    """Compile dag-benchmark."""
    log("Compiling")
    subprocess.run(["cmake", "-B", "build"], check=True)
    subprocess.run(["cmake", "--build", "build"], check=True)


def run_single(file: str, input_dir: str, result_dir: str) -> str:
    """Run a single instance of dag-benchmark."""
    ret = subprocess.run(
        ["./dag-benchmark", f"{input_dir}/{file}"],
        check=True,
        capture_output=True,
    )
    output_file = f"{result_dir}/{file}.txt"
    with open(output_file, "w") as f:
        f.write(ret.stdout.decode())
    return file


def run_benchmark(dataset: str) -> None:
    """Run the benchmark with the given data set and engine."""
    input_dir = get_input_dir(dataset)
    result_dir = get_result_dir(dataset)

    files = os.listdir(input_dir)

    if len(files) == 0:
        error_log(f"No inputs for {dataset}")

    log(f"Running benchmark '{dataset}'," + f" Total test instances {len(files)}")

    with ProcessPoolExecutor(max_workers=N_WORKERS) as executor:
        futures = [executor.submit(run_single, f, input_dir, result_dir) for f in files]
        for future in tqdm.tqdm(as_completed(futures), total=len(files)):
            future.result()


def main() -> None:
    """Run the benchmark with the data set read from .env and an engine given by the user."""
    log(f"Input data: {DATASET}")

    compile_cpp()
    run_benchmark(DATASET)


if __name__ == "__main__":
    main()
