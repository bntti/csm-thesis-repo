import sys
from typing import NoReturn


def debug_log(text: str) -> None:
    """Debug log."""
    print(f"\33[90m{text}\33[0m")


def log(text: str) -> None:
    """Regular log."""
    print(f"\33[96m{text}\33[0m")


def error_log(text: str) -> NoReturn:
    """Error log."""
    sys.exit(f"\33[91m{text}\33[0m")
