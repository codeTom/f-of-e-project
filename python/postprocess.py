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
import csv

#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#s.connect(("localhost", 19021))
#f = s.makefile()
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_ylim(-400,400)
ax.set_xlim(-400,400)
ax.set_zlim(0,600)

plt.ion()
c = 0
data = []

with open("chair_raised.csv") as csvfile:
    csvreader = csv.reader(csvfile, delimiter=',')
    for row in csvreader:
        if "#" in row[0]:
            continue
        data.append(np.array(row, dtype=np.float))

data = np.array(data)

print(data)

ax.scatter(data[:,4],data[:,5],data[:,6], c=data[:,3], cmap = cm.coolwarm)
ax.view_init(elev=20,azim=45)
plt.tight_layout()
plt.show(block=True)
