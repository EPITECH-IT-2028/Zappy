"""
@file game.py
@brief Main game management and client lifecycle handler
@author Epitech Project 2025
@date 2025

This module manages the main game loop, client connections, and thread lifecycle.
It handles multiple client connections and coordinates their activities with the server.
"""

import process
import protocole
import socket
import threading

## @brief List of active client threads
## @details Contains all threading.Thread objects for currently running clients
threads = []

## @brief List of active client objects
## @details Contains all ZappyClient objects currently connected to the server
clients = []

def add_client(client) -> None :
    """
    @brief Adds a new client to the game and starts its thread
    @param client ZappyClient object to add to the game
    @return None
    
    @details Creates a new daemon thread for the client, starts the initialization
    process, and adds both the thread and client to their respective global lists.
    The thread is marked as daemon to ensure proper cleanup on program exit.
    """

    global threads, clients

    client_thread = threading.Thread(
      target=protocole.initialize_clients,
      args=(client,)
    )
    client_thread.daemon = True
    client_thread.start()
    threads.append(client_thread)
    clients.append(client)

def remove_client(dead_client) -> None :
    """
    @brief Removes a disconnected client from the active clients list
    @param dead_client ZappyClient object to remove
    @return None
    
    @details Removes the specified client from the global clients list.
    This function is called when a client dies or disconnects from the server.
    """

    global clients

    clients.remove(dead_client)

def cleanup_thread() -> None :
    """
    @brief Cleans up finished threads from the threads list
    @return None
    
    @details Removes all threads that are no longer alive from the global
    threads list. This prevents memory leaks and keeps the thread list
    up to date with only active threads.
    """

    global threads

    alive_thread = [t for t in threads if t.is_alive()]
    threads = alive_thread

def game(process_args) -> None :
    """
    @brief Main game loop that manages client connections and lifecycle
    @param process_args Parsed command line arguments containing server info
    @return None
    
    @details This is the main entry point for the game. It:
    1. Establishes connection parameters from command line arguments
    2. Creates new clients as long as there are available slots on the server
    3. Monitors all client threads and cleans up finished ones
    4. Continues until all clients have disconnected
    
    The function will keep creating new clients until the server indicates
    no more slots are available (unused_slot = 0), then waits for all
    clients to finish their execution.

    """

    global threads

    server_address = (process_args.machine, process_args.port)

    unused_slot = 1
    while unused_slot > 0:
      unused_slot = protocole.connect_client(server_address, process_args.name)
      print("Client connected")

    while len(threads) > 0:
      cleanup_thread()

    print("All clients disconnected")