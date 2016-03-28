#include <iostream>
#include "dgtal.hh"
#include "features.cpp"

using namespace std;

int main(int argc, char **argv)
{
  MyImage img = GenericReader<MyImage>::import(argv[1]);
  
  vector<double> f = get_features(img);
  for (auto x : f)
    cout << x << endl;
  
  return 0;
}
