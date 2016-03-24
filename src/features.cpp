#include <cstdlib>
#include <iostream>
#include "dgtal.hh"

double dummy_feature(const MyImage &img)
{
  return rand() / (double)RAND_MAX;
}
