import os

from config import INPUT_DIR, RESULT_DIR
from util import debug_log, error_log


def get_input_dir(dataset: str) -> str:  # noqa: RET503
    """Get the input dir and generate it if it doesn't exist."""
    input_dir = f"{INPUT_DIR}/{dataset}"
    if os.path.exists(input_dir):
        return input_dir

    error_log(f"No input dir {dataset}")


def get_result_dir(dataset: str) -> str:
    """Get the result dir and create it if it doesn't exist."""
    result_dir = f"{RESULT_DIR}/{dataset}"
    if os.path.exists(result_dir):
        return result_dir

    debug_log(f"Creating result directory {dataset}")
    os.makedirs(result_dir)
    return result_dir


def clean_result_dir(dataset: str) -> None:
    """Clean the result dir."""
    result_dir = get_result_dir(dataset)

    debug_log(f"Clearing result directory {dataset}")
    for file in os.listdir(result_dir):
        os.remove(f"{result_dir}/{file}")
