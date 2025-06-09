import sys
from typing import List, Tuple
from cryptography.fernet import Fernet


def get_message(message: list[str]) -> List[Tuple[bytes, bytes]]:
    try:
        message_crypted: List[Tuple[bytes, bytes]] = []
        if len(message) > 0:
            for index in range(len(message)):
                if message[index] != "\n":
                    message_crypted_temp = encrypt(message[index])
                    message_crypted.append(message_crypted_temp)
                else:
                    e = "Empty message"
        e = "No messages on list"
        return message_crypted
    except Exception as e:
        print(f"Error message: {e}", file=sys.stderr)
        (sys.exit(84))


def get_decrypted_message(message_crypted: List[Tuple[bytes, bytes]]) -> List[str]:
    try:
        message_decrypted: List[str] = []
        if len(message_crypted) > 0:
            for index in range(len(message_crypted)):
                if message_crypted[index] != "\n":
                    message_decrypted_temp = decrypt(message_crypted[index])
                    message_decrypted.append(message_decrypted_temp)
                else:
                    e = "Empty message"
        e = "No messages on list"
        return message_decrypted
    except Exception as e:
        print(f"Error message: {e}", file=sys.stderr)
        (sys.exit(84))


def encrypt(message: str) -> Tuple[bytes, bytes]:
    key = Fernet.generate_key()
    fernet = Fernet(key)
    crypt_message = fernet.encrypt(message.encode())
    return crypt_message, key


def decrypt(message_crypted: Tuple[bytes, bytes]) -> str:
    fernet = Fernet(message_crypted[1])
    decrypt_message = fernet.decrypt(message_crypted[0]).decode()
    return decrypt_message
