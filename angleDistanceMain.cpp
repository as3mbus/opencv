#include "showImage.h"
#include "histogram2.h"
#include "angleDistance.h"
#include "imageSegmentation.h"

int main( int argc, char** argv ){
    Mat image=loadImage(argv[1]);
    histogram before=createHist(256),after=createHist(256);
    // showImage(image,"Base Image");
    Mat imageGr=Grayscaler(image);
    showImage(imageGr,"Grayscale");
    waitKey(0);
    fillHistogram(&before,imageGr);
    drawHistogram(before,"testing");
    waitKey(0);

    statistikHistogram(before);
    Mat Seg = singleThresholdSegmentation(imageGr,otsuThresholding(before),255);
    showImage(Seg,"Otsu");
    waitKey(0);
    int midObject;
    std::cout << "what middle object intensity u see ? 255 (white )/ 0 Black" << '\n';
    cin>>midObject;
    Mat Bond = boundary(Seg,midObject);
    position tes=getCentroid(Seg,midObject);
    showImage(Bond,"Boundary");
    DAShistogram DH1 = DistanceAngleHistogram(Seg,midObject ,72);
    drawHistogram(DH1,"Histo");
    string fileName=argv[1];
    fileName=+".DAS";
    saveDAS(DH1,fileName);
    waitKey(0);
    // normalize (&haha);
    // drawHistogram(haha,"histo2");
    // showImage(drawline(Seg,tes,255),"Line");
    // std::vector<double> haha2 =normalize(haha);
    // drawNormalHistogram(haha2,"Histo2");
    // showImage(boundary(Seg,0),"Testinger");
    // waitKey(0);
    // std::cout << "Modus haha"<<getModus(haha) << std::endl;
    if(argc>2){
      string loadFile=argv[2];
      DAShistogram DH2=loadDAS(loadFile);
      drawHistogram(DH2,"hehe");
      std::cout << "is Same"<<isSame(DH1,DH2) << std::endl;
      std::cout << "difference" <<difference(DH1,DH2)<< std::endl;
    }

    return 0;
}
