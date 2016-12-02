#include <fstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
  int i=2;
  ofstream file;
  string fileName=argv[1];
  const char* filechar = fileName.c_str();
  file.open(filechar,ios::trunc);
  while(i<argc){
    file<<argv[i]<<std::endl;
    i++;
  }
  file.close();
  return 0;
}
