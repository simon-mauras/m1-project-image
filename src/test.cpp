
#include <iostream>
#include "dgtal.hh"
#include "features.cpp"

using namespace std;

MyDigitalSet border(const MyDigitalSet &set)
{
  MyObject object(dt8_4, set);
  return object.border().pointSet();
}

MyImage shift_border(MyImage img, double v)
{
  if (v < 0)
    for (auto p : img.domain())
      img.setValue(p, 255 - img(p));
  
  Binarizer binarizer(img, 0);
  DistTransf dt(img.domain(), binarizer, l2Metric);
  
  MyImage result(img.domain());
  for (auto p : img.domain())
    if (dt(p) > abs(v))
      result.setValue(p, 255);
  
  if (v < 0)
    for (auto p : img.domain())
      result.setValue(p, 255 - result(p));
  
  return result;
}

MyImage smooth(const MyImage img)
{
  map<Point, double> filter;
  filter[Point(-1, -1)] = 3./21;
  filter[Point(-1, 1)] = 3./21;
  filter[Point(1, -1)] = 3./21;
  filter[Point(1, 1)] = 3./21;
  filter[Point(0, -1)] = 2./21;
  filter[Point(0, 1)] = 2./21;
  filter[Point(-1, 0)] = 2./21;
  filter[Point(1, 0)] = 2./21;
  filter[Point(0, 0)] = 1./21;
  
  MyImage result(img.domain());
  for (auto p : img.domain())
  {
    double val = 0;
    for (auto delta : filter)
    {
      Point q = p + delta.first;
      if (!img.domain().isInside(q))
        continue;
      if (img(q))
        val += delta.second;
    }
    if (val > 0.5)
      result.setValue(p, 255);
    else
      result.setValue(p, 0);
  }
  return result;
}

MyImage medial_axis(const MyImage &img)
{
  typedef functors::SimpleThresholdForegroundPredicate<MyImage> Binarizer;
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
  auto r = c.getPointsRange();
  
  MyImage result(img.domain());
  for (auto x : r)
    result.setValue(x,255);
  return result;
}

int main(int argc, char **argv)
{
  cout << argv[1] << endl;
  
  MyImage img_in = GenericReader<MyImage>::import(argv[1]);
  
  /*
  MyDigitalSet set_in = set_of_image(img_in);
  MyDigitalSet set_out = border(biggest_component(set_in));
  MyImage img_out = image_of_set(set_out);
  //*/
  MyImage img_out = convex_hull(normalize(img_in));
  
  cout << compute_perimeter(img_out) << endl;
  
  GenericWriter<MyImage>::exportFile(argv[2], img_out);
}
