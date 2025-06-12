import sys
from models import iaParser
from parser import ia_parser
from utils import exit_error
from data import open_csv
# from message import *

def iaProcess(argv: list[str]) -> None:
        """
        Main function that handles command line arguments.

        Args:
            argv (list[str]): Command line arguments

        Returns:
            None
        """
        # Fernet = None
        # message: List[str] = ["coucou", "caca", "robin", "etienne"]
        # crypted: List[Tuple[bytes, bytes]] = []
        # decrypted: List[str] = []
        # crypted = get_message(message)
        # print("sentence crypted: ", crypted)
        # decrypted = get_decrypted_message(crypted)
        # print("sentence decrypted: ", decrypted)
        # Parser = ia_parser(argv)
        # Parser = ia_parser(argv)
        # return Parser
        open_csv(sys.argv[5])

        
