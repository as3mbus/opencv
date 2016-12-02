#include "showImage.h"
#include "histogram2.h"
#include "cStretching.h"

int sizeofp(int *p){
   return sizeof(p)/sizeof p[0];// you cant get the size of empty array
    //http://stackoverflow.com/questions/492384/how-to-find-the-sizeofa-pointer-pointing-to-an-array
    //array sizeof : http://stackoverflow.com/questions/1898657/result-of-sizeof-on-array-of-structs-in-c
}


int main( int argc, char** argv ){
    Mat image=loadImage(argv[1]);

    showImage( image, basename(argv[1]) );
    waitKey(0);

    histogram before=createHist(256),after=createHist(256);

    Mat imageGr=Grayscaler(image);
    showImage( imageGr, "Grayscale" );

    fillHistogram(&before,imageGr);
    statistikHistogram(before);
    drawHistogram(before,"Histogram Before");

    waitKey(0);

    Mat imageGrCS=contrastStretching2(imageGr,before);

    showImage(imageGrCS,"Contrast Stretched");

    fillHistogram(&after,imageGrCS);
    statistikHistogram(after);
    drawHistogram(after,"Histogram After");
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
