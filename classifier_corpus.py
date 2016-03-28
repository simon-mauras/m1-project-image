#!/usr/bin/env python3
import os, re, csv, subprocess

indexing_exe = "./build/shape_indexing"

def database():
  database_dir = "./database/"
  for f in os.listdir(database_dir):
    m = re.match(r"^(\w*)-(\d*).pgm$", f)
    if m is None: raise(Exception(f))
    yield(m.group(1), database_dir + f)

with open("corpus.csv", "w") as csvfile:
  writer = csv.writer(csvfile)
  for group, filename in database():
    res = subprocess.run([indexing_exe, filename], stdout=subprocess.PIPE)
    X = [ float(x) for x in res.stdout.splitlines()]
    writer.writerow([group] + X)
