import socket

# List of ports to open
ports = [8080, 9090, 10000]

sockets = []

try:
    for port in ports:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(("127.0.0.1", port))
        sock.listen(1)
        print(f"Listening on port {port}")
        sockets.append(sock)
    input("Ports are open. Press Enter to close them...\n")
except Exception as e:
    print(f"Error: {e}")
finally:
    for sock in sockets:
        sock.close()
    print("All ports are closed.")