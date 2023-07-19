#Socket client example in python

import socket   #for sockets

#create an AF_INET, STREAM socket (TCP)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.settimeout(100000)
s.connect(("127.0.0.1",9997))
data = s.recv(1024)
print(data)
s.send(b'Thank you for your connecting')
s.close()