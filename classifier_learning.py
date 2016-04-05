#!/usr/bin/env python3
import csv, time
import numpy as np
import pickle
from sklearn import cross_validation, grid_search
from sklearn.metrics import make_scorer
from sklearn.multiclass import OneVsRestClassifier
from sklearn.svm import LinearSVC
from sklearn.linear_model import LogisticRegression
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
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

model = OneVsRestClassifier(KNeighborsClassifier(25))
model.fit(X,y)
with open("model.dump", "wb") as dumpfile:
  pickle.dump(model, dumpfile)

def scoring(estimator, X, y):
  proba = estimator.predict_proba(X)
  score = 0
  max_rank = 10
  for p, res in zip(proba, y):
    ok = { x[1] for x in sorted(zip(p, estimator.classes_))[-max_rank:] }
    if res in ok:
      score = score + 1
  return score / len(y)

cv = cross_validation.KFold(n=X.shape[0], n_folds=10,
                            shuffle=True, random_state=0)

"""
for i in range(1, 10, 1):
  X2 = np.copy(X)
  X2[:,-9:] *= i/10
  model.fit(X2,y)
  print("Score_in =", scoring(model, X2, y))
  score = np.min(cross_validation.cross_val_score(model,
                X2, y, cv=cv, scoring=scoring))
  print("Score_cv =", score)
"""

"""
params = {"estimator__n_neighbors" : list(range(1, 60))}
res = grid_search.GridSearchCV(model,params,scoring=scoring,n_jobs=-1,cv=cv)
res.fit(X, y)
print(res.best_score_)
print(res.best_params_)
"""

models = [
  ("KNeighbors 1", KNeighborsClassifier(1)),
  ("KNeighbors 25", KNeighborsClassifier(25)),
  ("KNeighbors 50", KNeighborsClassifier(50)),
  ("Gaussian Naive Bayes", GaussianNB()),
  ("Logistic Regression", LogisticRegression()),
  #("Decision Tree", DecisionTreeClassifier(max_depth=4)),
  ("Random Forest", RandomForestClassifier(max_depth=4, n_estimators=10)),
]

for mdl_name, mdl in models:
  print(">", mdl_name)
  t = time.time()
  model = OneVsRestClassifier(mdl)
  model.fit(X,y)
  print("Score_in =", scoring(model, X, y))
  res_cv = cross_validation.cross_val_score(model, X, y, cv=cv, scoring=scoring)
  print("Score_cv(avg) =", np.average(res_cv))
  print("Score_cv(min) =", np.min(res_cv))
  print("Done,", time.time() - t, "s")

