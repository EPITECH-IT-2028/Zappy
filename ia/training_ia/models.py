class iaParser:
    """
    Represents a iaParser body.
    """
    def __init__(self, port, name, machine):
        """
        Initializes a iaParser body.

        Args:
            port (int): port number
            name (str): name of the team
            machine (str): name of the machine;localhost by default
        """
        self.port = port
        self.name = name
        self.machine = machine

class Question:
    """
    Represents a Question body.
    """
    def __init__(self, rows):
        self.rows = rows

# class iaMessage:
#     """
#     Represents a iaMessage body.
#     """
#     def __init__(self, message, encrypt, received):
#         """
#         Initializes a iaParser body.
#
#         Args:
#             message (list:str): list of messages
#             encrypt (list:str): name of the team
#             received (list:str): name of the machine
#         """
#         self.message = message
#         self.encrypt = encrypt
#         self.received = received