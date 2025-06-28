import process
import protocole
import socket
import threading

threads = []
clients = []

def add_client(client) -> None : 
  global threads, clients

  client_thread = threading.Thread(
    target=protocole.handle_client,
    args=(client,)
  )
  client_thread.daemon = True
  client_thread.start()
  threads.append(client_thread)
  clients.append(client)

def remove_client(dead_client) -> None :
    global clients

    clients.remove(dead_client)

def cleanup_thread() -> None :
    global threads

    alive_thread = [t for t in threads if t.is_alive()]
    threads = alive_thread

def game(process_args) -> None :
  global threads

  server_address = (process_args.machine, process_args.port)

  unused_slot = 1
  while unused_slot > 0:
    unused_slot = protocole.connect_client(server_address, process_args.name)
    print("Client connected")

  while len(threads) > 0:
    cleanup_thread()

  print("All clients disconnected")