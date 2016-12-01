#include "showImage.h"
#include "spatialFiltering.h"

int main( int argc, char** argv ){
    Mat image=loadImage(argv[1]);
    showImage(image,"Original Image");
    waitKey(0);

    int w,h;
    cout<<"Ukuran Matriks Filter (Ganjil) (w,h)";cin>>w>>h;
    filter a =newFilter(w,h);
    fillFilter(&a);

    Mat sFed = spatialFilteringRGB(image,a);
    showImage(sFed,"Filtered");
    waitKey(0);

    return 0;
}
