#include "showImage.h"
#include "histogram2.h"
#include "otsuThresholding.h"
#include "imageSegmentation.h"
#include <time.h>


int main( int argc, char** argv ){
  clock_t t1,t2;
  t1=clock();

  Mat image=loadImage(argv[1]);
  showImage(image,"Original Image");

  histogram before=createHist(256),after=createHist(256);

  Mat imageGr=Grayscaler(image);
  showImage(imageGr,"Grayscale");
  fillHistogram(&before,imageGr);
  waitKey(0);
  statistikHistogram(before);

  // double ** tessub1 = generateVariance(before,200,1);
  int singleThreshold=otsuThresholding(before);
  int * duoThreshold=duoOtsu(before);

  std::cout << "Single Threshold\t: "<<singleThreshold << std::endl;

  std::cout << "Dual Threshold\t\t: "<<duoThreshold[0]<<" , "<<duoThreshold[1] << std::endl;

  showImage(singleThresholdSegmentation(imageGr,singleThreshold),"Otsu 1");
  showImage(duoThresholdSegmentation(imageGr,duoThreshold),"Otsu 2");

  waitKey(0); // Wait for a keystroke in the window
  t2=clock();
  float diff ((float)t2-(float)t1);
  cout<<diff<<endl;

  return 0;
}
