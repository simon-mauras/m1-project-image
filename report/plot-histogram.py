#!/usr/bin/env python3
import sys
filename = sys.argv[1]
with open(filename, "r") as f:
  data = [float(x) for x in f.read().splitlines()]
print(len(data))

import numpy as np
import matplotlib.pyplot as plt
plt.hist(data, bins=50, range=(1,100), color="white")
plt.savefig("hist.pdf")
