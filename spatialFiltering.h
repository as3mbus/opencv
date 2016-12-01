#ifndef SPATIALFILTERING_H
#define SPATIALFILTERING_H
#include "showImage.h"

struct filter{
  int **fil;
  int filsum,width,height;
};

filter newFilter(int width,int height);
void printfilter(filter f);
void fillFilter(filter *f);

Mat spatialFiltering(Mat image,filter a);
Mat spatialFilteringRGB(Mat image,filter a);

#endif
