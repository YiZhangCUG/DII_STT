# -*- coding:utf-8 -*-
# 
import sys
import numpy as np

lon,lat,ele,val,dev=np.loadtxt(sys.argv[1],unpack=True)
index,real=np.loadtxt(sys.argv[2],unpack=True)
index2,invert=np.loadtxt(sys.argv[3],unpack=True)

print("lon lat inverted real diff")
for i in range(len(index)):
	print("%.12f %.12f %.12f %.12f %.12f" % (lon[i],lat[i],invert[i],real[i],invert[i]-real[i]))
	pass