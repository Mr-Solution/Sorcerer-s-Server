import socket
import sys
import time

messages = [
    'This is the message.',
    'It will be sent ',
    'in parts.',
    'The answer to life, the universe, and everything'
]

server_address = ('localhost', 9002)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

for msg in messages:
    data = msg.encode('utf-8')
    sock.sendto(data, server_address)
    
data, serv_addr = sock.recvfrom(1024)
print(data)
print('from {}'.format(serv_addr) )

print('interchange end')

sock.close()