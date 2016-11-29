#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

struct MatchPathSeparator{
  bool operator()( char ch ) const{
    return ch == '/';
  }
};
// struct MatchPathSeparator{
//     bool operator()( char ch ) const{
//         return ch == '\\' || ch == '/';
//     }
// }; //for windows path

Mat loadImage(string imageDirName){
  string imageName("../data/HappyFish.jpg"); // by default
  if( imageDirName.length() > 1)
  {
      imageName = imageDirName;
  }
  Mat image;//(800, 800, CV_8UC1, Scalar(255, 255, 255));
  image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file
  if( image.empty() )                      // Check for invalid input
  {
      cout <<  "Could not open or find the image" << std::endl ;
      return image;
  }
  else return image;
}

void showImage(string name, Mat image){
  namedWindow( name, WINDOW_AUTOSIZE ); // Create a window for display.
  imshow( name, image );
}

string basename(string const& pathname ){
  return string(
    find_if(
      pathname.rbegin(), pathname.rend(),MatchPathSeparator() ).base(),
      pathname.end() );
}

int main( int argc, char** argv ){
  showImage(basename(argv[1]),loadImage(argv[1]));
  waitKey(0);

}
