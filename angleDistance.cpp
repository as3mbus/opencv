#include "angleDistance.h"

// template<typename T,int N> //template argument deduction
// int size(T (&arr1)[N]) //Passing the array by reference
// {
//   size_t size;
//   size=sizeof(arr1)/sizeof(arr1[0]);
//
//   std::cout<<size<<std::endl; //Correctly prints the size of arr
//
//   //EDIT
//
//   return N; //Correctly prints the size too [cool trick ;-)]
// }
position createPos(int x,int y){
  position a;
  a.x=x;
  a.y=y;
  return a;
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
int getModusIndeks(DAShistogram h){
    int maks=0;
    for(int i=0;i <h.arr.size();i++){
        if (h.arr.at(i) >h.arr.at(maks)) maks=i;
    }
    return maks;
}

void drawHistogram(DAShistogram h,string name){
  int hist_w = 512;
  int hist_h = 100;
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
  // int modusa=getModusIndeks(a),modusb=getModusIndeks(b);
  // double scale=(double) (b.max) /(a.max);
  // std::cout << "b.max, a.max = "<<b.max<<" , "<<a.max  << std::endl;
  // for(int i=0;i<a.times;i++){
  //   std::cout<<(int) (a.arr.at((modusa+i)%a.times)*scale) << " = = "<<b.arr.at((modusb+i)%b.times)<< std::endl;
  //   if( (int) (a.arr.at((modusa+i)%a.times)*scale) !=b.arr.at((modusb+i)%b.times))
  //     std::cout << "difference = " <<(abs( (int) (a.arr.at((modusa+i)%a.times)*scale) - (int) (b.arr.at((modusb+i)%b.times))))<< std::endl;
  // }
  return differenceMean(a,b)<=2;
}
double differenceMean(DAShistogram a,DAShistogram b){
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
    diff = abs(a.arr.at((modusa+i)%a.times)*scale - b.arr.at((modusb+i)%b.times));
    // std::cout << "diff = "<<(int)diff << '\n';
    x+=diff;

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
  // std::cout << "P0 = "<<p.x<<" , "<<p.y << std::endl;
  while(Img.at<uchar>( p.y + que.at(i).y ,p.x + que.at(i).x )!=segInt&&i<que.size()){
    // std::cout<< i  << " x , y = "<<p.x + que.at(i).x <<" "<<p.y + que.at(i).y << std::endl;
    i++;
  }

  bound.at<uchar>(p.y+que.at(i).y,p.x+que.at(i).x)= 255;
  p.y=p.y+que.at(i).y;
  p.x=p.x+que.at(i).x;
  // std::cout << "P = "<<p.x<<" , "<<p.y << std::endl;
  i=0;

  while(true){
    // std::cout << "P0 = "<<p.x<<" , "<<p.y << std::endl;
    bpx = b.x-p.x;
    bpy = b.y-p.y;
    // std::cout << "b = "<<b.x<<" , "<<b.y << std::endl;
    // std::cout << "bpx , bpy "<<bpx<<" , "<<bpy << std::endl;
    for (int in=0;in<que.size();in++){
      if(bpx==que.at(in).x&&bpy==que.at(in).y){
        i=in+1;
        break;
      }
    }
    // std::cout << "i "<< i << std::endl;

    // std::cout << "a" << std::endl;
    for(int x=0;x<que.size();x++){
        if(Img.at<uchar>(p.y+que.at((i+x)%8).y,p.x+que.at((i+x)%8 ).x)==segInt){
          i=(i+x)%8;
          // std::cout << "i"<< i  << std::endl;
          x=que.size();
        }
    }
    // std::cout << "a" << std::endl;
    bound.at<uchar>(p.y+que.at(i).y,p.x+que.at(i).x)= 255;
    b.x=p.x;
    b.y=p.y;
    p.y=p.y+que.at(i%8).y;
    p.x=p.x+que.at(i%que.size()).x;
    // std::cout << "P = "<<p.x<<" , "<<p.y << std::endl;

    if (p.x==p0.x&&p.y==p0.y)break;
  }

  return bound;
}
