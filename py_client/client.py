import socket

# Server details
HOST = "127.0.0.1"  # use server container name if using Docker Compose later
PORT = 5002

# Connect to server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    data = s.recv(1024)  # receive response from server
    print("Server response:", data.decode())
