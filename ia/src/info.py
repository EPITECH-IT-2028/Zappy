import sys


def info() -> None:
    """
    Display usage instructions for the 110borwein program. This function outputs the correct format
    for invoking the program from the command line and briefly describes the purpose of the input
    parameter. It is typically called when the user provides the '-h' option or when incorrect
    command-line arguments are used, providing clarity on how to correctly use the program.

    Usage:
        - Called to provide the user with guidance on how to execute the program and the meaning of
          the parameters it accepts.

    Outputs:
        - The expected command format.
        - A brief description of the program's functionality and the parameter it requires.
    """

    print("""USAGE
\tUSAGE: ./zappy_ai -p port -n name -h machine""")

    sys.exit(0)