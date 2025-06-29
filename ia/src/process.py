"""
@file process.py
@brief Command line argument processing for Zappy AI client
"""

import sys
from models import iaParser
from parser import ia_parser
from utils import exit_error
from message import *

def iaProcess(argv: list[str]) -> iaParser:
    """
    @brief Processes command line arguments for the Zappy AI client
    @param argv: List of command line arguments
    @return: iaParser object containing parsed arguments

    @details This function processes the command line arguments to extract the port,
    team name, and machine address. It validates the inputs and returns an iaParser
    object with the parsed values.
    """

    Parser = ia_parser(argv)
    return Parser
