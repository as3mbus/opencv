#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#define RAD(theta) (3.14)*((double)theta/180.0)

template<typename T,int N>
//template argument deduction
int size(T (&arr1)[N]) //Passing the array by reference
{
  size_t size;
  size=sizeof(arr1)/sizeof(arr1[0]);

  std::cout<<size<<std::endl; //Correctly prints the size of arr

  //EDIT

  return N; //Correctly prints the size too [cool trick ;-)]
}
using namespace cv;
using namespace std;

struct position{
  int x,y;
};

struct DAShistogram{
  std::vector<double> arr;
  position centroid;
  int segInt;
  int times,max,min;
};

struct histogram{
  int * aray;
  int maks,mins;
  double avg;
  int jumlahint,jumlahpx;
};

position createPos(int x,int y){
  position a;
  a.x=x;
  a.y=y;
  return a;
}

void seedGrowing4ByThreshold(Mat seed,Mat image, int simThreshold,int x,int y);
position getCentroid(Mat Img,int segInt);
position getCentroidBoundary(Mat Img);
int getDistance(Mat Img,position centroid,int segInt,int theta);

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

histogram createHist(int size){
    histogram h;
    h.aray=new int[size];
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
int getModus(DAShistogram h){
    int maks=h.arr.at(0);
    // std::vector<int>::iterator iter;
    // for(iter=h.arr.begin();iter !=h.arr.end();iter++){
    //     std::cout << "iter"<< *iter << std::endl;
    //     if (*iter > maks) maks=*iter ;
    // }
    for(int i=0;i<h.arr.size();i++){
      if (h.arr.at(i) > maks) maks=h.arr.at(i);
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
int getModusIndeks(DAShistogram h){
    int maks=0;
    for(int i=0;i <h.arr.size();i++){
        if (h.arr.at(i) >h.arr.at(maks)) maks=i;
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
    for (int i=0;i<256;i++){
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

void drawHistogram(DAShistogram h,string name){
  int hist_w = 1280;
  int hist_h = 400;
  int bin_w = cvRound((double) hist_w/h.arr.size());
  float bin_h = (float) hist_h/getModus(h);

  //cout<<hist_h<<"	"<<modus<<"	"<<bin_h;

  Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
  for (int i=0;i<h.arr.size();i++){
    //cout <<cvRound((double)bin_h*histogram[i])<<endl;
    line(histImage,  //img
      Point(bin_w*(i), hist_h), //point a
      Point(bin_w*(i), hist_h - cvRound((double) h.arr.at(i)*bin_h)),//point b
      Scalar(0,0,0),//color black
      bin_w,//thickness
      8,//linetype
      0);//Number of fractional bits in the point coordinates.
  }
  showImage(histImage,name);
}

void drawNormalHistogram(std::vector<double> h,string name){
  int hist_w = 1280;
  int hist_h = 400;
  int bin_w = cvRound((double) hist_w/h.size());
  float bin_h = (float) hist_h;

  //cout<<hist_h<<"	"<<modus<<"	"<<bin_h;

  Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
  for (int i=0;i<h.size();i++){
    //cout <<cvRound((double)bin_h*histogram[i])<<endl;
    line(histImage,  //img
      Point(bin_w*(i), hist_h), //point a
      Point(bin_w*(i), hist_h - cvRound((double) h.at(i)*bin_h)),//point b
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

Mat setCentroid(Mat Img,int segInt){
  int intens=0;
  int xc=0,yc=0,n=0;
  for(int y = 0; y < Img.rows; y++){
      for(int x = 0; x < Img.cols; x++){
          intens=(int)Img.at<uchar>(y,x);
          if(intens==segInt){
            xc+=x;
            yc+=y;
            n++;
          }
        }
  }
  xc=xc/n;
  yc=yc/n;

  Img.at<uchar>(yc,xc)=0;
  return Img;
}
position getCentroid(Mat Img,int segInt){
  position a;
  int intens=0;
  int xc=0,yc=0,n=0;
  for(int y = 0; y < Img.rows; y++){
      for(int x = 0; x < Img.cols; x++){
          intens=(int)Img.at<uchar>(y,x);
          if(intens==segInt){
            xc+=x;
            yc+=y;
            n++;
          }
        }
  }
  a.x=xc/n;
  a.y=yc/n;
  return a;
}
position getCentroidBoundary(Mat Img){
  position a;
  int intens=0;
  int xc=0,yc=0,n=0;
  for(int y = 0; y < Img.rows; y++){
      for(int x = 0; x < Img.cols; x++){
          intens=(int)Img.at<uchar>(y,x);
          if(intens==255){
            xc+=x;
            yc+=y;
            n++;
          }
        }
  }
  a.x=xc/n;
  a.y=yc/n;
  return a;
}

Mat drawline(Mat Img,position centroid,int segInt){
  Mat seed=Img.clone();
  bool reachend=false;
  int intens,i;
  int theta=0;
  double yplus,xplus;

  while(theta<=360){
    reachend =false;
    i=1;
    yplus=-sin(RAD(theta));
    xplus=cos(RAD(theta));
    std::cout << "xplus = "<<xplus << std::endl;
    std::cout << "yplus = "<<yplus << std::endl;
    while(!reachend){

      intens=(int)Img.at<uchar>((centroid.y+i*yplus),(centroid.x+i*xplus));
      if(intens==segInt){
        seed.at<uchar>(centroid.y+i*yplus,centroid.x+i*xplus)=100;
        i++;
      }
      else reachend=true;
    }
      theta+=5;
  }

  return seed;
}

int getDistance(Mat Img,position centroid,int segInt,int theta){
  bool reachend =false;
  int i=1,intens = 0;
  double yplus=-sin(RAD(theta)), xplus=cos(RAD(theta));
  // std::cout << "xplus = "<<xplus << std::endl;
  // std::cout << "yplus = "<<yplus << std::endl;
  while(!reachend){
    intens=(int)Img.at<uchar>((centroid.y+i*yplus),(centroid.x+i*xplus));
    if(intens==segInt){
      // std::cout << "OK" << std::endl;
      i++;
    }
    else reachend=true;
  }
  // std::cout << "x = "<<x << std::endl;
  return i-1;
}

int getDistanceBoundary(Mat Img, position centroid, int theta){
  bool reachend =false;
  int i=1,intens = 0;
  double yplus=-sin(RAD(theta)), xplus=cos(RAD(theta));
  // std::cout << "xplus = "<<xplus << std::endl;
  // std::cout << "yplus = "<<yplus << std::endl;
  while(!reachend){
    intens=(int)Img.at<uchar>((centroid.y+i*yplus),(centroid.x+i*xplus));
    if(intens==255){
      // std::cout << "OK" << std::endl;
      reachend=true;
    }
    else i++;
  }
  return i;

  // std::cout << "x = "<<
}

int getDistanceTrigonometri(Mat Img,position centroid,int segInt,int theta){
  bool reachend =false;
  int i=1,intens = 0;
  double testing=0;
  double yplus=-sin(RAD(theta)), xplus=cos(RAD(theta));
  // std::cout << "xplus = "<<xplus << std::endl;
  // std::cout << "yplus = "<<yplus << std::endl;
  while(!reachend){
    intens=(int)Img.at<uchar>((centroid.y+i*yplus),(centroid.x+i*xplus));
    if(intens==segInt){
      // std::cout << "OK" << std::endl;
      i++;
    }
    else{
       reachend=true;
       testing = sqrt(pow(centroid.y+i*yplus,2)+pow((centroid.x+i*xplus),2));
    }
  }
  std::cout << "x = "<<testing << std::endl;
  return testing;
}

DAShistogram DistanceAngleHistogram(Mat Img,int segInt,int times){
  DAShistogram a;
  a.centroid=getCentroid(Img,segInt);
  a.segInt=segInt;
  a.times=times;
  int distan=getDistance(Img,a.centroid,segInt,0);
  int max = distan,min = distan;
  for(int i=0;i<times;i++){
    distan=getDistance(Img,a.centroid,segInt,360/times*i);
    // std::cout << "distan"<< distan << std::endl;
    a.arr.push_back(getDistance(Img,a.centroid,segInt,360/times*i));
    if (distan>max)max=distan;
    if (distan<min)min=distan;
  }
  a.max=max;
  a.min=min;
  return a;
}
DAShistogram DistanceAngleHistogramBoundary(Mat Img,int times){
  DAShistogram a;
  a.centroid=getCentroidBoundary(Img);
  a.segInt=255;
  a.times=times;
  int distan=getDistanceBoundary(Img,a.centroid,0);
  int max = distan,min = distan;
  for(int i=0;i<times;i++){
    distan=getDistanceBoundary(Img,a.centroid,360/times*i);
    // std::cout << "distan"<< distan << std::endl;
    a.arr.push_back(distan);
    if (distan>max)max=distan;
    if (distan<min)min=distan;
  }
  a.max=max;
  a.min=min;
  return a;
}



void saveDAS(DAShistogram h,string fileName){
  ofstream file;
  const char* filechar = fileName.c_str();
  file.open(filechar,ios::trunc);
  file << h.centroid.x<< " "<<h.centroid.y << " " << h.segInt << " " << h.times<<" "<<h.max<<" "<<h.min<<std::endl;
  for(int i=0;i<h.arr.size();i++){
    file<<360/h.times*i<<" "<<h.arr.at(i)<<std::endl;
  }
  file.close();
}

DAShistogram loadDAS(string fileName){
  DAShistogram h;
  ifstream file;
  const char* filechar = fileName.c_str();
  file.open(filechar,ios::in);
  int a;
  file>>h.centroid.x>>h.centroid.y>>h.segInt>>h.times>>h.max>>h.min;
  for(int i=0;i<h.times;i++){
    file>>a;
    file>>a;
    // std::cout << "a"<<a << std::endl;
    h.arr.push_back(a);
  }
  file.close();
  return h;
}

bool isSame(DAShistogram a,DAShistogram b){
  int modusa=getModusIndeks(a),modusb=getModusIndeks(b);
  double scale=(double) (b.max) /(a.max);
  std::cout << "b.max, a.max = "<<b.max<<" , "<<a.max  << std::endl;
  for(int i=0;i<a.times;i++){
    std::cout<<(int) (a.arr.at((modusa+i)%a.times)*scale) << " = = "<<b.arr.at((modusb+i)%b.times)<< std::endl;
    if( (int) (a.arr.at((modusa+i)%a.times)*scale) !=b.arr.at((modusb+i)%b.times))
      std::cout << "difference = " <<(abs( (int) (a.arr.at((modusa+i)%a.times)*scale) - (int) (b.arr.at((modusb+i)%b.times))))<< std::endl;
  }
  return true;
}

double difference(DAShistogram a,DAShistogram b){
  int modusa=getModusIndeks(a),modusb=getModusIndeks(b);
  double modusdifference;
  double x=0,diff=0;;
  if (modusa>modusb){
    modusdifference = modusb/modusa;
  }else{
    modusdifference = modusa/modusb;
  }
  double scale=(double) (b.max) /(a.max);

  for(int i=0;i<a.times;i++){
    if(a.arr.at((modusa+i)%a.times)*scale!=b.arr.at((modusb+i)%b.times)){
      diff = abs(a.arr.at((modusa+i)%a.times)-b.arr.at((modusb+i)%b.times));
      x+=diff;
    }
  }
  return x/a.arr.size();
}

std::vector<double> normalize(DAShistogram h){
  std::vector<double> norm;
  double x;
  for (int i=0;i<h.arr.size();i++){
    x=(double)(h.arr.at(i)-h.min)/(h.max-h.min);
    std::cout << "x "<< x << std::endl;
    norm.push_back(x);
  }
  return norm;
}
void normalize(DAShistogram * h){
  double x;
  for (int i=0;i<h->arr.size();i++){
    x=(double)(h->arr.at(i)-h->min)/(h->max-h->min);
    h->arr.at(i)=x;
  }
}

Mat boundary(Mat Img, int segInt){
  Mat padded;
  copyMakeBorder( Img, padded, 1, 1, 1, 1, BORDER_CONSTANT , 255-segInt );
  Mat bound(Img.rows+2, Img.cols+2, CV_8UC1, Scalar(0, 0, 0));
  std::vector<position> que;
  int i=0,x=0,bpx,bpy;
  que.push_back(createPos(-1,0));
  que.push_back(createPos(-1,-1));
  que.push_back(createPos(0,-1));
  que.push_back(createPos(1,-1));
  que.push_back(createPos(1,0));
  que.push_back(createPos(1,1));
  que.push_back(createPos(0,1));
  que.push_back(createPos(-1,1));

  position p0,p,b;
  for(int y=0;y<Img.rows;y++){
    for(int x=0;x<Img.cols;x++){
      if(Img.at<uchar>(y,x)==segInt){
        p0=createPos(x,y);
        b=createPos(x,y);
        p=createPos(x,y);
        bound.at<uchar>(y+1,x+1)= 255;
        y=Img.rows;
        x=Img.cols;
      }
    }
  }
  std::cout << "P0 = "<<p.x<<" , "<<p.y << std::endl;

  while(Img.at<uchar>( p.y + que.at(i).y ,p.x + que.at(i).x )!=segInt&&i<que.size()){
    i++;
    std::cout << "x , y = "<<p.x + que.at(i).x <<" "<<p.y + que.at(i).y << std::endl;
  }

  bound.at<uchar>(p.y+que.at(i).y,p.x+que.at(i).x)= 255;
  p.y=p.y+que.at(i).y;
  p.x=p.x+que.at(i).x;
  std::cout << "P = "<<p.x<<" , "<<p.y << std::endl;
  i=0;

  while(true){
    std::cout << "P0 = "<<p.x<<" , "<<p.y << std::endl;
    bpx = b.x-p.x;
    bpy = b.y-p.y;
    std::cout << "b = "<<b.x<<" , "<<b.y << std::endl;
    std::cout << "bpx , bpy "<<bpx<<" , "<<bpy << std::endl;
    for (int in=0;in<que.size();in++){
      if(bpx==que.at(in).x&&bpy==que.at(in).y){
        i=in+1;
        break;
      }
    }
    std::cout << "i "<< i << std::endl;

    // std::cout << "a" << std::endl;
    for(int x=0;x<que.size();x++){
        if(Img.at<uchar>(p.y+que.at((i+x)%8).y,p.x+que.at((i+x)%8 ).x)==segInt){
          i=(i+x)%8;
          std::cout << "i"<< i  << std::endl;
          x=que.size();
        }
    }
    std::cout << "a" << std::endl;
    bound.at<uchar>(p.y+que.at(i).y,p.x+que.at(i).x)= 255;
    b.x=p.x;
    b.y=p.y;
    p.y=p.y+que.at(i%8).y;
    p.x=p.x+que.at(i%que.size()).x;
    std::cout << "P = "<<p.x<<" , "<<p.y << std::endl;

    if (p.x==p0.x&&p.y==p0.y)break;
  }

  return bound;
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
    statistikHistogram(before);
    drawHistogram(before,"testing");
    waitKey(0);
    Mat Seg = seedByThreshold(imageGr,otsuThresholding(before),255);
    showImage(Seg,"Otsu");
    waitKey(0);
    // Mat Bond = boundary(Seg,0);
    position tes=getCentroid(Seg,0);
    showImage(Seg,"Boundary");
    DAShistogram DH1 = DistanceAngleHistogram(Seg,0 ,72);
    drawHistogram(DH1,"Histo");
    string fileName=imageName+".DAS";
    saveDAS(DH1,fileName);
    waitKey(0);
    // normalize (&haha);
    // drawHistogram(haha,"histo2");
    // showImage(drawline(Seg,tes,255),"Line");
    // std::vector<double> haha2 =normalize(haha);
    // drawNormalHistogram(haha2,"Histo2");
    // showImage(boundary(Seg,0),"Testinger");
    waitKey(0);
    // std::cout << "Modus haha"<<getModus(haha) << std::endl;
    if(argc>2){
      string loadFile=argv[2];
      DAShistogram DH2=loadDAS(loadFile);
      drawHistogram(DH2,"hehe");
      std::cout << "is Same"<<isSame(DH1,DH2) << std::endl;
      std::cout << "difference" <<difference(DH1,DH2)<< std::endl;
    }

    // file<<


    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
