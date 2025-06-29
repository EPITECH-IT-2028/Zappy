import unittest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent / 'src'))

import message as msg


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