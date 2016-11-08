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

void statistikHistogram(histogram h,Mat img){
    cout<<"\nSTATISTIK GAMBAR"<<endl;
    cout<<"\nIntensitas Maksimal\t: "<<h.maks<<endl;
    cout<<"Intensitas Minimal\t: "<<h.mins<<endl;
    cout<<"Total Intensitas\t: "<<h.jumlahint<<endl;
    cout<<"Jumlah Pixel\t\t: "<<h.jumlahpx<<endl;
    cout<<"Intenstitas Rata - rata\t: "<<h.avg<<endl;
    cout<<"Standar Deviasi\t\t: "<<standarDeviasi(h,img)<<endl;
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
  // std::cout << "mean = "<<mean << std::endl;
  double sum=0;
  int n=0;
  for(int i= start;i<end;i++){
    for(int j=0;j<data[i];j++){
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

int * duoOtsu(histogram h){
  double var[(int) pow(254,2)][3];
  double vk[3];
  int n=0;
  int minindeks=0;
  for(int i=0;i < pow(254,2);i++){
    for(int j=0;j<3;j++){
      var[i][j]=0;
    }
  }
  int i=0;
  for(i;i<253;i++){
    vk[0]=0;
    vk[0]=countVariance(h.aray,0,i+1)*countSegment(h.aray,0,i+1)/h.jumlahpx;
    for(int j=i+2; j< 254;j++){
      vk[1]=0;
      vk[2]=0;

      vk[1]=countVariance(h.aray,i+1,j+1)*countSegment(h.aray,i+1,j+1)/h.jumlahpx;
      vk[2]=countVariance(h.aray,j+1,256)*countSegment(h.aray,j+1,256)/h.jumlahpx;
      // std::cout << "VK" <<" "<< vk[0]<<" "<<vk[1] <<" "<<vk[2] <<" "<<vk[0]+vk[1]+vk[2]<< std::endl;
      var[n][0] = vk[0]+vk[1]+vk[2];
      var[n][1]=i+1;
      var[n][2]=j+1;
      // std::cout << "var["<<n<<"] = "<<var[n][1] << std::endl;

      // std::cout << "var[minindeks][0] = "<<var[minindeks][0] << std::endl;
      if (var[minindeks][0]>var[n][0]){
        minindeks=n;
      }
      n++;
      // std::cout << "minindeks"<<minindeks << std::endl;
    }
  }
  // std::cout << "sampai sini" <<(int)var[minindeks][1]<<", "<<(int) var[minindeks][2]<< std::endl;
  static int index[2]={(int)var[minindeks][1],(int)var[minindeks][2]};
  // std::cout << "minindeks = "<< minindeks << std::endl;
  // std::cout << "index"<<index[0]<<", "<<index[1] << std::endl;
  return index;
}
//
// double ** generateVariance(histogram h,int start,int times){
//   static int size= (int) pow(254-start,times);
//   static int x=times+1;
//   double var[size][x];
//   double lv=0;
//   int n=0;
//   for(int i=0;i < size;i++){
//     for(int j=0;j<times+1;j++){
//       var[i][j]=0;
//     }
//   }
//   std::cout << "times = "<<times << std::endl;
//   if (times==1){
//     int vki=0,vka=0;
//     for(int i=start;i<254;i++){
//       vki=0,vka=0;
//       vki=countVariance(h.aray,start,i+1);
//       vka=countVariance(h.aray,i+1,256);
//       var[i][0]=vki+vka;
//       var[i][1]=i+1;
//       std::cout << "var[i] "<<var[i][0]<<" "<< var[i][1] << std::endl;
//     }
//     static double indexvar[size][x]= new double [254][2];
//     return var;
//   }
//   else{
//
//   }
// }
// double ** duplicateArray()

// int * multiOtsu(histogram h,start,times){
//   double var[(int) pow(254,2)][3];
//   double vk[3];
//   int n=0;
//   int minindeks=0;
//   for(int i=0;i < pow(254,2);i++){
//     for(int j=0;j<3;j++){
//       var[i][j]=0;
//     }
//   }
//   int i=0;
//   for(i;i<253;i++){
//     vk[0]=0;
//     vk[0]=countVariance(h.aray,0,i+1);
//     for(int j=i+2; j< 254;j++){
//       vk[1]=0;
//       vk[2]=0;
//
//       vk[1]=countVariance(h.aray,i+1,j+1);
//       vk[2]=countVariance(h.aray,j+1,256);
//       // std::cout << "VK" <<" "<< vk[0]<<" "<<vk[1] <<" "<<vk[2] <<" "<<vk[0]+vk[1]+vk[2]<< std::endl;
//       var[n][0] = vk[0]+vk[1]+vk[2];
//       var[n][1]=i;
//       var[n][2]=j;
//       // std::cout << "var["<<n<<"] = "<<var[n][1] << std::endl;
//
//       // std::cout << "var[minindeks][0] = "<<var[minindeks][0] << std::endl;
//       if (var[minindeks][0]>var[n][0]){
//         minindeks=n;
//       }
//       n++;
//       // std::cout << "minindeks"<<minindeks << std::endl;
//     }
//   }
//   std::cout << "sampai sini" <<(int)var[minindeks][1]<<", "<<(int) var[minindeks][2]<< std::endl;
//   static int index[2]={(int)var[minindeks][1],(int)var[minindeks][2]};
//   // std::cout << "minindeks = "<< minindeks << std::endl;
//   // std::cout << "index"<<index[0]<<", "<<index[1] << std::endl;
//   return index;
// }


Mat imageSegmentation1(Mat Img,int threshold){
    //generate contrast stretched image
    int intens=0;
    Mat Segment(Img.rows, Img.cols, CV_8UC1, Scalar(255, 255, 255));
    for(int y = 0; y < Segment.rows; y++){
        for(int x = 0; x < Segment.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            if(intens<=threshold){
              Segment.at<uchar>(y,x)=(int)cvRound( 0 );
            }
            else{
              Segment.at<uchar>(y,x)=(int)cvRound( 255 );
            }
          }
    }
    return Segment;
}

Mat duoSegmentation(Mat Img,int threshold[]){
    //generate contrast stretched image
    int intens=0;
    Mat Segment(Img.rows, Img.cols, CV_8UC1, Scalar(255, 255, 255));
    for(int y = 0; y < Segment.rows; y++){
        for(int x = 0; x < Segment.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            float i=0;
            while (intens>threshold[(int)i]&&i<2 ){
            //cout<<h <<"	"<<divider[i]<<endl;
            i++;
            }
            // std::cout << "i = "<<(i/2) *255 << std::endl;
            Segment.at<uchar>(y,x)=(int)cvRound( i/2 *255 );
          }
    }
    return Segment;
}

Mat imageSegmentation(Mat Img,int threshold[]){
    //generate contrast stretched image
    int intens=0;
    Mat Segment=Img;//(image.rows, image.cols, CV_8UC1, Scalar(255, 255, 255));
    for(int y = 0; y < Segment.rows; y++){
        for(int x = 0; x < Segment.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
            float i=0;
            while (intens>threshold[(int)i]&&i<(sizeofp(threshold)) ){
            //cout<<h <<"	"<<divider[i]<<endl;
            i++;
            }
            Segment.at<uchar>(y,x)=(int)cvRound( i/(sizeofp(threshold)-1)*255 );
          }
    }
    return Segment;
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
    // double ** tessub1 = generateVariance(before,200,1);
    int singleThreshold=otsuThresholding(before);
    int * duoThreshold=duoOtsu(before);
    std::cout << "Single Threshold\t: "<<singleThreshold << std::endl;
    std::cout << "Dual Threshold\t\t: "<<duoThreshold[0]<<" , "<<duoThreshold[1] << std::endl;
    showImage(imageSegmentation1(imageGr,singleThreshold),"Otsu 1");
    showImage(duoSegmentation(imageGr,duoThreshold),"Otsu 2");

    // fillHistogram(&after,imageGrCS);
    // statistikHistogram(after,imageGrCS);
    // drawHistogram(after,"Histogram After");
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
