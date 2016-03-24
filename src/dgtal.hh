#ifndef DGTAL_HEADER
#define DGTAL_HEADER

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>

using namespace std;
using namespace DGtal;

typedef ImageContainerBySTLVector<Z2i::Domain, unsigned char> MyImage;

#endif
