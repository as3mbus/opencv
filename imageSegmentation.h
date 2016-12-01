#ifndef IMAGESEGMENTATION_H
#define IMAGESEGMENTATION_H

#include "showImage.h"
#include "histogram.h"

Mat singleSegmentation(Mat Img,int threshold);
Mat duoSegmentation(Mat Img,int threshold[]);
Mat imageSegmentation(Mat Img,int threshold[]);

#endif
