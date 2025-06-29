import unittest
import sys
from unittest.mock import patch, MagicMock
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

import message as msg
from parser import ia_parser

class TestIaParser(unittest.TestCase):

    @patch('parser.exit_error')
    @patch('parser.iaParser')
    def test_valid_arguments(self, mock_iaParser, mock_exit):
        mock_parser_instance = MagicMock()
        mock_iaParser.return_value = mock_parser_instance

        argv = ["-p", "4242", "-n", "team1", "-h", "localhost"]
        result = ia_parser(argv)

        mock_iaParser.assert_called_once_with(port=4242, name="team1", machine="localhost")
        self.assertEqual(result, mock_parser_instance)
        mock_exit.assert_not_called()

    @patch('parser.exit_error')
    @patch('parser.iaParser')
    def test_default_hostname(self, mock_iaParser, mock_exit):
        mock_parser_instance = MagicMock()
        mock_iaParser.return_value = mock_parser_instance

        argv = ["-p", "4242", "-n", "team1"]
        result = ia_parser(argv)

        mock_iaParser.assert_called_once_with(port=4242, name="team1", machine="localhost")
        self.assertEqual(result, mock_parser_instance)
        mock_exit.assert_not_called()

    @patch('parser.exit_error', side_effect=SystemExit)
    def test_missing_port(self, mock_exit):
        argv = ["-n", "team1"]
        with self.assertRaises(SystemExit):
            ia_parser(argv)
        mock_exit.assert_called_once()
        self.assertIn("port", mock_exit.call_args[0][0])

    @patch('parser.exit_error', side_effect=SystemExit)
    def test_invalid_port(self, mock_exit):
        argv = ["-p", "invalid", "-n", "team1"]
        with self.assertRaises(SystemExit):
            ia_parser(argv)
        mock_exit.assert_called_once()
        self.assertIn("port", mock_exit.call_args[0][0])

    @patch('parser.exit_error', side_effect=SystemExit)
    def test_port_out_of_range(self, mock_exit):
        argv = ["-p", "70000", "-n", "team1"]
        with self.assertRaises(SystemExit):
            ia_parser(argv)
        mock_exit.assert_called_once()
        self.assertIn("port", mock_exit.call_args[0][0])

    @patch('parser.exit_error', side_effect=SystemExit)
    def test_missing_name(self, mock_exit):
        argv = ["-p", "4242"]
        with self.assertRaises(SystemExit):
            ia_parser(argv)
        mock_exit.assert_called_once()
        self.assertIn("team name", mock_exit.call_args[0][0])

    @patch('parser.exit_error', side_effect=SystemExit)
    def test_empty_name(self, mock_exit):
        argv = ["-p", "4242", "-n", ""]
        with self.assertRaises(SystemExit):
            ia_parser(argv)
        mock_exit.assert_called_once()
        self.assertIn("team name", mock_exit.call_args[0][0])

    @patch('parser.exit_error', side_effect=SystemExit)
    def test_empty_hostname(self, mock_exit):
        argv = ["-p", "4242", "-n", "team1", "-h", ""]
        with self.assertRaises(SystemExit):
            ia_parser(argv)
        mock_exit.assert_called_once()
        self.assertIn("machine hostname", mock_exit.call_args[0][0])

class SimpleEncodingTestCase(unittest.TestCase):
    def test_encrypt_basic(self):
        test_strings = ["hello", "", "a", "123"]

        for test_str in test_strings:
            encrypted = msg.encrypt(test_str)
            decrypted = msg.decrypt(encrypted)
            self.assertEqual(test_str, decrypted, f"Failed for input: '{test_str}'")

    def test_encrypt_decrypt_inverse(self):
        test_messages = ["Hello World", "Testing123", "", "Special$#@Characters!"]

        for message in test_messages:
            encrypted = msg.encrypt(message)
            decrypted = msg.decrypt(encrypted)
            self.assertEqual(message, decrypted)


if __name__ == "__main__":
    unittest.main()