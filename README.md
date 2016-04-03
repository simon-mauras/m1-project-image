# Installation

### Python dependancies
```
pip3 install scipy numpy sklearn --user
```

### Build
```
mkdir build
cd build
cmake .. -DDGtal_DIR=/home/dcoeurjo/DGtal/build # DGtal installation dir
```

# Similarity measure between two images

```
cp database/cattle-1.pgm tmp.pgm
./build/addNoise -i tmp.pgm -o tmp2.pgm -n 0.5
./build/shape_distance tmp.pgm tmp2.pgm
rm tmp.pgm tmp2.pgm
```
- Value between 0 and 1 (1 means "similar").
- The more this value is close to 1, the more our tool is robust (scale, rotate and noise)


# Classification evaluation

```
cp database/cattle-1.pgm tmp.pgm
./classifier_predict.py tmp.pgm > score_tmp.txt
./build/getRank score_tmp.txt 18
rm tmp.pgm score_tmp.pgm
```
- the tool outputs (standard output) a list of 70 values between 0
and 1 (classification score or probability to belong to the class)
- ```getRank``` returns the rank of a given class in a given
classification score.

# Learn from corpus
```
./classifier_corpus.py     # Build the corpus (corpus.csv)
./classifier_learning.py   # Build the model (model.dump)
```
