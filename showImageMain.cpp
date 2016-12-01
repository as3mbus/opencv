#include "showImage.h"

int main( int argc, char** argv ){
  showImage( loadImage(argv[1]) , basename(argv[1]) );
  waitKey(0);
}
