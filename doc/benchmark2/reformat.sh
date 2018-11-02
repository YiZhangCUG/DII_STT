#!/bin/bash
#
data=$1

python << END
# -*- coding:utf-8 -*-

import numpy as np

id, val = np.loadtxt("${data}",unpack=True)

for i in range(len(val)):
	print("%d %.10f" % (i+1,val[i]))
pass
END