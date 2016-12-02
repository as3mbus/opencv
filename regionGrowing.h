#ifndef REGIONGROWING_H
#define REGIONGROWING_H

#include "showImage.h"
#include "histogram2.h"
#include "otsuThresholding.h"
#include "imageSegmentation.h"

int input(histogram h,int n);
int input(histogram h,int l,int u);
int input(histogram h);

void seedGrowUpByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);
void seedGrowDownByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);
void seedGrowLeftByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);
void seedGrowRightByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);
void seedGrowing4ByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);

void seedGrowing4ByThreshold2(Mat seed,Mat image, int simThreshold,double tins);

Mat regionGrowingByThreshold(Mat image, int simThreshold,int seedLowerThreshold, int seedUpperThreshold);
Mat regionGrowingByIntensity(Mat image, int simThreshold,int seedIntensity);

Mat MoreRegionGrowingByIntensity(Mat image,histogram h,int times);
Mat MoreRegionGrowingByThreshold(Mat image,histogram h,int times);

#endif
