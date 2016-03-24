#ifndef DGTAL_HEADER
#define DGTAL_HEADER

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
//#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>
#include <DGtal/images/imagesSetsUtils/ImageFromSet.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>

using namespace std;
using namespace DGtal;
using namespace Z2i;

typedef ImageContainerBySTLVector<Z2i::Domain, unsigned char> MyImage;

#endif
