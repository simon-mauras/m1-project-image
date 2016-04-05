#!/usr/bin/env python3
import csv, sys, subprocess, os.path, pickle
import numpy as np

if len(sys.argv) != 2:
  print("Usage: %s <filename>" % sys.argv[0])
  exit(1)
filename = sys.argv[1]
if not os.path.isfile(filename):
  print("'%s' is not a valid filename" % filename)
  exit(1)

classes = list()
classId = dict()
with open("classes.csv", "r") as csvfile:
  reader = csv.reader(csvfile)
  for i,s in enumerate(reader):
    classId[s[0]] = i
    classes.append(s[0])

with open("model.dump", "rb") as dumpfile:
  model = pickle.load(dumpfile)

indexing_exe = "./build/shape_indexing"
res = subprocess.run([indexing_exe, filename], stdout=subprocess.PIPE)
X = [[ float(x) for x in res.stdout.splitlines() ]]

results = reversed(sorted(zip(model.predict_proba(X)[0], classes))[-10:])
for x in results:
  print(x)
