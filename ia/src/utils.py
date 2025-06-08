import sys

def exit_error(message) -> None:
    """
    Displays an error message and exits the program.

    Args:
        message (str): Error message to display.
    """
    sys.stderr.write(message + "\n")
    sys.exit(84)