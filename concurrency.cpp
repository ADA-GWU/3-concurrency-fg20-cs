#include <iostream>
// #include "CImg.h"
using namespace std;


int main(int argc, char *argv[]){
   if(argc < 4){
    cout << "Error: Not enough arguments! See the usage below:" << endl << endl;
    cout << "<program> [file] [size] [mode]" << endl << endl;
    cout << "\tfile \tThe name of the jpg file containing the image" << endl;  
    cout << "\tsize \tThe side of the square for the averaging"  << endl;
    cout << "\tmode \tProcessing mode which can be 'S' - single threaded and 'M' - multi threaded" << endl << endl;
    exit(0);
   }  

   cout << "Good to go!" << endl;

  return 0;
}

