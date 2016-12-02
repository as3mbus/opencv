#ifndef IMAGESEGMENTATION_H
#define IMAGESEGMENTATION_H

#include "showImage.h"
#include "imageSegmentation.h"

int sizeofp(int *p);

Mat singleThresholdSegmentation(Mat Img,int lowerThreshold,int upperThreshold);

Mat singleThresholdSegmentation(Mat Img,int threshold);
Mat duoThresholdSegmentation(Mat Img,int threshold[]);
Mat imageThresholdSegmentation(Mat Img,int threshold[]);
Mat moreThresholdSegmentation(Mat Img,Mat seed,int lowerThreshold,int upperThreshold,double TIns);

Mat singleIntensitySegmentation(Mat Img,int intensity);
Mat moreIntensitySegmentation(Mat Img,Mat seed,int intensity,double TIns);

#endif
