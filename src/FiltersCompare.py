# -*- coding: utf-8 -*-
"""
Created on Sun Nov 26 13:40:19 2017

@author: Nishant
"""

from scipy.signal import savgol_filter
import random as rand
import  matplotlib.pyplot as plt 
import numpy as np
import math

def apply_gf(x,y,n=8):
    y2=[]
    win=create_Gaussian_filter(n,2.5)

    for i in range(len(x)):
        w=[]
        if i>=n-1:       
            for t in range(n):
               w.append(win[t]*y[i-n+t+1])
            y2.append(sum(w))
        else:
            y2.append(y[i])
    return y2

def create_Gaussian_filter(n,sig):
    x=range(n)
    x=[i-int(n/2) for i in x]

    def Gau(x,sigm):
        pow=np.exp(-(x*x)/sigm/sigm/2)
        return (pow/np.sqrt(2*math.pi*sigm*sigm))
    x=[Gau(f,sig) for f in x]
    return x/sum(x)
    
    
def apply_wma(x,y,n=8):
    y2=[]
    
    for i in range(len(x)):
        w=[]
        if i>=n-1:
            for t in range(n):
               w.append(2*y[i-t]*(t+1)/n/(n+1))
            y2.append(sum(w))
        else:
            y2.append(y[i])
    return y2

#SG Filter function block
def SG_fil(data):
	filter_val = savgol_filter(data,7,2)
	return filter_val

Y=[]
X=np.arange(0,60,0.5)
for x in X:
    if (x>10 and x<25) or (x>25.5 and x<35.5) or x>37:
        Y.append(1+0.05*rand.random())
    else:
        Y.append(0)

Y2=apply_wma(X,Y)
Y3=apply_gf(X,Y)
Y4=SG_fil(Y)

plt.plot(X,Y,label='og')
plt.plot(X,Y2,label='filtered')
plt.plot(X,Y3,label='gf filtered')
plt.plot(X,Y4,label='sg filtered')
plt.legend(['Original values','WMA filtered values', 'Gaussian filtered values', 'SG filtered values'])
plt.show()



        
