#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "showImage.h"
#include "opencv2/imgproc/imgproc.hpp"

struct histogram{
    int aray[256];
    int maks,mins;
    double avg;
    int jumlahint,jumlahpx;
};

int sizeofp(int *p);

histogram createHist(int size);
void fillHistogram(histogram *h,Mat img);
int getModus(histogram h);
double standarDeviasi(histogram h,Mat img);

double countMean(int data[],int start,int end);//end = index terakhir+1
double countVariance(int data[],int start,int end);
double countSegment(int data[],int start,int end);


void drawHistogram(histogram h,string name);
void statistikHistogram(histogram h,Mat img);
#endif
