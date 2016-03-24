#include <iostream>
#include "dgtal.hh"
#include "features.cpp"

using namespace std;

MyImage image_border(const MyImage img)
{
  DigitalSet set_image(img.domain());
  SetFromImage<DigitalSet>::append<MyImage>(set_image, img, 0, 255);
  
  Object8_4 object_border(dt8_4, set_image);
  DigitalSet set_border = object_border.border().pointSet();
  
  return ImageFromSet<MyImage>::create<DigitalSet>(set_border, 255);
}

int main(int argc, char **argv)
{
  cout << argv[1] << endl;
  
  MyImage img_in = GenericReader<MyImage>::import(argv[1]);
  
  MyImage img_out = image_border(img_in);
  
  GenericWriter<MyImage>::exportFile("image.pgm", img_out);
}
