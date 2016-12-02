#include "showImage.h"
#include "histogram2.h"
#include "otsuThresholding.h"
#include "imageSegmentation.h"
#include "regionGrowing.h"

int main( int argc, char** argv ){
    Mat image=loadImage(argv[1]);
    histogram before=createHist(256),after=createHist(256);
    Mat imageGr=Grayscaler(image);

    showImage(imageGr,"Grayscale");
    fillHistogram(&before,imageGr);
    drawHistogram(before,"before");
    waitKey(0);

    int jumlahsegmentasi;
    int input=0;
    std::cout << "jumlah segmentasi : ";
    std::cin >> jumlahsegmentasi;
    std::cout << "tipe segmentasi : " << std::endl;
    std::cout << "1. Threshold" << std::endl;
    std::cout << "2. intensity" << std::endl;
    std::cin >> input;
    switch (input) {
      case 1:
        MoreRegionGrowingByThreshold(imageGr,before,jumlahsegmentasi);
        break;
      case 2:
        MoreRegionGrowingByIntensity(imageGr,before,jumlahsegmentasi);
        break;
      default:
        return 0;
    }
 // Wait for a keystroke in the window
    return 0;
}
