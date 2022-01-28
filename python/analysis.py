#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jan 27 01:44:55 2022

@author: filip
"""

import threading
import matplotlib.pyplot as plt
import matplotlib
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
#fig = plt.figure()
#ax = fig.add_subplot(111, projection='3d')
#ax.set_ylim(-400,400)
#ax.set_xlim(-400,400)
#ax.set_zlim(0,600)

#plt.ion()
c = 0
data = []

with open("chair_raised.csv") as csvfile:
    csvreader = csv.reader(csvfile, delimiter=',')
    for row in csvreader:
        if "#" in row[0]:
            continue
        data.append(np.array(row, dtype=np.float))

data = np.array(data)

timediff = data[1:,0]-data[:-1,0]
timediff *= 1000
timediff[timediff>200] = 200
plt.hist(timediff, bins=100, density=True)
plt.xlabel("Difference in receive times [ms]")
plt.ylabel("Density [1/ms]")
plt.tight_layout()

#based on https://timodenk.com/blog/exporting-matplotlib-plots-to-latex/
matplotlib.use("pgf")
matplotlib.rcParams.update({
        "pgf.texsystem": "pdflatex",
        "font.family": "serif",
        "text.usetex": True,
        "pgf.rcfonts": False,
        "font.size": 22
        })

plt.savefig("times_histogram.pgf")
    #print(data)


#ax.scatter(data[:,4],data[:,5],data[:,6], c=data[:,3], cmap = cm.coolwarm)
#plt.show(block=True)
