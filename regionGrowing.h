#ifndef REGIONGROWING_H
#define REGIONGROWING_H

#include "showImage.h"
#include "histogram.h"
#include "otsuThresholding.h"

int input(histogram h,int n);
int input(histogram h,int l,int u);
int input(histogram h);

Mat seedByThreshold(Mat Img,int lowerThreshold,int upperThreshold);
Mat seedByIntensity(Mat Img,int intensity);

Mat moreSeedByIntensity(Mat Img,Mat seed,int intensity,double TIns);
Mat moreSeedByThreshold(Mat Img,Mat seed,int lowerThreshold,int upperThreshold,double TIns);

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
