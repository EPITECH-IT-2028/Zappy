"""
@file: message.py
@brief Message encryption and decryption functions
@author Epitech Project 2025
@date 2025

This module provides functions to encrypt and decrypt messages using a simple character shifting algorithm.
"""

def encrypt(message: str) -> str:
    """
    @brief Encrypts a message by shifting each character's ASCII value
    @param message: The message to encrypt
    @return: The encrypted message as a string

    @details This function takes a string message, shifts each character's ASCII value by 348,
    reverses the order of the characters, and returns the resulting encrypted string.
    """
    
    encrypt_message = ''
    temp_encrypt_message = []
    for char in message:
        temp_encrypt_message.append(ord(char) + 348)
    temp_encrypt_message.reverse()
    for char in temp_encrypt_message:
        encrypt_message += chr(char)
    return encrypt_message


def decrypt(message: str) -> str:
    """
    @brief Decrypts a message by reversing the encryption process
    @param message: The encrypted message to decrypt
    @return: The decrypted message as a string

    @details This function takes an encrypted string message, reverses the order of the characters,
    shifts each character's ASCII value back by 348, and returns the resulting decrypted string.
    """

    decrypt_message = ''
    temp_decrypt_message = []
    for char in message:
        temp_decrypt_message.append(ord(char))
    temp_decrypt_message.reverse()
    for char in temp_decrypt_message:
        char -= 348
        decrypt_message += chr(char)
    return decrypt_message
