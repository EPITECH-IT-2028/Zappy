def encrypt(message: str) -> str:
    encrypt_message = ''
    temp_encrypt_message = []
    for char in message:
        temp_encrypt_message.append(ord(char) + 390)
    temp_encrypt_message.reverse()
    for char in temp_encrypt_message:
        encrypt_message += chr(char)
    return encrypt_message


def decrypt(message: str) -> str:
    decrypt_message = ''
    temp_decrypt_message = []
    for char in message:
        temp_decrypt_message.append(ord(char))
    temp_decrypt_message.reverse()
    for char in temp_decrypt_message:
        char -= 390
        decrypt_message += chr(char)
    return decrypt_message