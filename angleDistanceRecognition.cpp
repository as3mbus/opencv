#include "showImage.h"
#include "histogram2.h"
#include "otsuThresholding.h"
#include "imageSegmentation.h"
#include "angleDistance.h"


std::vector<DAShistogram> loadDASDB(string fileName){
  std::vector<DAShistogram> out;
  ifstream file;
  const char* filechar = fileName.c_str();
  file.open(filechar,ios::in);
  string a;
  while (file>>a){
    out.push_back(loadDAS(a));
  }
  file.close();
  return out;
}

std::vector<double> DASDBDifference(DAShistogram h, std::vector<DAShistogram> DASDB){
  std::vector<double> diffTable;
  double diff;
  for(int i=0;i<DASDB.size();i++){
    diff = differenceMean(h,DASDB.at(i));
    diffTable.push_back(diff);
  }
  return diffTable;
};

void evaluate(std::vector<double> table){
  int min=0;
  double diff;
  // std::cout << "diff = "<<diff << '\n';
  for (int i=0;i<table.size();i++){
    diff=table.at(i);
    std::cout << "Difference with object "<<i+1<<" = "<< diff << '\n';
    if(table.at(min)>diff)min=i;
  }
  std::cout << "Most Similar Object = Object "<<min+1 << '\n';
  std::cout << "with difference     = "<<table.at(min) << '\n';
}

int main(int argc, char const *argv[]) {
  /* code */
  Mat image = loadImage(argv[1]);
  Mat imageGr=Grayscaler(image);
  histogram before=createHist(256);
  fillHistogram(&before,imageGr);

  // drawHistogram(before,"testing");
  // waitKey(0);

  string imageName=basename(argv[1]);

  Mat Seg = singleThresholdSegmentation(imageGr,otsuThresholding(before),255);
  showImage(Seg,"Otsu");
  waitKey(0);

  int midObject;
  std::cout << "what middle object intensity u see ? 255 (white )/ 0 Black = ";
  cin>>midObject;
  // Mat Bond = boundary(Seg,midObject);
  // showImage(Bond,"Boundary ");

  position tes=getCentroid(Seg,midObject);

  std::vector<DAShistogram> DASDB;

  DAShistogram DH1 = DistanceAngleHistogram(Seg,midObject ,72);
  drawHistogram(DH1,"Distance Angle Histogram "+imageName);
  waitKey(0);
  if(argc>2){
    DASDB=loadDASDB(argv[2]);
    evaluate(DASDBDifference(DH1,DASDB));
  }
  return 0;
}
