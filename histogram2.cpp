#include "histogram2.h"

histogram createHist(int size){
  histogram h;
  for(int i=0;i <size;i++){
      h.aray.push_back(0);
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
          }
          if(intens < h->mins){
              h->mins = intens;
          }

          //Menambah nilai pada histogram
          h->aray.at(intens)++;
      }
  }

  //menghitung rata-rata
  h->avg =(double) h->jumlahint/h->jumlahpx;
}

int getModus(std::vector<int> data){
    int maks=data.at(0);
    int freq;
    for(int i=0;i <256;i++){
      freq=data.at(i);
      if (freq > maks) maks=freq;
    }
    return maks;
}
int getModusIndeks(std::vector<int> data){
    int maks=0;
    for(int i=0;i <256;i++){
        if (data.at(i) >data.at(maks)) maks=i;
    }
    return maks;
}

double countStandarDeviasi(std::vector<int> data ,int lowerThreshold,int upperThreshold){
    int mean=countMean(data,lowerThreshold,upperThreshold);
    // std::cout << "mean = "<<mean << std::endl;
    double sum=0;
    int n=0,size=0;
    for(int i= lowerThreshold;i<upperThreshold;i++){
      size=data.at(i);
      for(int j=0;j<size;j++){
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
double countMean(std::vector<int> data ,int start,int end){//end = index terakhir+1
  int sum=0;
  int count=0;
  int x;
  for(int i= start;i<end;i++){
    x=data.at(i);
    count=count+i*x;
    sum=sum+x;
  }
  if (sum!=0)
    return (double) count/sum;
  else
    return 0;
}
double countVariance(std::vector<int> data ,int start,int end){
  int mean=countMean(data,start,end);
  // std::cout << "mean = "<<mean << std::endl;
  double sum=0;
  int n=0,size=0;
  for(int i= start;i<end;i++){
    size=data.at(i);
    for(int j=0;j<size;j++){
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
double countSegment(std::vector<int> data ,int start,int end){
  double sum=0;
  for(int i= start;i<end;i++){
    sum+=data.at(i);
  }
  return sum;
}

void drawHistogram(histogram h,string name){
    int hist_w = 512;
    int hist_h = 100;
    int bin_w = floor((double) hist_w/256);
    float bin_h = (float) hist_h/getModus(h.aray);

    std::cout << h.aray.size() << '\n';
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    for (int i=0;i<h.aray.size();i++){
      //cout <<cvRound((double)bin_h*histogram[i])<<endl;
      line(histImage,  //img
        Point(bin_w*(i), hist_h), //point a
        Point(bin_w*(i), hist_h - cvRound((double) h.aray.at(i)*bin_h)),//point b
        Scalar(0,0,0),//color black
        bin_w,//thickness
        0,//linetype
        0);//Number of fractional bits in the point coordinates.
        // std::cout << cvRound((double) h.aray.at(i)*bin_h) << '\n';
    }
    showImage(histImage,name);
}
void statistikHistogram(histogram h){
    cout<<"\nSTATISTIK GAMBAR"<<endl;
    cout<<"\nIntensitas Maksimal\t: "<<h.maks<<endl;
    cout<<"Intensitas Minimal\t: "<<h.mins<<endl;
    cout<<"Total Intensitas\t: "<<h.jumlahint<<endl;
    cout<<"Jumlah Pixel\t\t: "<<h.jumlahpx<<endl;
    cout<<"Intenstitas Rata - rata\t: "<<h.avg<<endl;
    cout<<"Standar Deviasi\t\t: "<<countStandarDeviasi(h.aray,0,256)<<endl;
}

// Unknown Number versi Billy
int countNumberB(std::vector<int> data ,int n){
  int ST=0;
  int SDKiri= cvRound(countStandarDeviasi(data,0,n));
  int SDKanan=cvRound(countStandarDeviasi(data,n,256));
  int SDTotal=cvRound(countStandarDeviasi(data,0,256));
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
int countNumberK(std::vector<int> data ,int n){
  int ST=0;
  int SDKiri= cvRound(countStandarDeviasi(data,0,n));
  int SDKanan=cvRound(countStandarDeviasi(data,n,256));
  int SDTotal=cvRound(countStandarDeviasi(data,0,256));
  if (SDKiri==0||SDKanan==0){
    ST=cvRound(sqrt(SDKiri+SDKanan));
  }else{
    if(SDKiri>SDKanan) ST=cvRound(SDKanan);
    else ST=cvRound(SDKiri);
  }

  return ST;
}
