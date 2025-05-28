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