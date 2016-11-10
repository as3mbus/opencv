#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

void seedGrowing4ByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);
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
int getModusIndeks(histogram h){
    int maks=0;
    for(int i=0;i <256;i++){
        if (h.aray[i] >h.aray[maks]) maks=i;
    }
    return maks;
}

double countMean(int data[],int start,int end){//end = index terakhir+1
  int sum=0;
  int count=0;
  for(int i= start;i<end;i++){
    count=count+i*data[i];
    sum=sum+data[i];
  }
  if (sum!=0)
    return (double) count/sum;
  else
    return 0;
}
double countVariance(int data[],int start,int end){
  int mean=countMean(data,start,end);
  double sum=0;
  int n=0;
  for(int i= start;i<end;i++){
    for(int j=0;j<data[i];j++){
      sum=sum+(pow((i - mean),2));
      n++;
    }
  }
  if (n!=0)
    return (double) sum/n;
  else
    return 0;
}
double countSegment(int data[],int start,int end){
  double sum=0;
  for(int i= start;i<end;i++){
    sum+=data[i];
  }
  return sum;
}

double countStandarDeviasi(histogram h,Mat img){
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
double countStandarDeviasiByThreshold(histogram h,int lowerThreshold,int upperThreshold){
    int mean=countMean(h.aray,lowerThreshold,upperThreshold);
    // std::cout << "mean = "<<mean << std::endl;
    double sum=0;
    int n=0;
    for(int i= lowerThreshold;i<upperThreshold;i++){
      for(int j=0;j<h.aray[i];j++){
        sum=sum+(pow((i - mean),2));
        n++;
      }
      // std::cout << "pow = " <<(pow((data[i]-mean),2))<< std::endl;
    }
    // std::cout << "sum = "<<sum << std::endl;
    // std::cout << "end-start = "<<end-start << std::endl;
    // std::cout << "varaaa = "<<sum/(end - start) << std::endl;
    // std::cout << "n = "<<n << std::endl;
    if (n!=0)
      return (double) sqrt(sum/n);
    else
      return 0;
}

void statistikHistogram(histogram h,Mat img){
    cout<<"\nSTATISTIK GAMBAR"<<endl;
    cout<<"\nIntensitas Maksimal\t: "<<h.maks<<endl;
    cout<<"Intensitas Minimal\t: "<<h.mins<<endl;
    cout<<"Total Intensitas\t: "<<h.jumlahint<<endl;
    cout<<"Jumlah Pixel\t\t: "<<h.jumlahpx<<endl;
    cout<<"Intenstitas Rata - rata\t: "<<h.avg<<endl;
    cout<<"Standar Deviasi\t\t: "<<countStandarDeviasi(h,img)<<endl;
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

Mat seedByThreshold(Mat Img,int lowerThreshold,int upperThreshold){
    //generate contrast stretched image
    int intens=0;
    Mat seed(Img.rows, Img.cols, CV_8UC1, Scalar(255, 255, 255));
    for(int y = 0; y < seed.rows; y++){
        for(int x = 0; x < seed.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            if(intens<=upperThreshold&&intens>=lowerThreshold){
              seed.at<uchar>(y,x)=(int)cvRound( 255 );
            }
            else{
              seed.at<uchar>(y,x)=(int)cvRound( 0 );
            }
          }
    }
    return seed;
}

Mat seedByIntensity(Mat Img,int intensity){
    //generate contrast stretched image
    int intens=0;
    Mat seed(Img.rows, Img.cols, CV_8UC1, Scalar(255, 255, 255));
    for(int y = 0; y < seed.rows; y++){
        for(int x = 0; x < seed.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            if(intens==intensity){
              seed.at<uchar>(y,x)=(int)cvRound( 255 );
            }
            else{
              seed.at<uchar>(y,x)=(int)cvRound( 0 );
            }
          }
    }
    return seed;
}

void seedGrowUpByThreshold(Mat seed,Mat image, int simThreshold,int x,int y){
  if(y>0){
    if((int)seed.at<uchar>(y-1,x)!=255){
      if(abs((int)image.at<uchar>(y,x)-(int)image.at<uchar>(y-1,x))<=simThreshold){
        seed.at<uchar>(y-1,x) = (int)cvRound( 255 );
        seedGrowUpByThreshold(seed,image,simThreshold,x,y-1);
        // std::cout << "Grow up " << std::endl;
      }
    }
  }
}
void seedGrowDownByThreshold(Mat seed,Mat image, int simThreshold,int x,int y){
  if(y<seed.rows-1){
    if((int)seed.at<uchar>(y+1,x)!=255){
      if(abs((int)image.at<uchar>(y,x)-(int)image.at<uchar>(y+1,x))<=simThreshold){
        seed.at<uchar>(y+1,x) = (int)cvRound( 255 );
        seedGrowDownByThreshold(seed,image,simThreshold,x,y+1);
        // std::cout << "Grow Down " << std::endl;
      }
    }
  }
}
void seedGrowLeftByThreshold(Mat seed,Mat image, int simThreshold,int x,int y){
  if(x>0){
    if((int)seed.at<uchar>(y,x-1)!=255){
      if(abs((int)image.at<uchar>(y,x)-(int)image.at<uchar>(y,x-1))<=simThreshold){
        seed.at<uchar>(y,x-1)=(int)cvRound( 255 );
        seedGrowLeftByThreshold(seed,image,simThreshold,x-1,y);
      }
    }
  }
}
void seedGrowRightByThreshold(Mat seed,Mat image, int simThreshold,int x,int y){
  if(x<seed.cols){
    if((int)seed.at<uchar>(y,x+1)!=255){
      if(abs((int)image.at<uchar>(y,x)-(int)image.at<uchar>(y,x+1))<=simThreshold){
        seed.at<uchar>(y,x+1)=(int)cvRound( 255 );
        seedGrowRightByThreshold(seed,image,simThreshold,x+1,y);
      }
    }
  }
}

void seedGrowing4ByThreshold(Mat seed,Mat image, int simThreshold,int x,int y){
  // std::cout << "position = "<< x<<", "<<y << std::endl;

  seedGrowUpByThreshold(seed,image,simThreshold,x,y);
  seedGrowRightByThreshold(seed,image,simThreshold,x,y);
  seedGrowDownByThreshold(seed,image,simThreshold,x,y);
  seedGrowLeftByThreshold(seed,image,simThreshold,x,y);

}

void seedGrowing4ByThreshold2(Mat seed,Mat image, int simThreshold,int x,int y){
  int U=0,ox=x,oy=y;
  while (y>0){
    if((int)seed.at<uchar>(y-1,x)!=255){
      if(abs((int)image.at<uchar>(y,x)-(int)image.at<uchar>(y-1,x))<=simThreshold){
        seed.at<uchar>(y-1,x)=(int)cvRound( 255 );
        U++;
      }
      else break;
    }
    else break;
  }
  int R=0,D=0,L=0;
  std::cout << "next" << std::endl;
  for(int i=0;U-i>0;i++){
    while(x<seed.cols){
      if((int)seed.at<uchar>(y+U-i,x+1)!=255){
        if(abs((int)image.at<uchar>(y,x)-(int)image.at<uchar>(y,x+1))<=simThreshold){
          seed.at<uchar>(y,x+1)=(int)cvRound( 255 );
          R++;
        }
      }
    }
  }

}

Mat regionGrowingByThreshold(Mat image, int simThreshold,int seedlowerThreshold, int seedUpperThreshold){
  Mat seed=seedByThreshold(image, seedlowerThreshold,seedUpperThreshold);
  std::cout << "rows,cols = "<<seed.rows<<", "<<seed.cols << std::endl;
  int U=1,R=1,D=1,L=1;
  int intens;
  int tx,ty;
  while (U!=0){
    U=0;
    std::cout << "Test" << std::endl;
    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        // intens = seed.at<uchar>(y,x);
        // std::cout << "intens = "<< intens << std::endl;
        if(seed.at<uchar>(y,x)==255){

          // seedGrowUpByThreshold(seed,image,simThreshold,x,y);

          // std::cout << "found" << std::endl;
          ty=y;

          while (ty>0){
            if((int)seed.at<uchar>(ty-1,x)!=255){
              if(abs((int)image.at<uchar>(ty,x)-(int)image.at<uchar>(ty-1,x))<=simThreshold){
                seed.at<uchar>(ty-1,x)=(int)cvRound( 255 );
                ty--;
                U++;
              }
              else ty=0;
            }
            else ty=0;
          }

        }
      }
    }


    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        // intens = seed.at<uchar>(y,x);
        // std::cout << "intens = "<< intens << std::endl;
        if(seed.at<uchar>(y,x)==255){

          // std::cout << "found" << std::endl;
          tx=x;
          while (tx<seed.cols-1){
            if((int)seed.at<uchar>(y,tx+1)!=255){
              if(abs((int)image.at<uchar>(y,tx)-(int)image.at<uchar>(y,tx+1))<=simThreshold){
                seed.at<uchar>(y,tx+1)=(int)cvRound( 255 );
                tx++;
                U++;
              }
              else tx=seed.cols;
            }
            else tx=seed.cols;
          }

        }
      }
    }

    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        // intens = seed.at<uchar>(y,x);
        // std::cout << "intens = "<< intens << std::endl;
        if(seed.at<uchar>(y,x)==255){

          // seedGrowDownByThreshold(seed,image,simThreshold,x,y);

          // std::cout << "found" << std::endl;
          ty=y;
          while (ty<seed.rows-1){
            if((int)seed.at<uchar>(ty+1,x)!=255){
              if(abs((int)image.at<uchar>(ty,x)-(int)image.at<uchar>(ty+1,x))<=simThreshold){
                seed.at<uchar>(ty+1,x)=(int)cvRound( 255 );
                ty++;
                U++;
              }
              else ty=seed.rows;
            }
            else ty=seed.rows;
          }


        }
      }
    }

    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        // intens = seed.at<uchar>(y,x);
        // std::cout << "intens = "<< intens << std::endl;
        if(seed.at<uchar>(y,x)==255){

          // std::cout << "found" << std::endl;
          tx=x;
          while (tx>0){
            if((int)seed.at<uchar>(y,tx-1)!=255){
              if(abs((int)image.at<uchar>(y,tx)-(int)image.at<uchar>(y,tx-1))<=simThreshold){
                seed.at<uchar>(y,tx-1)=(int)cvRound( 255 );
                tx--;
                U++;
              }
              else tx=0;
            }
            else tx=0;
          }

        }
      }
    }


  }
  return seed;
}
Mat regionGrowingByIntensity(Mat image, int simThreshold,int seedIntensity){
  Mat seed=seedByIntensity(image, seedIntensity);
  std::cout << "rows,cols = "<<seed.rows<<", "<<seed.cols << std::endl;
  int U=1;
  int intens;
  int tx,ty;
  while (U!=0){
    U=0;
    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        if(seed.at<uchar>(y,x)==255){

          ty=y;

          while (ty>0){
            if((int)seed.at<uchar>(ty-1,x)!=255){
              if(abs((int)image.at<uchar>(ty,x)-(int)image.at<uchar>(ty-1,x))<=simThreshold){
                seed.at<uchar>(ty-1,x)=(int)cvRound( 255 );
                ty--;
                U++;
              }
              else ty=0;
            }
            else ty=0;
          }

        }
      }
    }


    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        if(seed.at<uchar>(y,x)==255){

          tx=x;

          while (tx<seed.cols-1){
            if((int)seed.at<uchar>(y,tx+1)!=255){
              if(abs((int)image.at<uchar>(y,tx)-(int)image.at<uchar>(y,tx+1))<=simThreshold){
                seed.at<uchar>(y,tx+1)=(int)cvRound( 255 );
                tx++;
                U++;
              }
              else tx=seed.cols;
            }
            else tx=seed.cols;
          }

        }
      }
    }

    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        if(seed.at<uchar>(y,x)==255){

          ty=y;

          while (ty<seed.rows-1){
            if((int)seed.at<uchar>(ty+1,x)!=255){
              if(abs((int)image.at<uchar>(ty,x)-(int)image.at<uchar>(ty+1,x))<=simThreshold){
                seed.at<uchar>(ty+1,x)=(int)cvRound( 255 );
                ty++;
                U++;
              }
              else ty=seed.rows;
            }
            else ty=seed.rows;
          }


        }
      }
    }

    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        if(seed.at<uchar>(y,x)==255){

          tx=x;

          while (tx>0){
            if((int)seed.at<uchar>(y,tx-1)!=255){
              if(abs((int)image.at<uchar>(y,tx)-(int)image.at<uchar>(y,tx-1))<=simThreshold){
                seed.at<uchar>(y,tx-1)=(int)cvRound( 255 );
                tx--;
                U++;
              }
              else tx=0;
            }
            else tx=0;
          }

        }
      }
    }

  }
  return seed;
}

