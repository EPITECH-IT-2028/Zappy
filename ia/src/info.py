"""
@file info.py
@brief Zappy AI client usage information handler
@author Epitech Project 2025
@date 2025

This module provides a function to display usage information for the Zappy AI client.
"""

import sys

def info() -> None:
    """
    @brief Displays usage information for the Zappy AI client
    @return None
    @details Prints the command line usage instructions for the Zappy AI client,
    including required parameters such as port, name, and machine.
    This function is typically called when the user requests help or when
    the command line arguments are incorrect.
    """

    print("""USAGE
\tUSAGE: ./zappy_ai -p port -n name -h machine""")

    sys.exit(0)