#include "showImage.h"
#include "histogram.h"
#include "regionGrowing.h"
#include "angleDistance.h"
#include "angleDistance.h"

int main( int argc, char** argv ){
    Mat image=loadImage(argv[1]);
    histogram before=createHist(256),after=createHist(256);
    // showImage(image,"Base Image");
    Mat imageGr=Grayscaler(image);
    showImage(imageGr,"Grayscale");
    fillHistogram(&before,imageGr);
    statistikHistogram(before);
    drawHistogram(before,"testing");
    waitKey(0);
    Mat Seg = seedByThreshold(imageGr,otsuThresholding(before),255);
    showImage(Seg,"Otsu");
    waitKey(0);
    // Mat Bond = boundary(Seg,0);
    position tes=getCentroid(Seg,0);
    showImage(Seg,"Boundary");
    DAShistogram DH1 = DistanceAngleHistogram(Seg,0 ,72);
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
    waitKey(0);
    // std::cout << "Modus haha"<<getModus(haha) << std::endl;
    if(argc>2){
      string loadFile=argv[2];
      DAShistogram DH2=loadDAS(loadFile);
      drawHistogram(DH2,"hehe");
      std::cout << "is Same"<<isSame(DH1,DH2) << std::endl;
      std::cout << "difference" <<difference(DH1,DH2)<< std::endl;
    }

    // file<<


    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
