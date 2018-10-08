# -*- coding: utf-8 -*-
"""
Created on Wed Sep 19 22:45:56 2018

@author: Rishav
"""

from data_read import readfile
import os
import numpy as np
from sklearn.externals import joblib
root="Check"
X=[]
for (dirpath,dirnames,filenames) in os.walk(root):
    break
for i in filenames:
    X.append(np.array(readfile(root+os.sep+i)))
X=np.array(X)
joblib.dump(X,'CHECKING.pkl')
#%%
X=joblib.load("CHECKING.pkl")
#%%
from sklearn.preprocessing import scale
X_reshaped=[]
for i in X:
    X_reshaped.append(i.reshape(i.shape[1],i.shape[0]))
import matplotlib.pyplot as plt

#%%
for j in range(30):
    Xd=[]
    for i in X_reshaped:
        Xd.append(scale(i[j].reshape(len(i[j]),1)))
    Xd=np.array(Xd)
    plt.figure(figsize=(40,20))
    for i in Xd:
        plt.plot(i)
    plt.savefig('HELLO{name}.jpg'.format(name=str(j)))
    plt.show()