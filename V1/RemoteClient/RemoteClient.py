import telnetlib

HOST = '192.168.1.107'
PORT = 23

panel = telnetlib.Telnet(HOST)

panel.write([127, 127, 127])
