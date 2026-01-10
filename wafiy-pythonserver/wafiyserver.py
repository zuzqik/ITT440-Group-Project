import socket
import mysql.connector
import time
import threading
from datetime import datetime

# DB_CONFIG host must match the --name of your database container
DB_CONFIG = {
    'host': 'db_container',
    'user': 'root',
    'password': 'password',
    'database': 'project_db'
}

def update_db_loop():
    points = 0
    while True:
        try:
            conn = mysql.connector.connect(**DB_CONFIG)
            cursor = conn.cursor()
            points += 5  # Points must increase every time updated 
            now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            
            # Update user in database table [cite: 40]
            query = "REPLACE INTO user_stats (user, points, datetime_stamp) VALUES (%s, %s, %s)"
            cursor.execute(query, ("wafiy_user", points, now))
            conn.commit()
            
            print(f"Update Success: {points} points at {now}")
            cursor.close()
            conn.close()
        except Exception as e:
            print(f"Waiting for Database... {e}")
        
        time.sleep(30) # Must update every 30 seconds 

def start_socket_server():
    # Create TCP socket [cite: 38]
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 8080)) # Port > 1024 [cite: 39]
    server.listen(5)
    
    while True:
        client, addr = server.accept()
        data = client.recv(1024).decode()
        if data == "GET_LATEST_POINT":
            # Server accesses DB before passing to client [cite: 50]
            conn = mysql.connector.connect(**DB_CONFIG)
            cursor = conn.cursor()
            cursor.execute("SELECT points FROM user_stats WHERE user='wafiy_user'")
            val = cursor.fetchone()
            client.send(str(val[0]).encode() if val else b"No Data")
            conn.close()
        client.close()

if __name__ == "__main__":
    threading.Thread(target=update_db_loop, daemon=True).start()
    start_socket_server()
