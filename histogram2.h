#ifndef HISTOGRAM2_H
#define HISTOGRAM2_H value

#include "showImage.h"
#include "opencv2/imgproc/imgproc.hpp"

struct histogram{
  std::vector<int> aray;
  int maks , mins , jumlahpx , jumlahint;
  double avg;
};

histogram createHist(int size);

void fillHistogram(histogram *h,Mat img);


int getModus(std::vector<int> data);
int getModusIndeks(std::vector<int> data);

double countStandarDeviasi(std::vector<int> data ,int lowerThreshold,int upperThreshold);
double countMean(std::vector<int> data ,int start,int end);//end = index terakhir+1
double countVariance(std::vector<int> data ,int start,int end);
double countSegment(std::vector<int> data ,int start,int end);

void drawHistogram(histogram h,string name);
void statistikHistogram(histogram h);

// Unknown Number versi Billy
int countNumberB(std::vector<int> data ,int n);
// Versi Kadek
int countNumberK(std::vector<int> data ,int n);


#endif
