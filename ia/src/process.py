import sys
from models import iaParser
from parser import ia_parser
from utils import exit_error

def iaProcess(argv: list[str]):
        """
        Main function that handles command line arguments.

        Args:
            argv (list[str]): Command line arguments

        Returns:
            None
        """
        Parser = ia_parser(argv)
