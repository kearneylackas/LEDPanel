from pprint import pprint
from math import *
import numpy

deltaX = 19.24500897
deltaY = 11.11111111
cornerRadius = 22.22222222
stripLength = 25
rotation = -pi/3

data = []
data.append([0, 1*deltaX,  -1*deltaY, -1])
data.append([0, 2*deltaX,  -2*deltaY, +1])
data.append([0, 3*deltaX,  -1*deltaY, -1])
data.append([0, 4*deltaX,  -2*deltaY, +1])
data.append([0, 5*deltaX,  -1*deltaY, -1])
data.append([0, 6*deltaX,  -2*deltaY, +1])
data.append([0, 7*deltaX,  -1*deltaY, -1])
data.append([0, 8*deltaX,  -2*deltaY, +1])
data.append([0, 9*deltaX,  -1*deltaY, -1])
data.append([0, 8*deltaX,  -4*deltaY, +1])
data.append([0, 7*deltaX,  -5*deltaY, -1])
data.append([0, 6*deltaX,  -4*deltaY, +1])
data.append([0, 5*deltaX,  -5*deltaY, -1])
data.append([0, 4*deltaX,  -4*deltaY, +1])
data.append([0, 3*deltaX,  -5*deltaY, -1])
data.append([0, 2*deltaX,  -4*deltaY, +1])
data.append([0, 3*deltaX,  -7*deltaY, -1])
data.append([0, 4*deltaX,  -8*deltaY, +1])
data.append([0, 5*deltaX,  -7*deltaY, -1])
data.append([0, 6*deltaX,  -8*deltaY, +1])
data.append([0, 7*deltaX,  -7*deltaY, -1])
data.append([0, 6*deltaX, -10*deltaY, +1])
data.append([0, 5*deltaX, -11*deltaY, -1])
data.append([0, 4*deltaX, -10*deltaY, +1])
data.append([0, 5*deltaX, -13*deltaY, -1])

for strip in range(1,6):
    for point in range(stripLength):
        tempX = data[stripLength*(strip-1)+point][1]
        tempY = data[stripLength*(strip-1)+point][2]
        tempDir = data[stripLength*(strip-1)+point][3]
        data.append([strip, tempX*cos(rotation) - tempY*sin(rotation), tempX*sin(rotation) + tempY*cos(rotation), -tempDir])

upTriangle = [[cos(11 * pi / 6), sin(11 * pi / 6)], [cos(pi/2), sin(pi / 2)], [cos(7 * pi / 6), sin(7 * pi / 6)]]
downTriangle = [[cos(pi / 6), sin(pi / 6)], [cos(5 * pi / 6), sin(5 * pi / 6)], [cos(3 * pi / 2), sin(3 * pi / 2)]]

dataJSON = []

for pixel in range(150):
    x = data[pixel][1]
    y = data[pixel][2]

    if data[pixel][3] == 1:
        direction = 'up'
        corners = [{'x': x + cornerRadius*upTriangle[0][0], 'y': y + cornerRadius*upTriangle[0][1]}, {'x': x + cornerRadius*upTriangle[1][0], 'y': y + cornerRadius*upTriangle[1][1]}, {'x': x + cornerRadius*upTriangle[2][0], 'y': y + cornerRadius*upTriangle[2][1]}]
    else:
        direction = 'down'
        corners = [{'x': x + cornerRadius*downTriangle[0][0], 'y': y + cornerRadius*downTriangle[0][1]}, {'x': x + cornerRadius*downTriangle[1][0], 'y': y + cornerRadius*downTriangle[1][1]}, {'x': x + cornerRadius*downTriangle[2][0], 'y': y + cornerRadius*downTriangle[2][1]}]

    dataJSON.append({'strip': data[pixel][0], 'x': data[pixel][1], 'y': data[pixel][2], 'dir': direction, 'corners': corners})

import json

with open('pixelCoordinates.json', 'w') as file:
    json.dump(dataJSON, file, indent=2)
    file.close()
