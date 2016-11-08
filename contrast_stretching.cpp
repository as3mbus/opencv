#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <string>
#include <iostream>
using namespace cv;
using namespace std;



int main( int argc, char** argv ){
    string imageName("../data/HappyFish.jpg"); // by default
    if( argc > 1)
    {
        imageName = argv[1];
    }
    Mat image;
    image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file
    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );			 	// Create a window for display.
    imshow( "Display window", image );                				// Show our image inside it.
    
    int n,maks,mins,jumlah,intens,modus,median;
    float avg;

    
    int histogram[256];
    int histoAfter[256];
    
    for(int i = 0; i < 256; i++){
        histogram[i] = 0;
    }
    

    maks = image.at<uchar>(0,0);
    mins = image.at<uchar>(0,0);
    jumlah = 0;
    n = 0;

    Mat im_gray = imread(imageName.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
    
    
    namedWindow( "Gray scale ed", WINDOW_AUTOSIZE ); 				// Create a window for display.
    imshow( "Gray scale ed", im_gray );                				// Show our image inside it.
    

    

    //Kalkulasi jumlah pixel untuk setiap intensitas
    for(int y = 0; y < im_gray.rows; y++){
        for(int x = 0; x < im_gray.cols; x++){
            //Inisialisasi intensitas
            intens = (int)im_gray.at<uchar>(y,x);
            //parameter rata-rata
            jumlah += intens;
            n++;

            //Mengganti maks dan min
            if(intens > maks){
                maks = intens;
            }else if(intens < mins){
                mins = intens;
            }

            //Menambah nilai pada histogram
            histogram[intens]++;
        }
    }

    //menghitung rata-rata
    avg = jumlah/n;

    //Menemukan nilai intensitas yang paling sering muncul

    modus = histogram[0];
    for(int i = 0; i < 256; i++){
        if(modus < histogram[i]){
            modus = histogram[i];
        }
    }
    


    //Statistik Gambar
    cout<<"\nSTATISTIK GAMBAR"<<endl;
    cout<<"\nNilai Maksimal\t: "<<maks<<endl;
    cout<<"Nilai Minimal\t: "<<mins<<endl;
    cout<<"Total Intensitas\t: "<<jumlah<<endl;
    cout<<"Jumlah Pixel\t: "<<n<<endl;
    cout<<"Rata - rata\t: "<<avg<<endl;

    //Gambar histogram
    int hist_w = 1280;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w/256);
    float bin_h = (float) hist_h/modus;

    //cout<<hist_h<<"	"<<modus<<"	"<<bin_h;
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    for (int i=0;i<sizeof(histogram);i++){
	//cout <<cvRound((double)bin_h*histogram[i])<<endl;
	line(histImage,  //img
	     Point(bin_w*(i), hist_h), //point a
             Point(bin_w*(i), hist_h - cvRound((double) histogram[i]*bin_h)),//point b
	     Scalar(0,0,0),//color black
	     bin_w,//thickness
             8,//linetype
             0);//Number of fractional bits in the point coordinates. 
    }
    
    namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
    imshow("Histogram", histImage);


    //generate contrast stretched image
    Mat ContStretch=im_gray;//(image.rows, image.cols, CV_8UC1, Scalar(255, 255, 255));
    int divideby;
    //cout <<"titik pemecah pada histogram : ";cin>>divideby;

    int a=0,b=255;
    int c,d; //lowest and highest val of the section 
    int divider[3]={30,120,200};
    for(int y = 0; y < image.rows; y++){
        for(int x = 0; x < image.cols; x++){
            intens=(int)im_gray.at<uchar>(y,x);
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

    for(int y = 0; y < ContStretch.rows; y++){
        for(int x = 0; x < ContStretch.cols; x++){
            //Inisialisasi intensitas
            intens = (int)ContStretch.at<uchar>(y,x);
            //parameter rata-rata
            jumlah += intens;
            n++;

            //Mengganti maks dan min
            if(intens > maks){
                maks = intens;
            }else if(intens < mins){
                mins = intens;
            }

            //Menambah nilai pada histogram
            histoAfter[intens]++;
        }
    }
    namedWindow("After", CV_WINDOW_AUTOSIZE);
    imshow("After", ContStretch);

    modus = histogram[0];
    for(int i = 0; i < 256; i++){
        if(modus < histoAfter[i]){
            modus = histoAfter[i];
        }
    }

    
    bin_h = (float) hist_h/modus;
    Mat graphAfter(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    for (int i=0;i<sizeof(histogram);i++){
	//cout <<cvRound((double)bin_h*histogram[i])<<endl;
	line(graphAfter,  //img
	     Point(bin_w*(i), hist_h), //point a
             Point(bin_w*(i), hist_h - cvRound((double) histoAfter[i]*bin_h)),//point b
	     Scalar(0,0,0),//color black
	     bin_w,//thickness
             8,//linetype
             0);//Number of fractional bits in the point coordinates. 
    }

    //display histogram


    
    namedWindow("Histogram After", CV_WINDOW_AUTOSIZE);
    imshow("Histogram After", graphAfter);
    

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}

