#include <cstdlib>
#include <iostream>
#include "dgtal.hh"

MyImage add_border(const MyImage &img)
{
  MyImage res(Domain(img.domain().lowerBound() - Point(1, 1),
                     img.domain().upperBound() + Point(1, 1)));
  for (auto p : img.domain())
    res.setValue(p, img(p));
  return res;
}

MyDigitalSet set_of_image(const MyImage &img)
{
  MyDigitalSet result(img.domain());
  SetFromImage<MyDigitalSet>::append<MyImage>(result, img, 0, 255);
  return result;
}

MyImage image_of_set(const MyDigitalSet &set)
{
  return ImageFromSet<MyImage>::create<MyDigitalSet>(set, 255);
}

double feature_dummy()
{
  return rand() / (double)RAND_MAX;
}

vector<double> compute_dt(const MyImage &img)
{
  Binarizer binarizer(img, 0);
  DistTransf dt(img.domain(), binarizer, l2Metric);
  
  vector<double> v;
  for (auto x : dt.domain())
    if (dt(x) > 0)
      v.push_back(dt(x));
  sort(v.begin(), v.end());
  
  const int NB = 10;
  vector<double> res;
  for (int i=1; i<=NB; i++)
    res.push_back(v[i*(v.size()-1)/NB] / v[v.size()-1]);
  
  return res;
}

double compute_area(const MyImage &img)
{
  double result = 0;
  for (auto x : img.domain())
    if (img(x))
      result++;
  return result;
}

double compute_perimeter(const MyImage &img)
{
  Binarizer binarizer(img, 0);
  
  KSpace ks;
  ks.init(img.domain().lowerBound(), img.domain().upperBound(), true);
  
  SurfelAdjacency<2> sAdj(true);
  
  vector<vector<SCell> > contours;
  Surfaces<KSpace>::extractAll2DSCellContours(contours, ks, sAdj, binarizer);
  
  int maxi = 0;
  for (int id=0; id<(int)contours.size(); id++)
    if (contours[id].size() > contours[maxi].size())
      maxi = id;
  
  Curve c;
  c.initFromSCellsVector(contours.at(maxi));
  typedef Curve::PointsRange Range;
  Range r = c.getPointsRange();
  
  DSSLengthEstimator<Range::ConstCirculator> DSSlength;
  DSSlength.init(1, r.c(), r.c());
  return DSSlength.eval();
}

vector<double> get_features(const MyImage &img)
{
  vector<double> result;
  
  MyImage image = add_border(img);
  
  vector<double> constant;
  for (auto c : compute_dt(image))
    constant.push_back(c);
  vector<double> linear;
  linear.push_back(compute_perimeter(image)/20);
  vector<double> quadratic;
  quadratic.push_back(compute_area(image));
  
  for (auto q : quadratic)
    linear.push_back(sqrt(q));
  for (int i=0; i<(int)linear.size(); i++)
    for (int j=i+1; j<(int)linear.size(); j++)
      constant.push_back(linear[i] / linear[j]);
  for (auto c : constant)
    result.push_back(c);
  
  return result;
}
