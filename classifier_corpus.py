#!/usr/bin/env python3
import time, os, re, csv, subprocess

indexing_exe = "./build/shape_indexing"

def database():
  database_dir = "./database/"
  files = list(os.listdir(database_dir))
  nb_files = len(files)
  for i,f in enumerate(files):
    print("\r(%d/%d)" % (i, nb_files), end="", flush=True)
    m = re.match(r"^(\w*)-(\d*).pgm$", f)
    if m is None: raise(Exception(f))
    yield(m.group(1), database_dir + f)
  print("\r" + " " * 20)

t = time.time()
with open("corpus.csv", "w") as csvfile:
  writer = csv.writer(csvfile)
  for group, filename in database():
    res = subprocess.run([indexing_exe, filename], stdout=subprocess.PIPE)
    X = [ float(x) for x in res.stdout.splitlines()]
    writer.writerow([group] + X)
print("Done,", time.time() - t, "s")
