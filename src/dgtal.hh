#ifndef DGTAL_HEADER
#define DGTAL_HEADER

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/kernel/sets/DigitalSetBySTLSet.h>
#include <DGtal/kernel/BasicPointFunctors.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>
#include <DGtal/images/imagesSetsUtils/ImageFromSet.h>
#include <DGtal/images/SimpleThresholdForegroundPredicate.h>
#include <DGtal/images/ConstImageAdapter.h>
#include <DGtal/geometry/curves/FP.h>
#include <DGtal/geometry/curves/estimation/DSSLengthEstimator.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>
#include <DGtal/geometry/volumes/distance/PowerMap.h>
#include <DGtal/geometry/volumes/distance/ReducedMedialAxis.h>
#include <DGtal/topology/helpers/Surfaces.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>

using namespace std;
using namespace DGtal;
using namespace Z2i;

typedef DigitalSetBySTLSet<Domain> MyDigitalSet;
typedef ImageContainerBySTLVector<Domain, unsigned char> MyImage;
typedef Object<DT8_4, MyDigitalSet> MyObject;

typedef functors::SimpleThresholdForegroundPredicate<MyImage> Binarizer;
typedef DistanceTransformation<Space, Binarizer, L2Metric> DistTransf;

#endif
