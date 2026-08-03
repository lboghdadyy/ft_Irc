import socket
import threading
import time

HOST = "127.0.0.1"
PORT = 6667

def client(i):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))

        s.sendall(b"PASS password\r\n")
        s.sendall(f"NICK user{i}\r\n".encode())
        s.sendall(f"USER user{i} 0 * :user{i}\r\n".encode())

        time.sleep(5000)

        s.close()
    except Exception as e:
        print(i, e)

threads = []

for i in range(100):
    t = threading.Thread(target=client, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()
