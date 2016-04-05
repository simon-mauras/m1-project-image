#!/usr/bin/env python3
import sys
filename = sys.argv[1]
with open(filename, "r") as f:
  data = [float(x) for x in f.read().splitlines()]
print(len(data))

import numpy as np
import matplotlib.pyplot as plt
plt.plot(range(2, 11), data, "ok")
plt.axis([2, 10, 0, 1])
plt.xlabel("k")
plt.ylabel("Similarity")
plt.savefig("rotation.pdf")
plt.show()
