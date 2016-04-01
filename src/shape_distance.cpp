#include <cmath>
#include <vector>
#include <iostream>
#include "dgtal.hh"
#include "features.cpp"

using namespace std;

double similarity(vector<double> v1, vector<double> v2)
{
  double dist = 0;
  for (int i=0; i<(int)v1.size(); i++)
    dist = max(dist, abs(v1[i] - v2[i])/(v1[i] + v2[i]));
  return 1 - dist;
}

int main(int argc, char** argv)
{
  MyImage image1 = GenericReader<MyImage>::import(argv[1]);
  MyImage image2 = GenericReader<MyImage>::import(argv[2]);
  
  vector<double> features1 = get_features(image1);
  vector<double> features2 = get_features(image2);
  cout << similarity(features1, features2) << endl;
}
