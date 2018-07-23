#!/usr/bin/python
#printing data instead of plotting
import subprocess
import matplotlib.pyplot as plt
import numpy as np
import os

meshes = range(10,210,10)
results = []
for mesh in meshes:
    print(float(subprocess.Popen("python topopt_zeek.py {} {} 0.4 5.4 3.0 1".format(mesh, mesh).split(),
              stdout=subprocess.PIPE).communicate()[0].decode('utf-8').split('\n')[0]))
