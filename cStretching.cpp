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
struct histogram{
    int aray[256];
    int maks,mins;
    double avg;
    int jumlahint,jumlahpx;
};

histogram createHist(int size){
    histogram h;
    for(int i=0;i <size;i++){
        h.aray[i]=0;
    }
    return h;
}
void fillHistogram(histogram *h,Mat img){
    h->jumlahint=0;
    h->jumlahpx=0;
    h->maks=(int)img.at<uchar>(0,0);
    h->mins=(int)img.at<uchar>(0,0);
    int intens;
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){
            //Inisialisasi intensitas
            intens = (int)img.at<uchar>(y,x);
            //parameter rata-rata
            h->jumlahint += intens;
            h->jumlahpx++;

            //Mengganti maks dan min
            if(intens > h->maks){
                h->maks = intens;
            }else if(intens < h->mins){
                h->mins = intens;
            }

            //Menambah nilai pada histogram
            h->aray[intens]++;
        }
    }

    //menghitung rata-rata
    h->avg =(double) h->jumlahint/h->jumlahpx;
}
int getModus(histogram h){
    int maks=h.aray[0];
    for(int i=0;i <256;i++){
        if (h.aray[i] > maks) maks=h.aray[i];
    }
    return maks;
}
double standarDeviasi(histogram h,Mat img){
  int intens;
  double xx=0;
  for(int i=0;i<img.rows;i++){
    for(int j=0;j<img.cols;j++){
      intens = (int)img.at<uchar>(i,j);
      xx+=pow((intens-h.avg),2);
    }
  }
  double xy=xx/h.jumlahpx-1;
  return sqrt(xy);
}

void drawHistogram(histogram h,string name){
    int hist_w = 1280;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w/256);
    float bin_h = (float) hist_h/getModus(h);

    //cout<<hist_h<<"	"<<modus<<"	"<<bin_h;

    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    for (int i=0;i<sizeof(h.aray);i++){
      //cout <<cvRound((double)bin_h*histogram[i])<<endl;
      line(histImage,  //img
        Point(bin_w*(i), hist_h), //point a
        Point(bin_w*(i), hist_h - cvRound((double) h.aray[i]*bin_h)),//point b
        Scalar(0,0,0),//color black
        bin_w,//thickness
        8,//linetype
        0);//Number of fractional bits in the point coordinates.
    }
    showImage(histImage,name);
}
void statistikHistogram(histogram h,Mat img){
    cout<<"\nSTATISTIK GAMBAR"<<endl;
    cout<<"\nIntensitas Maksimal\t: "<<h.maks<<endl;
    cout<<"Intensitas Minimal\t: "<<h.mins<<endl;
    cout<<"Total Intensitas\t: "<<h.jumlahint<<endl;
    cout<<"Jumlah Pixel\t\t: "<<h.jumlahpx<<endl;
    cout<<"Intenstitas Rata - rata\t: "<<h.avg<<endl;
    cout<<"Standar Deviasi\t\t: "<<standarDeviasi(h,img)<<endl;
}

int sizeofp(int *p){
   return sizeof(p)/sizeof p[0];// you cant get the size of empty array
    //http://stackoverflow.com/questions/492384/how-to-find-the-sizeofa-pointer-pointing-to-an-array
    //array sizeof : http://stackoverflow.com/questions/1898657/result-of-sizeof-on-array-of-structs-in-c
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
void drawGrayscale(Mat img){
    showImage(Grayscaler(img),"greyscale testing");
}

Mat contrastStretching1(Mat grImg,histogram h){
    //generate contrast stretched image
    Mat ContStretch=grImg;//(image.rows, image.cols, CV_8UC1, Scalar(255, 255, 255));
    int divideby;
    int intens;
    cout <<"jumlah breakpoint pada histogram : ";cin>>divideby;
    int a=h.mins,b=h.maks;
    int c,d; //lowest and highest val of the section
    int divider[divideby];
    for (int i=0;i<divideby;i++){
      do {
        /* code */
        std::cout << "divider "<<i+1<<" : ";cin>>divider[i];
        if (divider[i]>255&&divider[i]<0) {
          /* code */
          std::cout << "invalid input" << std::endl;
        }
      } while(divider[i]>255&&divider[i]<0);
    }
    for(int y = 0; y < ContStretch.rows; y++){
        for(int x = 0; x < ContStretch.cols; x++){
            intens=(int)grImg.at<uchar>(y,x);
            int i=0;
            while (intens>divider[i]){
            //cout<<h <<"	"<<divider[i]<<endl;
            i++;
            }
            if (i==(int)(sizeof(divider)/4)){
              c=divider[(int)(sizeof(divider)/4)];
              d=255;
            }else if (i==0){
              c=0;
              d=divider[0];
            }else{
              c=divider[i-1];
              d=divider[i];
            }
            ContStretch.at<uchar>(y,x)=(int)cvRound( (intens-c) * ( (b-a) / (d-c) ) );
          }
    }
    return ContStretch;
}
void contrastStretching1Draw(Mat grImg,histogram h){
    showImage(contrastStretching1(grImg,h),"Contrast Stretched ");
}

Mat contrastStretching2(Mat grImg,histogram h){
    //generate contrast stretched image
    Mat ContStretch=grImg;//(image.rows, image.cols, CV_8UC1, Scalar(255, 255, 255));
    int divideby;
    int intens;
    cout <<"jumlah breakpoint pada histogram : ";cin>>divideby;
    int a,b;
    int c,d; //lowest and highest val of the section
    int divider[divideby][2];
    for (int i=0;i<divideby;i++){
      do {
        /* code */
        std::cout << "divider "<<i+1<<" (x y) : ";cin>>divider[i][0]>>divider[i][1];
        if (divider[i][0]>255&&divider[i][0]<0&&divider[i][1]>255&&divider[i][1]<0) {
          /* code */
          std::cout << "invalid input" << std::endl;
        }
      } while(divider[i][0]>255&&divider[i][0]<0&&divider[i][1]>255&&divider[i][1]<0);
    }
    for(int y = 0; y < ContStretch.rows; y++){
        for(int x = 0; x < ContStretch.cols; x++){
            intens=(int)grImg.at<uchar>(y,x);
            int i=0;
            while (intens>divider[i][0]&&i<divideby){
            //cout<<h <<"	"<<divider[i]<<endl;
            i++;
            }
            if (i==divideby){
              a=divider[divideby-1][1];
              b=255;
              c=divider[divideby-1][0];
              d=255;
            }else if (i==0){
              a=0;
              b=divider[0][1];
              c=0;
              d=divider[0][0];
            }else{
              a=divider[i-1][1];
              b=divider[i][1];
              c=divider[i-1][0];
              d=divider[i][0];
            }
            ContStretch.at<uchar>(y,x)=(int)cvRound( (intens-c) * ( (b-a) / (d-c) ) +a );
          }
    }
    return ContStretch;
}
void contrastStretching2Draw(Mat grImg,histogram h){
    showImage(contrastStretching2(grImg,h),"Contrast Stretched ");
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
    histogram before=createHist(256),after=createHist(256);
    showImage(image,"Base Image");
    Mat imageGr=Grayscaler(image);
    showImage(imageGr,"Grayscale");
    fillHistogram(&before,imageGr);
    statistikHistogram(before,imageGr);
    drawHistogram(before,"Histogram Before");
    Mat imageGrCS=contrastStretching2(imageGr,before);
    showImage(imageGrCS,"Contrast Stretched");
    fillHistogram(&after,imageGrCS);
    statistikHistogram(after,imageGrCS);
    drawHistogram(after,"Histogram After");
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
