import threading
import matplotlib.pyplot as plt
import numpy as np
import time
import requests
import socket
from numpy import sin,cos
import time
from matplotlib.animation import FuncAnimation
from threading import Thread
from matplotlib import cm


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("localhost", 19021))
f = s.makefile()
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_ylim(-500,500)
ax.set_xlim(-500,500)
ax.set_zlim(0,1000)

plt.ion()
plt.show()
c = 0
points = [[],[],[],[]]

def read_data():
    #first read all previous data
    #this is a little hacky
    last_time = time.time()
    preread = 0
    while time.time()-last_time < 10e-3:
        last_time = time.time()
        f.readline()
        preread+=1

    print(f"#Discarded {preread} lines")
    print(f"time,azimuth/deg,elevation/deg,distance/mm,x/mm,y/mm,z/mm")
    while True:
        line = f.readline()
        data = line.strip().split(",")
        if len(data) != 3 or not data[0] or not data[1] or not data[2]:
            print("Invalid line:")
            print(line.strip())
            continue
        #convert radial to x y z coordinates
        try:
            theta = np.pi/2-int(data[1]) * np.pi/180
            phi = int(data[0]) * np.pi/180
            r = int(data[2])
            if r > 2000: #sensor range
                continue
            #theta=np.pi/2 #2D test
        except:
            print("Invalid data")
            print(line.strip())

        point = np.array([
            r * cos(phi) * sin(theta),
            r * sin(phi) * sin(theta),
            r * cos(theta),
            r
        ])
        for i in range(4):
            points[i].append(point[i])

        print(f"{time.time()-last_time},{data[0]},{data[1]},{data[2]},{point[0]},{point[1]},{point[2]}")

reading = Thread(target=read_data)
reading.start()

while True:
    ax.scatter(points[0],points[1],points[2], c=points[3], cmap = cm.coolwarm)
    #plt.draw()
    plt.pause(0.05)
    #print(len(points[0]))