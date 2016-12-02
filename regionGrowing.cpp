#include "imageSegmentation.h"
#include "regionGrowing.h"
#include "histogram2.h"

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
      input1=getModusIndeks(h.aray);
      std::cout << "modus = "<<input1 << std::endl;
      break;
    case -2:
      input1=otsuThresholding(h);
      std::cout << "Otsu number = "<<input1 << std::endl;
      break;
    case -3:
      input1=countNumberB(h.aray,n);
      std::cout << "numberB = "<<input1 << std::endl;
      break;
    case -4:
      input1=countNumberK(h.aray,n);
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
      input1=getModusIndeks(h.aray);
      std::cout << "modus = "<<input1 << std::endl;

      break;
    case -2:
      input1=otsuThresholding(h);
      std::cout << "Otsu number = "<<input1 << std::endl;
      break;
    case -3:
      input1=countNumberB(h.aray,l);
      std::cout << "numberB left = "<<input1 << std::endl;
      break;
    case -4:
      input1=countNumberK(h.aray,l);
      std::cout << "numberk left = "<<input1 << std::endl;
      break;
    case -5:
      input1=countNumberB(h.aray,u);
      std::cout << "numberB right = "<<input1 << std::endl;
      break;
    case -6:
      input1=countNumberK(h.aray,u);
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
      input1=getModusIndeks( h.aray);
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
  Mat seed=singleThresholdSegmentation(image, seedLowerThreshold,seedUpperThreshold);
  seedGrowing4ByThreshold2(seed,image,simThreshold,255);
  return seed;
}
Mat regionGrowingByIntensity(Mat image, int simThreshold,int seedIntensity){
  Mat seed=singleIntensitySegmentation(image, seedIntensity);
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
  showImage(singleIntensitySegmentation(image,seedintensity),"seed ");
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
    Mat seed=moreIntensitySegmentation(image,Seed,seedintensity,tins);
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
  showImage(singleThresholdSegmentation(image,seedLowerThreshold,seedUpperThreshold),"seed ");
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

    Mat seed=moreThresholdSegmentation(image,Seed,seedLowerThreshold,seedUpperThreshold,tins);
    Sed[done]=seed;
    showImage(Sed[done],"seed ");
    waitKey(0);
    std::cout << "Simi Threshold\t: "<<std::endl;
    simThreshold=input(h,seedLowerThreshold,seedUpperThreshold);

    seedGrowing4ByThreshold2(seed,image,simThreshold,tins);

    gro[done]=seed;
    showImage(gro[done],"Growth ");
    waitKey(0);
    Seed=seed;
    done++;
  }
  return Seed;
}
