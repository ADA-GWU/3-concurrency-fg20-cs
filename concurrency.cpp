#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int limit(int x){
  return (x > 255)? 255 : x;
}

int r_average(Mat img, int x, int y, int size){
  int r = 0;
  for (int i = y; i < (y+size); i++) {
    for (int j = x; j < (x+size); j++){
      r += img.at<Vec3b>(i, j)[0];
    }
  }
  return limit(r / (size*size));
}

int g_average(Mat img, int x, int y, int size){
  int g = 0;
  for (int i = y; i < (y+size); i++) {
    for (int j = x; j < (x+size); j++){
      g += img.at<Vec3b>(i, j)[1];
    }
  }
  return limit(g / (size*size));
}

int b_average(Mat img, int x, int y, int size){
  int b = 0;
  for (int i = y; i < (y+size); i++) {
    for (int j = x; j < (x+size); j++){
      b += img.at<Vec3b>(i, j)[2];
    }
  }
  return limit(b / (size*size));
}

int addition(int q, int div){
  int rest = div - (q % div);
  if(rest == div) return 0;
  return rest;
}

void blur(Mat img, int size){

  int width = img.rows;
  int height = img.cols;
  
  int new_rows = width + addition(width, size);
  int new_cols = height + addition(height, size);
  Mat newImage(new_rows, new_cols, CV_8UC3);
  img.copyTo(newImage);
  namedWindow("Image", WINDOW_NORMAL);
  

  int rows = width - (width%size);
  int cols = height - (height%size);

  for (int y = 0; y < rows; y+=size) {
    for (int x = 0; x < cols; x+=size) {
      imshow("Image", newImage);
      int r_av = r_average(img, x, y, size);
      int g_av = g_average(img, x, y, size);
      int b_av = b_average(img, x, y, size);
      for (int i = y; i <= y+size; i++) {
        for (int j = x; j <= x+size; j++){
          Vec3b color;
          color[0] = r_av;
          color[1] = g_av;
          color[2] = b_av;
          newImage.at<Vec3b>(i, j) = color;
        }
      }
    }
    int k = waitKey(10);
    if(k == 'q')
      break;
  }
}


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
  string file_name = argv[1];
  int size = stoi(argv[2]);
  Mat img = imread(file_name, IMREAD_COLOR);

  blur(img, size);
  
  return 0;
}