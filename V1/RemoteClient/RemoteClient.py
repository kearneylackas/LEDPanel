import socket
import matplotlib.pyplot as plt
import json
from math import sin, cos, pi


class RemoteClient:
    def __init__(self, host='192.168.1.107', port=5555):
        self._simulation_figure = plt.figure(num=1, facecolor='black', figsize=(10, 10), frameon=False)
        self._simulation = self._simulation_figure.add_subplot(111)


        #self.panel = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #self.panel.connect((host, port))

    def panel_emulator(self, command):
        with open("pixelCoordinates.json", 'r') as file:
            pixels = json.load(file)
            file.close()

        x_up = []
        x_dwn = []
        y_up = []
        y_dwn = []
        for i, pixel in enumerate(pixels):
            if pixel['dir'] == 'up':
                x_up.append(pixel['x'])
                y_up.append(pixel['y'])
            else:
                x_dwn.append(pixel['x'])
                y_dwn.append(pixel['y'])

        up_triangle = [[cos(11 * pi / 6), sin(11 * pi / 6)], [cos(pi / 2), sin(pi / 2)], [cos(7 * pi / 6), sin(7 * pi / 6)]]
        down_triangle = [[cos(pi / 6), sin(pi / 6)], [cos(5 * pi / 6), sin(5 * pi / 6)], [cos(3 * pi / 2), sin(3 * pi / 2)]]

        self._simulation.clear()
        self._simulation.scatter(x_up, y_up, s=2800, marker=up_triangle, c=command, linewidths=0.0, edgecolor='none')
        self._simulation.scatter(x_dwn, y_dwn, s=2800, marker=down_triangle, c=command, linewidths=0.0, edgecolor='none')
        self._simulation.axis('off')


