import sys

import process
import game

if __name__ == "__main__":
    if len(sys.argv) == 2:
        sys.stderr.write("Only 1 argument are accept\n")
        sys.exit(84)

    process.ia_process(sys.argv)
    # args = process.iaProcess(sys.argv)

    # game.game(args)

    sys.exit(0)