import socket

s = socket.socket()
host = socket.gethostname()
port = 9955
s.bind(("127.0.0.1", port))
s.listen(5)
c, addr = s.accept()
data = c.recv(1024)
print(data)
c.send(b'Thank you for your connecting')
c.close()