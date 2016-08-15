import telnetlib
import socket
import matplotlib

HOST = '192.168.1.107'
PORT = 5555
BUFFER_SIZE = 1024

panel = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
panel.connect((HOST,PORT))

pixels = [0xA0]
#number = 0
#for i in range(0,256):
#    pixels.append(number)
#    number = number + 1
print(pixels)
panel.send(bytearray(pixels))
panel.close()
