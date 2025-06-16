import sys

import process
import game

if __name__ == "__main__":
    if len(sys.argv) != 5:
        sys.stderr.write("Only 1 argument are accept\n")
        sys.exit(84)

    args = process.ia_process(sys.argv)

    game.game(args)

    sys.exit(0)