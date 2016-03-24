#include <cmath>
#include <vector>
#include <iostream>
#include "dgtal.hh"
#include "features.cpp"

using namespace std;

double similarity(vector<double> v1, vector<double> v2)
{
  double res = 0;
  for (int i=0; i<(int)v1.size(); i++)
    res += (v1[i] - v2[i]) * (v1[i] - v2[i]);
  return 1 - sqrt(res / v1.size());
}

int main(int argc, char** argv)
{
  MyImage image1 = GenericReader<MyImage>::import(argv[1]);
  MyImage image2 = GenericReader<MyImage>::import(argv[2]);
  
  vector<double> features1, features2;
  features1.push_back(dummy_feature(image1));
  features2.push_back(dummy_feature(image2));
  cout << similarity(features1, features2) << endl;
}
