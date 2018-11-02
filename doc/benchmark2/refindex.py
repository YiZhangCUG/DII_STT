# -*-- coding:utf-8 -*-
# 
import sys
import numpy as np

index,val=np.loadtxt(sys.argv[1],unpack=True)

for i in range(len(val)):
	print("%d %.10f" % (i,val[i]))
	pass