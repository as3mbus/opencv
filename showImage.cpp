#include "showImage.h"

using namespace cv;
using namespace std;

std::string dirnameOf(const std::string& fname)
{
     size_t pos = fname.find_last_of("\\/");
     return (std::string::npos == pos)
         ? ""
         : fname.substr(0, pos);
}

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

Mat Grayscaler(Mat img){
    int gr_w = img.cols;
    int gr_h = img.rows;
    //cout<<hist_h<<"	"<<modus<<"	"<<bin_h;

    Mat grImage(gr_h, gr_w, CV_8UC1, Scalar(255, 255, 255));
    for(int y = 0; y < gr_h; y++){
        for(int x = 0; x < gr_w; x++){
            Point3_<uchar>* p = img.ptr<Point3_<uchar> >(y,x);
            grImage.at<uchar>(y,x)=.2126 * p->z +.7152 * p->y+.0722 * p->x;
        }
    }
    return grImage;
}

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

void showImage(Mat image, string name){
  namedWindow( name, WINDOW_AUTOSIZE ); // Create a window for display.
  imshow( name, image );
}

string basename(string const& pathname ){
  return string(
    find_if(
      pathname.rbegin(), pathname.rend(),MatchPathSeparator() ).base(),
      pathname.end() );
}

String convertIntString(int x){
  stringstream convert;
  string hasil;
  convert<<x;
  hasil=convert.str();
  return hasil;
}
