#include "imageSegmentation.h"

int sizeofp(int *p){
   return sizeof(p)/sizeof p[0];// you cant get the size of empty array
   //http://stackoverflow.com/questions/492384/how-to-find-the-sizeofa-pointer-pointing-to-an-array
    //array sizeof : http://stackoverflow.com/questions/1898657/result-of-sizeof-on-array-of-structs-in-c
}

Mat singleThresholdSegmentation(Mat Img,int lowerThreshold,int upperThreshold){
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

Mat singleThresholdSegmentation(Mat Img,int threshold){
    return singleThresholdSegmentation(Img,threshold,256);
}
Mat duoThresholdSegmentation(Mat Img,int threshold[]){
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
Mat imageThresholdSegmentation(Mat Img,int threshold[]){
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
Mat moreThresholdSegmentation(Mat Img,Mat seed,int lowerThreshold,int upperThreshold,double TIns){
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

Mat singleIntensitySegmentation(Mat Img,int intensity){
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
Mat moreIntensitySegmentation(Mat Img,Mat seed,int intensity,double TIns){
    //generate contrast stretched image
    int intens=0;
    Mat seed2(Img.rows, Img.cols, CV_8UC1, Scalar(0, 0, 0));;
    for(int y = 0; y < seed.rows; y++){
        for(int x = 0; x < seed.cols; x++){
            intens=(int)Img.at<uchar>(y,x);
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
