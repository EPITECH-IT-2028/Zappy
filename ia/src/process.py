import sys
# from models import iaParser
# from parser import ia_parser
# from utils import exit_error
from message import *

def iaProcess(argv: list[str]) -> None:
        """
        Main function that handles command line arguments.

        Args:
            argv (list[str]): Command line arguments

        Returns:
            None
        """
        a = encrypt('I_need_help_to_level_up_to_X_with_Y')
        print(a)
        b = decrypt(a)
        print(b)
        c = encrypt('I_am_starting_to_play')
        print(c)
        d = decrypt(c)
        print(d)
        return None
