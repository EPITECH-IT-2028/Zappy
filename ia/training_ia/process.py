import sys
from models import iaParser
from parser import ia_parser
from protocole import *

def ia_process(argv: list[str]) -> iaParser:
        train_model()
        parser = ia_parser(argv)
        return parser

