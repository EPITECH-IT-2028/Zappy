"""
@filename: models.py
@brief: AI parser model definition
"""

class iaParser:
    """
    @brief Represents a Zappy AI client parser
    @details This class encapsulates the necessary attributes for a Zappy AI client,
    including the port number, team name, and machine address.
    @attributes:
        port (int): The port number for the Zappy server connection.
        name (str): The name of the AI team.
        machine (str): The address of the Zappy server machine.
    """
    
    def __init__(self, port, name, machine):
        self.port = port
        self.name = name
        self.machine = machine
