#include <cmath>
#include <vector>
#include <iostream>
#include "features.cpp"

using namespace std;

double dist(vector<double> v1, vector<double> v2)
{
  double res = 0;
  for (int i=0; i<v1.size(); i++)
    res += (v1[i] - v2[i]) * (v1[i] - v2[i]);
  return sqrt(res);
}

int main(int argc, char** argv)
{
  MyImage image1 = GenericReader<MyImage>::import(argv[1]);
  MyImage image2 = GenericReader<MyImage>::import(argv[2]);
  
  vector<double> features1, features2;
  features1.push_back(dummy_feature(image1));
  features2.push_back(dummy_feature(image2));
  cout << dist(features1, features2) << endl;
}
