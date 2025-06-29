"""
@file: parser.py
@brief: Zappy AI client command line argument parser
"""

import sys

from utils import exit_error
from models import iaParser

def ia_parser(argv: list[str]) -> iaParser or None:
    """
    @brief Parses command line arguments for the Zappy AI client
    @param argv: List of command line arguments
    @return: iaParser object containing parsed arguments or None on error

    @details This function processes the command line arguments to extract the port,
    team name, and machine address. It validates the inputs and returns an iaParser
    object with the parsed values. If any argument is invalid or missing, it exits
    with an error message.
    @raises ValueError: If any argument is invalid or missing
    """
    
    try:
        port = None
        name = None
        machine = None
        for arg in argv:
            if arg == "-p":
                port = argv[argv.index("-p") + 1]
            if arg == "-n":
                name = argv[argv.index("-n") + 1]
            if arg == "-h":
                machine = argv[argv.index("-h") + 1]
        if machine is None:
            machine = "localhost"

        if not isinstance(name, str) or name.strip() == "":
            exit_error("Error: Invalid or missing team name (-n).")

        try:
            port = int(port)
            if port <= 0 or port > 65535:
                raise ValueError
        except (ValueError, TypeError):
            exit_error("Error: Invalid or missing port (-p must be a number between 1 and 65535).")

        if not isinstance(machine, str) or machine.strip() == "":
            exit_error("Error: Invalid machine hostname (-h).")
        Parser = iaParser(
            port=port,
            name=name,
            machine=machine
        )
        return Parser
    except ValueError as e:
        exit_error(f"Error parsing: {e}")
        return None