#ifndef OTSUTHRESHOLDING_H
#define OTSUTHRESHOLDING_H
#include "histogram.h"

int otsuThresholding(histogram h);

int * duoOtsu(histogram h);

// double ** generateVariance(histogram h,int start,int times);

// int * multiOtsu(histogram h,start,times);

#endif
