#ifndef SHOWIMAGE_H
#define SHOWIMAGE_H
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

// struct MatchPathSeparator{
//     bool operator()( char ch ) const{
//         return ch == '\\' || ch == '/';
//     }
// }; //for windows path

Mat loadImage(string imageDirName);
Mat Grayscaler(Mat img);

void showImage(Mat image, string name);

string basename(string const& pathname );
#endif
