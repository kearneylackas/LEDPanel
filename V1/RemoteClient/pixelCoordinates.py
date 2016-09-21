from pprint import pprint
from math import *
import json

deltaX = 33.33333333
deltaY = 19.24500897
cornerRadius = deltaY*2
stripLength = 25
rotation = -pi/3

imagePixels = 200
imagePanelWidth = 20*deltaX
imagePixelSize = imagePanelWidth/imagePixels

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

for strip in range(1, 6):
    for point in range(stripLength):
        tempX = data[stripLength*(strip-1)+point][1]
        tempY = data[stripLength*(strip-1)+point][2]
        tempDir = data[stripLength*(strip-1)+point][3]
        data.append([strip, tempX*cos(rotation) - tempY*sin(rotation), tempX*sin(rotation) + tempY*cos(rotation), -tempDir])

upTriangle = [[cos(11 * pi / 6), sin(11 * pi / 6)], [cos(pi/2), sin(pi / 2)], [cos(7 * pi / 6), sin(7 * pi / 6)]]
downTriangle = [[cos(pi / 6), sin(pi / 6)], [cos(5 * pi / 6), sin(5 * pi / 6)], [cos(3 * pi / 2), sin(3 * pi / 2)]]

dataJSON = []
count = 0

for pixel in range(150):
    x = data[pixel][1]
    y = data[pixel][2]
    print(x,y,count)
    containedPixels = []

    if data[pixel][3] == 1:
        direction = 'up'
        corners = [{'x': x + cornerRadius*upTriangle[0][0], 'y': y + cornerRadius*upTriangle[0][1]}, {'x': x + cornerRadius*upTriangle[1][0], 'y': y + cornerRadius*upTriangle[1][1]}, {'x': x + cornerRadius*upTriangle[2][0], 'y': y + cornerRadius*upTriangle[2][1]}]

        for xPixel in range(imagePixels):
            xPixelLocation = (xPixel+0.5-imagePixels/2)*imagePixelSize
            for yPixel in range(imagePixels):
                yPixelLocation = (yPixel + 0.5 - imagePixels / 2) * imagePixelSize
                if abs(xPixelLocation-x) < cornerRadius and abs(yPixelLocation-y) < cornerRadius:
                    tri = [corners[0]['x'], corners[1]['y']], [corners[1]['x'], corners[2]['y']], [corners[2]['x'], corners[0]['y']]
                    pt = [xPixelLocation, yPixelLocation]
                    a = 1 / (-tri[1][1] * tri[2][0] + tri[0][1] * (-tri[1][0] + tri[2][0]) + tri[0][0] * (tri[1][1] - tri[2][1]) + tri[1][0] * tri[2][1])
                    s = a * (tri[2][0] * tri[0][1] - tri[0][0] * tri[2][1] + (tri[2][1] - tri[0][1]) * pt[0] + (tri[0][0] - tri[2][0]) * pt[1])
                    t = a * (tri[0][0] * tri[1][1] - tri[1][0] * tri[0][1] + (tri[0][1] - tri[1][1]) * pt[0] + (tri[1][0] - tri[0][0]) * pt[1])
                    if (t > 0) and (s > 0) and (1 - s - t > 0):
                        count += 1
                        containedPixels.append(pt)
    else:
        direction = 'down'
        corners = [{'x': x + cornerRadius*downTriangle[0][0], 'y': y + cornerRadius*downTriangle[0][1]}, {'x': x + cornerRadius*downTriangle[1][0], 'y': y + cornerRadius*downTriangle[1][1]}, {'x': x + cornerRadius*downTriangle[2][0], 'y': y + cornerRadius*downTriangle[2][1]}]

        for xPixel in range(imagePixels):
            xPixelLocation = (xPixel+0.5-imagePixels/2)*imagePixelSize
            for yPixel in range(imagePixels):
                yPixelLocation = (yPixel + 0.5 - imagePixels / 2) * imagePixelSize
                if abs(xPixelLocation - x) < cornerRadius and abs(yPixelLocation - y) < cornerRadius:
                    tri = [corners[0]['x'], corners[1]['y']], [corners[1]['x'], corners[2]['y']], [corners[2]['x'], corners[0]['y']]
                    pt = [xPixelLocation, yPixelLocation]
                    a = 1 / (-tri[1][1] * tri[2][0] + tri[0][1] * (-tri[1][0] + tri[2][0]) + tri[0][0] * (tri[1][1] - tri[2][1]) + tri[1][0] * tri[2][1])
                    s = a * (tri[2][0] * tri[0][1] - tri[0][0] * tri[2][1] + (tri[2][1] - tri[0][1]) * pt[0] + (tri[0][0] - tri[2][0]) * pt[1])
                    t = a * (tri[0][0] * tri[1][1] - tri[1][0] * tri[0][1] + (tri[0][1] - tri[1][1]) * pt[0] + (tri[1][0] - tri[0][0]) * pt[1])
                    if (t > 0) and (s > 0) and (1 - s - t > 0):
                        count += 1
                        containedPixels.append(pt)


    dataJSON.append({'strip': data[pixel][0], 'x': data[pixel][1], 'y': data[pixel][2], 'dir': direction, 'corners': corners, 'pixels': containedPixels})

with open('pixelCoordinates.json', 'w') as file:
    json.dump(dataJSON, file, indent=2)
    file.close()

