import contextlib
import os

from dotenv import load_dotenv

FILE_DIR = os.path.dirname(__file__)
with contextlib.suppress(FileNotFoundError):
    load_dotenv(dotenv_path=".env")

DATASET = os.getenv("DATASET") or ""
N_WORKERS = int(os.getenv("J") or "8")


INPUT_DIR = f"{FILE_DIR}/../inputs"
RESULT_DIR = f"{FILE_DIR}/../results"
PLOT_DIR = f"{FILE_DIR}/../plots"
