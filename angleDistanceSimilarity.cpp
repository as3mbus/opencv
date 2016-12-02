#include "showImage.h"
#include "histogram2.h"
#include "otsuThresholding.h"
#include "imageSegmentation.h"
#include "angleDistance.h"

int main(int argc, char const *argv[]) {
  /* code */
  Mat image = loadImage(argv[1]);
  Mat imageGr=Grayscaler(image);
  histogram before=createHist(256);
  fillHistogram(&before,imageGr);
  
  // drawHistogram(before,"testing");
  // waitKey(0);

  string imageName=basename(argv[1]);
  statistikHistogram(before);
  Mat Seg = singleThresholdSegmentation(imageGr,otsuThresholding(before),255);
  showImage(Seg,"Otsu");
  waitKey(0);
  int midObject;
  std::cout << "what middle object intensity u see ? 255 (white )/ 0 Black = ";
  cin>>midObject;

  // Mat Bond = boundary(Seg,midObject);
  // showImage(Bond,"Boundary ");

  position tes=getCentroid(Seg,midObject);

  DAShistogram DH1 = DistanceAngleHistogram(Seg,midObject ,72);
  drawHistogram(DH1,"Distance Angle Histogram "+imageName);
  waitKey(0);
  if(argc>2){
    string loadFile=argv[2];
    DAShistogram DH2=loadDAS(loadFile);
    drawHistogram(DH2,basename(argv[2]));
    std::cout << "is Same = "<<isSame(DH1,DH2) << std::endl;
    std::cout << "difference = " <<differenceMean(DH1,DH2)<< std::endl;
  }
  return 0;
}
