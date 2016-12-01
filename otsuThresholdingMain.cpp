#include "showImage.h"
#include "histogram.h"
#include "otsuThresholding.h"
#include "imageSegmentation.h"

int main( int argc, char** argv ){
  Mat image=loadImage(argv[1]);
  showImage(image,"Original Image");

  histogram before=createHist(256),after=createHist(256);

  Mat imageGr=Grayscaler(image);
  showImage(imageGr,"Grayscale");
  fillHistogram(&before,imageGr);

  statistikHistogram(before);

  waitKey(0);

  // double ** tessub1 = generateVariance(before,200,1);
  int singleThreshold=otsuThresholding(before);
  int * duoThreshold=duoOtsu(before);

  std::cout << "Single Threshold\t: "<<singleThreshold << std::endl;

  std::cout << "Dual Threshold\t\t: "<<duoThreshold[0]<<" , "<<duoThreshold[1] << std::endl;

  showImage(singleSegmentation(imageGr,singleThreshold),"Otsu 1");
  showImage(duoSegmentation(imageGr,duoThreshold),"Otsu 2");

  waitKey(0); // Wait for a keystroke in the window
  return 0;
}
