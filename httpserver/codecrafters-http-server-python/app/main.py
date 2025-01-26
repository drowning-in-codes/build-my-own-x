# Uncomment this to pass the first stage
import os
import socket
import threading
import sys
import gzip


def handle_client(conn, addr, save_dir=None):
    print("Connection from", addr)
    # print("Received", conn.recv(1024))
    req: str = conn.recv(1024).decode("utf-8")
    sections = req.split("\r\n")
    # request line
    request_line = sections[0].split(" ")
    method = request_line[0]
    path = request_line[1]
    print("path: ", path)
    # headers
    userAgent = None
    acceptEncoding = None
    headers = req.split("\r\n\r\n")[0].split("\r\n")[1:]
    for header in headers:
        if header.lower().startswith("user-agent"):
            userAgent = header.split(":")[1].strip()
            print("User-Agent: ", userAgent)
        elif header.lower().startswith("accept-encoding"):
            acceptEncoding = header.split(":")[1].strip().split(", ")
    reqBody = req.split("\r\n\r\n")[1]
    msgTempl = (
        "HTTP/1.1 200 OK\r\n" "Content-Type: text/plain\r\n" "Content-Length: {}\r\n"
    )
    fileTempl = (
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: {}\r\n"
    )
    if path == "/":
        conn.sendall(b"HTTP/1.1 200 OK\r\n\r\n")
    elif path.startswith("/echo/"):
        print(acceptEncoding)
        pathName = path.split("/")[2]
        compressed = False
        if acceptEncoding:
            for encoding in acceptEncoding:
                if encoding == "gzip":
                    msgTempl += "Content-Encoding: gzip\r\n"
                    pathName = gzip.compress(pathName.encode("utf-8"))
                    msgTempl = msgTempl.format(len(pathName))
                    msgTempl = msgTempl.encode("utf-8")
                    msgTempl += b"\r\n" + pathName
                    compressed = True
                    conn.sendall(msgTempl)
                    break
            if not compressed:
                msgTempl += "\r\n" + pathName
                conn.sendall(msgTempl.format(len(pathName)).encode("utf-8"))
        else:
            msgTempl += "\r\n" + pathName
            conn.sendall(msgTempl.format(len(pathName)).encode("utf-8"))
    elif path.startswith("/user-agent"):
        msgTempl += "\r\n" + userAgent
        conn.sendall(msgTempl.format(len(userAgent)).encode("utf-8"))
    elif path.startswith("/files/"):
        fileName = path.split("/")[-1]
        filePath = os.path.join(save_dir, fileName)
        if method == "POST":
            with open(filePath, "w") as f:
                f.write(reqBody)
            conn.sendall(b"HTTP/1.1 201 Created\r\n\r\n")
        elif os.path.exists(filePath) and os.path.isfile(filePath):
            with open(filePath, "r") as f:
                file = f.read()
            fileTempl += "\r\n" + str(file)
            conn.sendall(fileTempl.format(os.path.getsize(filePath)).encode("utf-8"))
        else:
            conn.sendall(b"HTTP/1.1 404 Not Found\r\n\r\n")
    else:
        conn.sendall(b"HTTP/1.1 404 Not Found\r\n\r\n")
    conn.close()


def main():
    # You can use print statements as follows for debugging, they'll be visible when running tests.
    print("Logs from your program will appear here!")
    save_dir = ""
    if len(sys.argv) > 1 and sys.argv[1] == "--directory":
        save_dir = sys.argv[2]
        print("dir: ", save_dir)
    # Uncomment this to pass the first stage
    #
    # create socket and bind to addr
    server_socket = socket.create_server(("localhost", 4221), reuse_port=False)
    server_socket.listen()
    while True:
        conn, addr = server_socket.accept()  # wait for client
        my_thread = threading.Thread(
            target=handle_client, args=(conn, addr, save_dir), daemon=False
        )
        my_thread.start()


if __name__ == "__main__":
    main()
