#include "cStretching.h"

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
