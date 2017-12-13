"""
Created on Wed Nov 29 17:27:25 2017

@author: Aishwary
"""
import numpy as np
import glob
import pandas
import math
import os
import matplotlib.pyplot as plt

def sortKeyFunc(s):
    return int(os.path.basename(s)[:-4])

mean = []
std = []
files = sorted(glob.glob('Hard_surface\*.csv'), key=sortKeyFunc)

for i in files:
    dataframe = pandas.read_csv(i, header=None)
    dataset = dataframe.values
    a_list = np.reshape(dataset, (200,1))
    i = 0
    time1 = []
    for i in range(0,len(a_list)):
        time1.append(i/10)
    mean.append(np.mean(a_list))
    std.append(np.std(a_list))

plt.plot(mean,std, color='red')
plt.title('For Hard surface: With increase in distance from ultrasonic sensor')
plt.xlabel('Mean in cm')
plt.ylabel('Standard Deviation')
plt.show()
