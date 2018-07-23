#!/usr/bin/python
#produces performance plotting for topopt.py with various mesh sizes
import subprocess
import matplotlib.pyplot as plt
import numpy as np
import os

meshes = range(20,100,20)
results = []
for mesh in meshes:
    result = float(subprocess.Popen("python CVXOPT.py {} {} 0.4 5.4 3.0 1".format(mesh, mesh).split(),
              stdout=subprocess.PIPE).communicate()[0].decode('utf-8').split('\n')[0])
    results.append(result)
plt.plot(meshes, results)
plt.xlabel('Mesh Size')
plt.ylabel('Time [s]')
plt.title('Performance - TOPOPT with various mesh size on ICSMASTER Cluster')
plt.legend(loc='upper right')
#plt.show()
plt.savefig("Performance.png", bbox_inches="tight")

