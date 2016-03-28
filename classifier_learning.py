#!/usr/bin/env python3
import csv, time
import numpy as np
import pickle
from sklearn import cross_validation
from sklearn.metrics import make_scorer
from sklearn.multiclass import OneVsRestClassifier
from sklearn.svm import LinearSVC
from sklearn.linear_model import LogisticRegression
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import MultinomialNB
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier

classes = list()
classId = dict()
with open("classes.csv", "r") as csvfile:
  reader = csv.reader(csvfile)
  for i,s in enumerate(reader):
    classId[s[0]] = i
    classes.append(s[0])

y = []
X = []
with open("corpus.csv", "r") as csvfile:
  reader = csv.reader(csvfile)
  for row in reader:
    y.append(classId[row[0]])
    X.append([float(x) for x in row[1::]])
X = np.array(X)
y = np.array(y)

"""
import matplotlib.pyplot as plt
for i in classId:
  c = classId[i]
  plt.plot([x[0] for (x,c2) in zip(X,y) if c2 == c], [x[1] for (x,c2) in zip(X,y) if c2 == c], "x")
plt.show()
"""

model = OneVsRestClassifier(KNeighborsClassifier(20))
model.fit(X,y)
with open("model.dump", "wb") as dumpfile:
  pickle.dump(model, dumpfile)


cv = cross_validation.KFold(n=X.shape[0], n_folds=10,
                            shuffle=True, random_state=0)

models = [
  ("KNeighbors 5", KNeighborsClassifier(5)),
  ("KNeighbors 10", KNeighborsClassifier(10)),
  ("KNeighbors 15", KNeighborsClassifier(15)),
  ("KNeighbors 20", KNeighborsClassifier(20)),
#  ("Linear SVC", LinearSVC()),
  ("Naive Bayes", MultinomialNB()),
  ("Logistic Regression", LogisticRegression()),
  ("Decision Tree", DecisionTreeClassifier(max_depth=4)),
  ("Random Forest", RandomForestClassifier(max_depth=4, n_estimators=10)),
]

def scoring(estimator, X, y):
  proba = estimator.predict_proba(X)
  score = 0
  for p, res in zip(proba, y):
    ok = { x[1] for x in sorted(zip(p, estimator.classes_))[-10:] }
    if res in ok:
      score = score + 1
  return score / len(y)

for mdl_name, mdl in models:
  print(">", mdl_name)
  t = time.time()
  model = OneVsRestClassifier(mdl)
  model.fit(X,y)
  print("Score_in =", scoring(model, X, y))
  score = np.average(cross_validation.cross_val_score(model,
                X, y, cv=cv, scoring=scoring))
  print("Score_cv =", score)
  print("Done,", time.time() - t, "s")

