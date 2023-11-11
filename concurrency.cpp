#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <vector>

using namespace std;
using namespace cv;

int limit(int x){
  return (x > 255)? 255 : x;
}

Vec3b rgbAverage(Mat img, int x, int y, int size_x, int size_y){
  int r = 0, g = 0, b = 0;
  for (int i = y; i < (y+size_y); i++) {
    for (int j = x; j < (x+size_x); j++){
      r += img.at<Vec3b>(i, j)[0];
      g += img.at<Vec3b>(i, j)[1];
      b += img.at<Vec3b>(i, j)[2];
    }
  }
  Vec3b color;
    color[0] = limit(r / (size_x*size_y));
    color[1] = limit(g / (size_x*size_y));
    color[2] = limit(b / (size_x*size_y));

  return color;
}

void quantize(Mat img, int col, int row, int size_col, int size_row){
  Vec3b color;
  color = rgbAverage(img, col, row, size_col, size_row);
  for (int i = row; i < row+size_row; i++) {
    for (int j = col; j < col+size_col; j++){
      img.at<Vec3b>(i, j) = color;
    }
  }
}

void quantizeImageParallel(Mat img, int size){
  
  namedWindow("Image", WINDOW_NORMAL);
  unsigned int nThreads = thread::hardware_concurrency();
  if(nThreads == 0) nThreads = 4;
  vector<thread> threads;

  int segmentRow = (img.rows - img.rows%size)/nThreads;
  int fittedCols = img.cols - img.cols%size;
  int fittedRows = segmentRow - segmentRow%size;

  for(int k = 0; k < segmentRow; k+=size){
    for (int x = 0; x < img.cols; x+=size){
      if(x < (fittedCols) && k < (fittedRows)){
        int i = 0;
        for (int y = k; i < nThreads; i++, y = k+i*segmentRow){
          threads.push_back(thread(quantize, img, x, y, size, size));
          threads[i].join();
        }
        for (int j = 0; j < nThreads; j++){
          threads.pop_back();
        }
      }
        
      if(x >= (fittedCols) && k >= (fittedRows)){
        int i = 0;
        for (int y = k; i < nThreads; i++, y = k + i*segmentRow){
          threads.push_back(thread(quantize, img, x, y, img.cols%size, segmentRow%size));
          threads[i].join();
        }
          
        for (int j = 0; j < nThreads; j++){
          threads.pop_back();
        }
      }

      if(x >= (fittedCols) && k < (fittedRows)){
        int i = 0;
        for (int y = k; i < nThreads; i++, y = k+i*segmentRow){
          threads.push_back(thread(quantize, img, x, y, img.cols%size, size));
          threads[i].join();
        }

        for (int j = 0; j < nThreads; j++){
          threads.pop_back();
        }
      }
      
      if(x < (fittedCols) && k >= (fittedRows)){
        int i = 0;
        for (int y = k; i < nThreads; i++, y = k+i*segmentRow){
          threads.push_back(thread(quantize, img, x, y, size, segmentRow%size));
          threads[i].join();
        }
          
        for (int j = 0; j < nThreads; j++){
          threads.pop_back();
        }
      }
      imshow("Image", img);
      waitKey(100);
      imwrite("result_parallel.jpg", img);
    }
  }

  int rest = img.rows  - segmentRow*nThreads;
  if(rest > 0){
    for (int x = 0; x < img.cols; x+=size){
      if(x < (fittedCols)){
        thread t(quantize, img, x, segmentRow*nThreads, size, rest);
        t.join();
      }
      else{
        thread t(quantize, img, x, segmentRow*nThreads, img.cols%size, rest);
        t.join(); break;
      }
      imshow("Image", img);
      waitKey(100);
      imwrite("result_parallel.jpg", img);
    }
  }
  imshow("Image", img);
  waitKey(0);
  imwrite("result_parallel.jpg", img);

}

void quantizeImageSequential(Mat img, int size){

  int height = img.rows;
  int width = img.cols;

  namedWindow("Image", WINDOW_NORMAL);
  int x, y;
  Vec3b color;
  for (y = 0; y < height; y+=size) {
    for (x = 0; x < width; x+=size) {
      imshow("Image", img);
      if(x < (width-width%size) && y < (height-height%size)){
        quantize(img, x, y, size, size);
      }
      if(x < (width-width%size) && y >= (height-height%size)){
        quantize(img, x, y, size, height%size);
      }
      if(x >= (width-width%size) && y < (height-height%size)){
        quantize(img, x, y, width%size, size);
      }
      else{
        quantize(img, x, y, width%size, height%size);
      }
      waitKey(1);
    }
  }
  imshow("Image", img);
  waitKey(0);
  imwrite("result_sequential.jpg", img);
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
  string fileName = argv[1];
  int size = stoi(argv[2]);
  string mode = argv[3];
  Mat img = imread(fileName, IMREAD_COLOR);

  if(mode == "S")
    quantizeImageSequential(img, size);
  if(mode == "M")
    quantizeImageParallel(img, size);

  return 0;
}