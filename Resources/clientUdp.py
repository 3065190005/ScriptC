import socket

udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

dest_addr = ('127.0.0.1', 9933)

send_data = "Hello Python"

print("send")

udp_socket.bind(('127.0.0.1', 9934))

recv_data = udp_socket.recvfrom(1024)

udp_socket.sendto(send_data.encode('utf-8'), dest_addr)

print(recv_data[0])


udp_socket.close()