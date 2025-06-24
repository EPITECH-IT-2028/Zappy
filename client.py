import socket
import time

def main():
    host = '127.0.0.1'
    port = 4242

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print(f"Connected to {host}:{port}")

        messages = [
            "team1\n",
            "Look\n",
            "Take"
            "sibur\n"
        ]

        for msg in messages:
            print(f"Sending: {msg.strip()}")
            s.sendall(msg.encode())
            time.sleep(0.5)  # short delay between messages

if __name__ == "__main__":
    main()
