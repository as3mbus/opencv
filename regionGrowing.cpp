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

String convertIntString(int x){
  stringstream convert;
  string hasil;
  convert<<x;
  hasil=convert.str();
  return hasil;
}

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

double countStandarDeviasi(histogram h){
  int intens;
  int mean=h.avg;
  double xx=0;
  for(int i=0;i<256;i++){
    for(int j=0;j<h.aray[i];j++){
      xx+=pow((i-h.avg),2);
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

//versi Billy
int countNumberB(histogram h,int n){
  int ST=0;
  int SDKiri= cvRound(countStandarDeviasiByThreshold(h,0,n));
  int SDKanan=cvRound(countStandarDeviasiByThreshold(h,n,255));
  int SDTotal=cvRound(countStandarDeviasi(h));
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
  return ST;
}

// Versi Kadek
int countNumberK(histogram h,int n){
  int ST=0;
  int SDKiri= cvRound(countStandarDeviasiByThreshold(h,0,n));
  int SDKanan=cvRound(countStandarDeviasiByThreshold(h,n,255));
  int SDTotal=cvRound(countStandarDeviasi(h));
  if (SDKiri==0||SDKanan==0){
    ST=cvRound(sqrt(SDKiri+SDKanan));
  }else{
    if(SDKiri>SDKanan) ST=cvRound(SDKanan);
    else ST=cvRound(SDKiri);
  }

  return ST;
}


void statistikHistogram(histogram h){
    cout<<"\nSTATISTIK GAMBAR"<<endl;
    cout<<"\nIntensitas Maksimal\t: "<<h.maks<<endl;
    cout<<"Intensitas Minimal\t: "<<h.mins<<endl;
    cout<<"Total Intensitas\t: "<<h.jumlahint<<endl;
    cout<<"Jumlah Pixel\t\t: "<<h.jumlahpx<<endl;
    cout<<"Intenstitas Rata - rata\t: "<<h.avg<<endl;
    cout<<"Standar Deviasi\t\t: "<<countStandarDeviasi(h)<<endl;
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

int input(histogram h,int n){
  int input1=0;
  std::cout << "Special Number\t:" << std::endl;
  std::cout << "-1. Modus\t:" << std::endl;
  std::cout << "-2. Otsu Number\t:" << std::endl;
  std::cout << "-3. countNumberB\t:" << std::endl;
  std::cout << "-4. countNumberK\t:" << std::endl;

  std::cin >> input1;
  switch (input1) {
    case -1:
      input1=getModusIndeks( h);
      std::cout << "modus = "<<input1 << std::endl;
      break;
    case -2:
      input1=otsuThresholding(h);
      std::cout << "Otsu number = "<<input1 << std::endl;
      break;
    case -3:
      input1=countNumberB(h,n);
      std::cout << "numberB = "<<input1 << std::endl;
      break;
    case -4:
      input1=countNumberK(h,n);
      std::cout << "numberk = "<<input1 << std::endl;
      break;
    default:
      break;
  }
  return input1;
}
int input(histogram h,int l,int u){
  int input1=0;
  std::cout << "Special Number\t:" << std::endl;
  std::cout << "-1. Modus\t:" << std::endl;
  std::cout << "-2. Otsu Number\t:" << std::endl;
  std::cout << "-3. countNumberBl\t:" << std::endl;
  std::cout << "-4. countNumberKl\t:" << std::endl;
  std::cout << "-5. countNumberBu\t:" << std::endl;
  std::cout << "-6. countNumberKu\t:" << std::endl;

  std::cin >> input1;
  switch (input1) {
    case -1:
      input1=getModusIndeks(h);
      std::cout << "modus = "<<input1 << std::endl;

      break;
    case -2:
      input1=otsuThresholding(h);
      std::cout << "Otsu number = "<<input1 << std::endl;
      break;
    case -3:
      input1=countNumberB(h,l);
      std::cout << "numberB left = "<<input1 << std::endl;
      break;
    case -4:
      input1=countNumberK(h,l);
      std::cout << "numberk left = "<<input1 << std::endl;
      break;
    case -5:
      input1=countNumberB(h,u);
      std::cout << "numberB right = "<<input1 << std::endl;
      break;
    case -6:
      input1=countNumberK(h,u);
      std::cout << "numberk right = "<<input1 << std::endl;
      break;
    default:
      break;
  }
  return input1;
}
int input(histogram h){
  int input1=0;
  std::cout << "Special Number\t:" << std::endl;
  std::cout << "-1. Modus\t:" << std::endl;
  std::cout << "-2. Otsu Number\t:" << std::endl;

  std::cin >> input1;
  switch (input1) {
    case -1:
      input1=getModusIndeks( h);
      std::cout << "modus = "<<input1 << std::endl;
      break;
    case -2:
      input1=otsuThresholding(h);
      std::cout << "Otsu number = "<<input1 << std::endl;
      break;
    default:
      break;
  }
  return input1;
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

Mat moreSeedByIntensity(Mat Img,Mat seed,int intensity,double TIns){
    //generate contrast stretched image
    int intens=0;
    Mat seed2(Img.rows, Img.cols, CV_8UC1, Scalar(0, 0, 0));;
    for(int y = 0; y < seed.rows; y++){
        for(int x = 0; x < seed.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            // std::cout << "intens = "<<(int)seed.at<uchar>(y,x) << std::endl;
            if(seed.at<uchar>(y,x)!=0){
              seed2.at<uchar>(y,x)=(int)cvRound(seed.at<uchar>(y,x));
            }
            if(intens==intensity){
              seed2.at<uchar>(y,x)=cvRound(TIns);
            }

          }
    }
    return seed2;
}

Mat moreSeedByThreshold(Mat Img,Mat seed,int lowerThreshold,int upperThreshold,double TIns){
    //generate contrast stretched image
    int intens=0;
    Mat seed2(Img.rows, Img.cols, CV_8UC1, Scalar(0, 0, 0));;
    for(int y = 0; y < seed.rows; y++){
        for(int x = 0; x < seed.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            // std::cout << "intens = "<<(int)seed.at<uchar>(y,x) << std::endl;
            if(seed.at<uchar>(y,x)!=0){
              seed2.at<uchar>(y,x)=(int)cvRound(seed.at<uchar>(y,x));
            }
            if(intens<=upperThreshold&&intens>=lowerThreshold){
              seed2.at<uchar>(y,x)=cvRound(TIns);
            }

          }
    }
    return seed2;
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

void seedGrowing4ByThreshold2(Mat seed,Mat image, int simThreshold,double tins){
  int U,ty,tx;
  while (U!=0){
    U=0;
    // std::cout << "Test" << std::endl;
    for(int y = 0; y < seed.rows; y++){
      for(int x = 0; x < seed.cols; x++){

        // intens = seed.at<uchar>(y,x);
        // std::cout << "intens = "<< intens << std::endl;
        if(seed.at<uchar>(y,x)==tins){

          // seedGrowUpByThreshold(seed,image,simThreshold,x,y);

          // std::cout << "found" << std::endl;
          ty=y;

          while (ty>0){
            if((int)seed.at<uchar>(ty-1,x)!=tins){
              if(abs((int)image.at<uchar>(ty,x)-(int)image.at<uchar>(ty-1,x))<=simThreshold){
                seed.at<uchar>(ty-1,x)=tins;
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
        if(seed.at<uchar>(y,x)==tins){

          // std::cout << "found" << std::endl;
          tx=x;
          while (tx<seed.cols-1){
            if((int)seed.at<uchar>(y,tx+1)!=tins){
              if(abs((int)image.at<uchar>(y,tx)-(int)image.at<uchar>(y,tx+1))<=simThreshold){
                seed.at<uchar>(y,tx+1)=(int)cvRound( tins );
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
        if(seed.at<uchar>(y,x)==tins){

          // seedGrowDownByThreshold(seed,image,simThreshold,x,y);

          // std::cout << "found" << std::endl;
          ty=y;
          while (ty<seed.rows-1){
            if((int)seed.at<uchar>(ty+1,x)!=tins){
              if(abs((int)image.at<uchar>(ty,x)-(int)image.at<uchar>(ty+1,x))<=simThreshold){
                seed.at<uchar>(ty+1,x)=(int)cvRound( tins );
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
        if(seed.at<uchar>(y,x)==tins){

          // std::cout << "found" << std::endl;
          tx=x;
          while (tx>0){
            if((int)seed.at<uchar>(y,tx-1)!=tins){
              if(abs((int)image.at<uchar>(y,tx)-(int)image.at<uchar>(y,tx-1))<=simThreshold){
                seed.at<uchar>(y,tx-1)=(int)cvRound( tins );
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

}

Mat regionGrowingByThreshold(Mat image, int simThreshold,int seedLowerThreshold, int seedUpperThreshold){
  Mat seed=seedByThreshold(image, seedLowerThreshold,seedUpperThreshold);
  seedGrowing4ByThreshold2(seed,image,simThreshold,255);
  return seed;
}
Mat regionGrowingByIntensity(Mat image, int simThreshold,int seedIntensity){
  Mat seed=seedByIntensity(image, seedIntensity);
  seedGrowing4ByThreshold2(seed,image,simThreshold,255);
  return seed;
}

Mat MoreRegionGrowingByIntensity(Mat image,histogram h,int times){
  int done=0;
  int simThreshold,seedintensity;
  Mat *Sed=new Mat[times];
  Mat *gro=new Mat[times];
  std::cout << "Seed Intensity\t: "<<std::endl;
  seedintensity=input(h);
  showImage(seedByIntensity(image,seedintensity),"seed ");
  waitKey(0);
  std::cout << "Simi Threshold\t: "<<std::endl;
  simThreshold=input(h,seedintensity);
  Mat Seed=regionGrowingByIntensity(image,simThreshold,seedintensity);
  gro[done]=Seed;
  showImage(gro[done],"Growth ");
  waitKey(0);
  done++;
  while (done<times){
    std::cout << "Seed Intensity\t: "<<std::endl;
    seedintensity=input(h);
    int tins=(double)(times-done+1)/(times+1)*255;
    Mat seed=moreSeedByIntensity(image,Seed,seedintensity,tins);
    Sed[done]=seed;
    showImage(Sed[done],"seed ");
    waitKey(0);
    std::cout << "Simi Threshold\t: "<<std::endl;
    simThreshold=input(h,seedintensity);
    seedGrowing4ByThreshold2(seed,image,simThreshold,tins);
    gro[done]=seed;
    showImage(gro[done],"Growth ");
    waitKey(0);
    Seed=seed;
    done++;
  }
  return Seed;
}

Mat MoreRegionGrowingByThreshold(Mat image,histogram h,int times){
  int done=0;
  int simThreshold,seedLowerThreshold,seedUpperThreshold;
  Mat *Sed=new Mat[times];
  Mat *gro=new Mat[times];
  std::cout << "Seed Upper Threshold\t: "<<std::endl;
  seedUpperThreshold=input(h);
  std::cout << "Seed Lower Threshold\t: "<<std::endl;
  seedLowerThreshold=input(h);
  showImage(seedByThreshold(image,seedLowerThreshold,seedUpperThreshold),"seed ");
  waitKey(0);
  std::cout << "Simi Threshold\t: "<<std::endl;
  simThreshold=input(h,seedLowerThreshold,seedUpperThreshold);
  Mat Seed=regionGrowingByThreshold(image,simThreshold,seedLowerThreshold,seedUpperThreshold);
  gro[done]=Seed;
  showImage(gro[done],"Growth ");
  waitKey(0);
  done++;

  while (done<times){
    std::cout << "Seed Upper Threshold\t: "<<std::endl;
    seedUpperThreshold=input(h);
    std::cout << "Seed Lower Threshold\t: "<<std::endl;
    seedLowerThreshold=input(h);
    int tins=(double)(times-done+1)/(times+1)*255;

    Mat seed=moreSeedByThreshold(image,Seed,seedLowerThreshold,seedUpperThreshold,tins);
    Sed[done]=seed;
    showImage(Sed[done],"seed ");
    waitKey(0);
    std::cout << "Simi Threshold\t: "<<std::endl;
    simThreshold=input(h,seedLowerThreshold,seedUpperThreshold);

    seedGrowing4ByThreshold2(seed,image,simThreshold,255);

    gro[done]=seed;
    showImage(gro[done],"Growth ");
    waitKey(0);
    Seed=seed;
    done++;
  }
  return Seed;
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
    // statistikHistogram(before,imageGr);
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


    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
