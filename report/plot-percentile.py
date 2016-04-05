#!/usr/bin/env python3
import sys
import matplotlib.pyplot as plt
with open("apple-dtperc.txt", "r") as f:
  y = [0.] + [float(i) for i in f.read().splitlines()]
  x = [ i / (len(y) - 1) for i in range(len(y)) ]
  plt.plot(x, y, "x", label="Apple")

with open("beetle-dtperc.txt", "r") as f:
  y = [0.] + [float(i) for i in f.read().splitlines()]
  x = [ i / (len(y) - 1) for i in range(len(y)) ]
  plt.plot(x, y, "x", label="Beetle")

with open("cattle-dtperc.txt", "r") as f:
  y = [0.] + [float(i) for i in f.read().splitlines()]
  x = [ i / (len(y) - 1) for i in range(len(y)) ]
  plt.plot(x, y, "x", label="Cattle")

plt.xlabel("Percentile")
plt.ylabel("Normalized distance")
plt.legend(loc=2)
plt.savefig("percentiles.pdf")
