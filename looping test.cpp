#include <math.h>
#include <string>
#include <iostream>
using namespace std;
int main(){
    int divider[5]={30,100,170};
    cout<<(int)(sizeof(divider)/4)-1<<endl;
    for(int h=0;h<256;h++){
	
        int i=0;
        while (h>divider[i]){
            //cout<<h <<"	"<<divider[i]<<endl;
            
            i++;
        }
        if (i==(int)(sizeof(divider)/4))
 	    cout<<h<<"anda di batas tertinggi"<<endl;
	else if (i==0)
	    cout<<h<<"anda berada di batas terbawah"<<endl;
        else 
            cout<<h<<"anda tidak di mana mana"<<endl;
    }
}
