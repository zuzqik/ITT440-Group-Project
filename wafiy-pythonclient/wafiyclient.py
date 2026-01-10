import socket
import time

SERVER_HOST = "python-server"  # container name later
SERVER_PORT = 5001

while True:
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((SERVER_HOST, SERVER_PORT))

        client.sendall(b"GET_POINTS")
        response = client.recv(1024).decode()

        print("Server response:", response)

        client.close()
        time.sleep(10)

    except Exception as e:
        print("Error:", e)
        time.sleep(5)
	
