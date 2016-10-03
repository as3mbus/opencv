#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

void showImage(Mat a,string b){
    namedWindow(b, CV_WINDOW_AUTOSIZE);
    imshow(b, a);
}

struct filter{
  int **fil;
  int filsum,width,height;
};

filter newFilter(int width,int height){
  filter a;
  if (width%2!=0&&height%2!=0){
    a.width=width;
    a.height=height;
    a.fil = new int*[height];
    for(int i=0;i<height;i++){
      a.fil[i]=new int[width];
      for (int j=0; j < width; j++) {
        a.fil[i][j]=0;
      }
    }
  }
  else{
    std::cout << "filter width & height should be odd" << std::endl;
  }
  return a;
}

void printfilter(filter f){
  for (size_t i = 0; i < f.height; i++) {
    for (size_t j = 0; j < f.width; j++) {
      /* code */
      std::cout << f.fil[i][j];
    }
    /* code */
    std::cout << std::endl;
  }
}

void fillFilter(filter *f){
  std::cout << "jumlah kolom : "<<f->width << std::endl;
  f->filsum=0;
  for(int i=0;i<f->height;i++){
    std::cout << "baris "<<i+1<<" ";
    for(int j=0;j<f->width;j++){
      std::cin >> f->fil[i][j];
      f->filsum+=f->fil[i][j];
    }
  }
  if(f->filsum==0)f->filsum++;
}


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

Mat spatialFiltering(Mat image,filter a){
  Mat Out=image;
  int outtens=0;
  int intens=0;
  int xx,yy;
  int fw=floor(a.width/2);
  int fh=floor(a.height/2);
  for(int y=0;y<image.rows;y++){
    for(int x=0;x<image.cols;x++){
      intens =0;
      for (int i = -fh; i < fh+1; i++) {
        for (int j = -fw; j < fw+1; j++) {
          if(y+i<0){
            yy=image.rows+i;
          }else if (y+i>=image.rows){
            yy=0+i;
          }else{
            yy=y+i;
          }
          if(x+j<0){
            xx=image.cols+j;
          }else if (x+j>=image.cols){
            xx=0+j;
          }else{
            xx=x+j;
          }
          intens+=((int)image.at<uchar>(yy,xx))*a.fil[fh+i][fw+j];
        }
      }
      if ((int)cvRound(intens/a.filsum)<0)
        outtens=0;
      else if((int)cvRound(intens/a.filsum)>255)
        outtens=255;
      else outtens=(int)cvRound(intens/a.filsum);
      //std::cout << "outtens"<<outtens << std::endl;
      Out.at<uchar>(y,x)=outtens;
    }
  }
  return Out;
}

Mat spatialFilteringRGB(Mat image,filter a){
  Mat Out=image;
  int outtensB=0,outtensG=0,outtensR=0;
  int intensB=0,intensG=0,intensR=0;
  int xx,yy;
  int fw=floor(a.width/2);
  int fh=floor(a.height/2);
  for(int y=0;y<image.rows;y++){
    for(int x=0;x<image.cols;x++){
      intensB =0;
      intensG = 0;
      intensR = 0;
      for (int i = -fh; i < fh+1; i++) {
        for (int j = -fw; j < fw+1; j++) {
          if(y+i<0){
            yy=image.rows+i;
          }else if (y+i>=image.rows){
            yy=0+i;
          }else{
            yy=y+i;
          }
          if(x+j<0){
            xx=image.cols+j;
          }else if (x+j>=image.cols){
            xx=0+j;
          }else{
            xx=x+j;
          }
          intensB+=((int)image.at<cv::Vec3b>(yy,xx)[0])*a.fil[fh+i][fw+j];
          intensG+=((int)image.at<cv::Vec3b>(yy,xx)[1])*a.fil[fh+i][fw+j];
          intensR+=((int)image.at<cv::Vec3b>(yy,xx)[2])*a.fil[fh+i][fw+j];
        }
      }
      if ((int)cvRound(intensB/a.filsum)<0)
        outtensB=0;
      else if((int)cvRound(intensB/a.filsum)>255)
        outtensB=255;
      else outtensB=(int)cvRound(intensB/a.filsum);

      if ((int)cvRound(intensG/a.filsum)<0)
        outtensG=0;
      else if((int)cvRound(intensG/a.filsum)>255)
        outtensG=255;
      else outtensG=(int)cvRound(intensG/a.filsum);

      if ((int)cvRound(intensR/a.filsum)<0)
        outtensR=0;
      else if((int)cvRound(intensR/a.filsum)>255)
        outtensR=255;
      else outtensR=(int)cvRound(intensR/a.filsum);

      Out.at<cv::Vec3b>(y,x)[0]=outtensB;
      Out.at<cv::Vec3b>(y,x)[1]=outtensG;
      Out.at<cv::Vec3b>(y,x)[2]=outtensR;
    }
  }
  return Out;
}

int main( int argc, char** argv ){
    string imageName("../data/HappyFish.jpg"); // by default
    if( argc > 1){
        imageName = argv[1];
    }
    Mat image;
    image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file
    if( image.empty() ){ //check invalid input;
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    showImage(image,"Original Image");
    waitKey(0);
    int w,h;
    cout<<"Ukura Matriks Filter (Ganjil) (w,h)";cin>>w>>h;
    filter a =newFilter(w,h);
    fillFilter(&a);
    Mat sFed = spatialFilteringRGB(image,a);
    showImage(sFed,"Filtered");
    waitKey(0);
    return 0;
}