// Mat MultiRegionGrowing(){
//   return 0;
// }

int otsuThresholding(histogram h){
  double var[254];
  double vki,vka;
  int minindeks=0;
  for(int i=0;i <254;i++){
    var[i]=0;
  }
  int i=0;
  for(i;i<254;i++){

    vki=0,vka=0;
    vki=countVariance(h.aray,0,i+1)*countSegment(h.aray,0,i+1)/h.jumlahpx;
    vka=countVariance(h.aray,i+1,256)*countSegment(h.aray,i+1,256)/h.jumlahpx;
    var[i]=vki+vka;
    if (var[minindeks]>var[i]){
      minindeks=i;
    }
    // std::cout << "var["<<i<<"] = "<<var[i] << std::endl;
  }
  // std::cout << "minindeks = "<< minindeks << std::endl;
  return minindeks+1;
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
    // showImage(image,"Base Image");
    Mat imageGr=Grayscaler(image);
    showImage(imageGr,"Grayscale");
    fillHistogram(&before,imageGr);
    statistikHistogram(before,imageGr);
    waitKey(0);

    int input1=0;
    std::cout << "\n\nselect Seed Method\t: " << std::endl;
    std::cout << "1. Modus Intensity " << std::endl;
    std::cout << "2. Otsu Thresholding " << std::endl;
    std::cout << "3. Manual " << std::endl;
    std::cin >> input1;
    int SLT,SUT,ST;
    Mat imageSeed;
    if (input1==1){
      imageSeed = seedByIntensity(imageGr,getModusIndeks(before));
    }else if(input1==2){
      SLT=otsuThresholding(before);
      SUT=255;
      imageSeed = seedByThreshold(imageGr,SLT,SUT);
    }else if (input1==3) {
      std::cout << "Seed lower Threshold\t: ";
      std::cin >> SLT;
      std::cout << "Seed upper Threshold\t: ";
      std::cin >> SUT;
      imageSeed = seedByThreshold(imageGr,SLT,SUT);
    }else return 0;

    showImage(imageSeed,"Seed");
    waitKey(0);
    int input2=0;
    std::cout << "\n\nselect Similarity Threshold Method\t: " << std::endl;
    std::cout << "1. Automatic (Experimental) " << std::endl;
    std::cout << "2. Manual " << std::endl;
    std::cin >> input2;

    int SDKiri= cvRound(countStandarDeviasiByThreshold(before,0,getModusIndeks(before)));
    int SDKanan=cvRound(countStandarDeviasiByThreshold(before,getModusIndeks(before),255));
    int SDTotal=cvRound(countStandarDeviasi(before,imageGr));
    std::cout << "SDKiri\t : "<<SDKiri << std::endl;
    std::cout << "SDKanan\t : "<<SDKanan << std::endl;
    std::cout << "SDTotal\t: "<<SDTotal  << std::endl;

    switch (input2) {
      case 1:


        // // Versi Kadek
        // if (SDKiri==0||SDKanan==0){
        //   ST=cvRound(sqrt(SDKiri+SDKanan));
        // }else{
        //   if(SDKiri>SDKanan) ST=cvRound(SDKanan);
        //   else ST=cvRound(SDKiri);
        // }

        // versi ku
        if (SDKiri==0||SDKanan==0){
          if(SDKiri>SDKanan){
            if (SDTotal>SDKiri)
              ST=cvRound(SDTotal-SDKiri);
            else
              ST=cvRound(SDKiri-SDTotal);
          }

          else {
            if (SDTotal>SDKanan)
              ST=cvRound(SDTotal-SDKanan);
            else
              ST=cvRound(SDKanan-SDTotal);
          }

        }else{
          if(SDKiri>SDKanan) ST=cvRound(SDKanan);
          else ST=cvRound(SDKiri);
        }
        std::cout << "ST = "<<ST << std::endl;
        break;
      case 2:
        std::cout << "Similarity Threshold\t: ";
        std::cin >> ST;
        break;
      default:
        return 0;
    }

    Mat test;
    switch (input1) {
      case 1:
        test=regionGrowingByIntensity(imageGr,ST,getModusIndeks(before ));
        break;
      case 2:case 3:
        test=regionGrowingByThreshold(imageGr,ST,SLT,SUT);
        break;
    }




    showImage(test,"seedafter");

    // fillHistogram(&after,imageGrCS);
    // statistikHistogram(after,imageGrCS);
    // drawHistogram(after,"Histogram After");
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
