
#include "imageSegmentation.h"

Mat singleSegmentation(Mat Img,int threshold){
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
