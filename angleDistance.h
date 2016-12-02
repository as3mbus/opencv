#ifndef ANGLEDISTANCE_H
#define ANGLEDISTANCE_H

#include "showImage.h"
#include "histogram2.h"
#include "regionGrowing.h"
#include "otsuThresholding.h"
// #include <sstream>
#include <fstream>

#define RAD(theta) (3.14)*((double)theta/180.0)

// template<typename T,int N> //template argument deduction
// int size(T (&arr1)[N]) //Passing the array by reference
// {
//   size_t size;
//   size=sizeof(arr1)/sizeof(arr1[0]);
//
//   std::cout<<size<<std::endl; //Correctly prints the size of arr
//
//   //EDIT
//
//   return N; //Correctly prints the size too [cool trick ;-)]
// }

struct position{
  int x,y;
};

struct DAShistogram{
  std::vector<double> arr;
  position centroid;
  int segInt;
  int times,max,min;
};

position createPos(int x,int y);

position getCentroid(Mat Img,int segInt);
position getCentroidBoundary(Mat Img);
Mat setCentroid(Mat Img,int segInt);


int getModus(DAShistogram h);
int getModusIndeks(DAShistogram h);

void drawHistogram(DAShistogram h,string name);
void drawNormalHistogram(std::vector<double> h,string name);

Mat drawline(Mat Img,position centroid,int segInt);

int getDistance(Mat Img,position centroid,int segInt,int theta);
int getDistanceBoundary(Mat Img, position centroid, int theta);
int getDistanceTrigonometri(Mat Img,position centroid,int segInt,int theta);

DAShistogram DistanceAngleHistogram(Mat Img,int segInt,int times);
DAShistogram DistanceAngleHistogramBoundary(Mat Img,int times);

void saveDAS(DAShistogram h,string fileName);
DAShistogram loadDAS(string fileName);

bool isSame(DAShistogram a,DAShistogram b);
double difference(DAShistogram a,DAShistogram b);

std::vector<double> normalize(DAShistogram h);
void normalize(DAShistogram * h);

Mat boundary(Mat Img, int segInt);

#endif
