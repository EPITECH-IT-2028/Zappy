"""
@file zappyIa.py
@brief Main entry point for the Zappy AI client
"""

import sys
import info
import process
import game

if __name__ == "__main__":
    """
    @brief Main function to start the Zappy AI client

    @details This function checks the command line arguments, processes them,
    and starts the game loop. It handles the number of arguments and displays
    usage information if the arguments are incorrect.
    """
    
    if len(sys.argv) != 2 and len(sys.argv) != 5 | len(sys.argv) != 7:
        sys.stderr.write("Invalid number of arguments (-help for more informations).\n")
        sys.exit(84)

    if len(sys.argv) == 2:
        if sys.argv[1] == "-help":
            info.info()
        sys.stderr.write("Invalid number of arguments (-help for more informations).\n")
        sys.exit(84)

    args = process.iaProcess(sys.argv)

    game.game(args)

    sys.exit(0)