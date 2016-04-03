#include <cstdlib>
#include <iostream>
#include "dgtal.hh"

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

MyImage add_border(const MyImage &img)
{
  MyImage res(Domain(img.domain().lowerBound() - Point(1, 1),
                     img.domain().upperBound() + Point(1, 1)));
  for (auto p : img.domain())
    res.setValue(p, img(p));
  return res;
}

MyImage border(const MyImage &img)
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
  auto r = c.getPointsRange();
  
  MyImage result(img.domain());
  for (auto x : r)
    result.setValue(x, 255);
  return result;
}

double similarity_rotation(const MyImage &img, double angle, double radius)
{
  int nb = 0;
  Point med(0, 0);
  for (auto p : img.domain())
  {
    if (img(p))
    {
      nb++;
      med += p;
    }
  }
  
  int res = 0;
  med /= nb;
  for (auto p : img.domain())
  {
    if (img(p))
    {
      Point q;
      if (p == med)
        q = med;
      else
      {
        double a = atan2(p[1] - med[1], p[0] - med[0]);
        double r = (p - med).norm();
        q = med + Point(r * cos(a + angle), r * sin(a + angle));
      }
      
      bool ok = false;
      for (int i=-radius; i<=radius; i++)
        for (int j=-radius; j<=radius; j++)
          if (i*i + j*j <= radius*radius)
            if (img.domain().isInside(q + Point(i, j)))
              if (img(q + Point(i, j)))
                ok = true;
      if (ok) res++;
    }
  }
  
  return res / (double)nb;
}

MyImage fill(const MyImage &img)
{
  set<Point> background;
  queue<Point> q;
  q.push(Point(-1, -1));
  background.insert(Point(-1, -1));
  while (!q.empty())
  {
    Point act = q.front();
    q.pop();
    for (int i=-1; i<=1; i++)
    for (int j=-1; j<=1; j++)
    {
      if (abs(i)+abs(j) > 1) continue;
      Point p = act + Point(i, j);
      if (!img.domain().isInside(p)) continue;
      if (background.count(p)) continue;
      if (img(p)) continue;
      background.insert(p);
      q.push(p);
    }
  }
  
  MyImage result(img.domain());
  for (auto p : result.domain())
    if (background.count(p))
      result.setValue(p, 0);
    else
      result.setValue(p, 255);
  
  return result;
}

MyImage normalize(const MyImage &img)
{
  return fill(border(add_border(img)));
}
  

MyImage convex_hull(const MyImage &img)
{
  vector<Point> lower, upper;
  int x_min = img.domain().lowerBound()[0];
  int y_min = img.domain().lowerBound()[1];
  int x_max = img.domain().upperBound()[0] - 1;
  int y_max = img.domain().upperBound()[1] - 1;
  for (int x=x_min; x<=x_max; x++)
  {
    Point p;
    int y_lower = y_min;
    int y_upper = y_max;
    while (y_lower <= y_max && !img(Point(x, y_lower))) y_lower++;
    while (y_upper >= y_min && !img(Point(x, y_upper))) y_upper--;
    
    if (y_lower > y_max || y_upper < y_min) continue;
    
    p = Point(x, y_lower);
    while (true)
    {
      if (lower.size() < 2)
        break;
      Point a = p - lower[lower.size()-2];
      Point b = p - lower[lower.size()-1];
      int scalar = a[0] * b[1] - a[1] * b[0];
      if (scalar > 0)
        break;
      lower.pop_back();
    }
    lower.push_back(p);
    
    p = Point(x, y_upper);
    while (true)
    {
      if (upper.size() < 2)
        break;
      Point a = p - upper[upper.size()-2];
      Point b = p - upper[upper.size()-1];
      int scalar = a[0] * b[1] - a[1] * b[0];
      if (scalar < 0)
        break;
      upper.pop_back();
    }
    upper.push_back(p);
  }
  
  MyImage result(img.domain());
  int id_upper = -1, id_lower = -1;
  for (int x=x_min; x<=x_max; x++)
  {
    if (upper[id_upper+1][0] <= x) id_upper++;
    if (lower[id_lower+1][0] <= x) id_lower++;
    if (id_upper+1 == (int)upper.size()) break;
    if (id_lower+1 == (int)lower.size()) break;
    if (id_upper == -1) continue;
    if (id_lower == -1) continue;
    for (int y=y_min; y<=y_max; y++)
    {
      Point a, b, p = Point(x, y);
      a = p - upper[id_upper];
      b = p - upper[id_upper+1];
      if (a[0] * b[1] - a[1] * b[0] > 0) continue;
      a = p - lower[id_lower];
      b = p - lower[id_lower+1];
      if (a[0] * b[1] - a[1] * b[0] < 0) continue;
      result.setValue(p, 255);
    }
  }
  // For the last column...
  //for (int y=lower.back()[0]; y<=upper.back()[0]; y++)
    //result.setValue(Point(lower.back()[0], y), 255);
  
  return result;
}


double feature_dummy()
{
  return rand() / (double)RAND_MAX;
}

vector<double> compute_dt(const MyImage &img, int NB)
{
  Binarizer binarizer(img, 0);
  DistTransf dt(img.domain(), binarizer, l2Metric);
  
  vector<double> v;
  for (auto x : dt.domain())
    if (dt(x) > 0)
      v.push_back(dt(x));
  
  vector<double> res;
  if (v.empty())
  {
    for (int i=1; i<=NB; i++)
      res.push_back(0);
  }
  else
  {
    sort(v.begin(), v.end());
    for (int i=1; i<=NB; i++)
      res.push_back(v[i*(v.size()-1)/NB] / v[v.size()-1]);
  }
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
  
  MyImage image = normalize(img);
  MyImage convex = convex_hull(image);
  
  for (auto c : compute_dt(image, 15))
    result.push_back(c);
  
  for (auto c : compute_dt(convex, 15))
    result.push_back(c);
  double perimeter = compute_perimeter(image);
  double area = compute_area(image);
  double perimeter_convex = compute_perimeter(convex);
  double area_convex = compute_area(convex);
  
  result.push_back(perimeter / sqrt(area) / 15);
  result.push_back(perimeter_convex / sqrt(area_convex) / 2);
  result.push_back(area / area_convex);
  
  for (int i=2; i<=10; i++)
    result.push_back(similarity_rotation(image, 2 * M_PI / i, 3));
  
  return result;
}
