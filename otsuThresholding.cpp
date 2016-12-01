#include "otsuThresholding.h"

int sizeofp(int *p){
   return sizeof(p)/sizeof p[0];// you cant get the size of empty array
    //http://stackoverflow.com/questions/492384/how-to-find-the-sizeofa-pointer-pointing-to-an-array
    //array sizeof : http://stackoverflow.com/questions/1898657/result-of-sizeof-on-array-of-structs-in-c
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
