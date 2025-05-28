import sys

import info
import process

if __name__ == "__main__":
    if len(sys.argv) != 2 and len(sys.argv) != 5 | len(sys.argv) != 7:
        sys.stderr.write("Invalid number of arguments (-help for more informations).\n")
        sys.exit(84)

    if len(sys.argv) == 2:
        if sys.argv[1] == "-help":
            info.info()
        sys.stderr.write("Invalid number of arguments (-help for more informations).\n")
        sys.exit(84)

    process.iaProcess(sys.argv)

    sys.exit(0)