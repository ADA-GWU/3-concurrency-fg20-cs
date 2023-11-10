#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <vector>
// #include <omp.h>
using namespace std;
using namespace cv;

int limit(int x){
  return (x > 255)? 255 : x;
}

Vec3b rgbAverage(Mat img, int x, int y, int size){
  int r = 0, g = 0, b = 0;
  for (int i = y; i < (y+size); i++) {
    for (int j = x; j < (x+size); j++){
      r += img.at<Vec3b>(i, j)[0];
      g += img.at<Vec3b>(i, j)[1];
      b += img.at<Vec3b>(i, j)[2];
    }
  }
  Vec3b color;
    color[0] = limit(r / (size*size));
    color[1] = limit(g / (size*size));
    color[2] = limit(b / (size*size));

  return color;
}

int addition(int q, int div){
  int rest = div - (q % div);
  if(rest == div) return 0;
  return rest;
}

void copyTo(Mat source, Mat dest){
  int sRows = source.rows, sCols = source.cols;
  dest.setTo(Scalar(0,0,0));
  for (int i = 0; i < sRows; i++){
    for (int j = 0; j < sCols; j++){
      dest.at<Vec3b>(i, j) = source.at<Vec3b>(i, j);
    }
  }
}

Mat preprocess(Mat img, int size){
  
  int newRows = img.rows + addition(img.rows, size);
  int newCols = img.cols + addition(img.cols, size);
  Mat newImage(newRows, newCols, CV_8UC3);
  copyTo(img, newImage);

  return newImage;
}

// void quantizeImageParallel(Mat newImage, int yPos1, int blockSize,  int size, string windowName){
  
//   Vec3b color;
//   int y;
//   for (y = yPos1; y < (yPos1 + yPos1*blockSize); y+=size) {
//     for (int x = 0; x < newImage.cols; x+=size) {
//       // imshow(windowName, newImage);
//       color = rgbAverage(newImage, x, y, size);
//       for (int i = y; i < y+size; i++) {
//         for (int j = x; j < x+size; j++){
//           newImage.at<Vec3b>(i, j) = color;
//         }
//       }
//       // int k = waitKey(1);
//     }
//   }
//   cout << "Thread id: " << yPos1 << endl;
//   cout << "current last pos: " << y << endl;
//   cout << "total rows: " << newImage.rows << endl;
//   if ((newImage.rows - y) < blockSize){
//     for (;y < newImage.cols; y+=size) {
//       for (int x = 0; x < newImage.cols; x+=size) {
//         // imshow(windowName, newImage);
//         color = rgbAverage(newImage, x, y, size);
//         for (int i = y; i < y+size; i++) {
//           for (int j = x; j < x+size; j++){
//             newImage.at<Vec3b>(i, j) = color;
//           }
//         }
//         // int k = waitKey(1);
//       }
//     }
//   }
  
//   // imshow(windowName, newImage);
//   // int key = waitKey(0);
// }


void quantizeImage(Mat img, int size){

  int height = img.rows;
  int width = img.cols;

  int newRows = height + addition(height, size);
  int newCols = width + addition(width, size);
  Mat newImage(newRows, newCols, CV_8UC3);

  cout << newImage.rows << endl;
  cout << newImage.cols << endl;

  copyTo(img, newImage);

  cout << newImage.rows << endl;
  cout << newImage.cols << endl;

  namedWindow("Image", WINDOW_NORMAL);
  resizeWindow("Image", 600, 600);
  Vec3b color;
  for (int y = 0; y < newRows; y+=size) {
    for (int x = 0; x < newCols; x+=size) {
      imshow("Image", newImage);
      color = rgbAverage(newImage, x, y, size);
      for (int i = y; i < y+size; i++) {
        for (int j = x; j < x+size; j++){
          newImage.at<Vec3b>(i, j) = color;
        }
      }
      int k = waitKey(1);
    }
  }
  imshow("Image", newImage);
  int key = waitKey(0);
  imwrite("result.jpg", newImage);
}

void displayImage(Mat img){
  imshow("Image", img);
  waitKey(0);
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
        color = rgbAverage(img, x, y, size);
        for (int i = y; i < y+size; i++) {
          for (int j = x; j < x+size; j++){
            img.at<Vec3b>(i, j) = color;
          }
        }
      }
      if(x >= (width-width%size) && y >= (height-height%size)){
        for (int i = y; i < height; i++) {
          for (int j = x; j < width; j++){
            img.at<Vec3b>(i, j) = color;
          }
        }
      }
      if(x >= (width-width%size) && y < (height-height%size)){
        for (int i = y; i < y+size; i++) {
          for (int j = x; j < width; j++){
            img.at<Vec3b>(i, j) = color;
          }
        }
      }
      if(x < (width-width%size) && y >= (height-height%size)){
        for (int i = y; i < height; i++) {
          for (int j = x; j < x+size; j++){
            img.at<Vec3b>(i, j) = color;
          }
        }
      }
      waitKey(1);
    }
  }
  imshow("Image", img);
  waitKey(0);
  imwrite("result.jpg", img);
}

void splitImage(vector<Mat>& segments, Mat img, int blockSize){
  int x = 0;
  for (int i = 0; i < segments.size()-1; i++){
    segments[i] = Mat(img, Rect(0, x, img.cols, blockSize));
    x += blockSize;
  } 
}

void quantizeImageParallel(Mat img, int col, int row, int size){
  Vec3b color;
  color = rgbAverage(img, col, row, size);
  for (int i = row; i < row+size; i++) {
    for (int j = col; j < col+size; j++){
      img.at<Vec3b>(i, j) = color;
    }
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
  string fileName = argv[1];
  int size = stoi(argv[2]);
  string mode = argv[3];
  Mat img = imread(fileName, IMREAD_COLOR);

  if(mode == "S")
    quantizeImageSequential(img, size);
  if(mode == "M"){
    namedWindow("Image", WINDOW_NORMAL);

    unsigned int nThreads = thread::hardware_concurrency();
    if(nThreads == 0) nThreads = 4;


    int segmentRow = (img.rows - img.rows%size)/nThreads;

    vector<thread> threads;

    for(int k = 0; k < segmentRow; k+=size){
      for (int x = 0; x < img.cols; x+=size){
        int i = 0;
        for (int y = k; i < nThreads; i++, y = k+i*segmentRow){
          threads.push_back(thread(quantizeImageParallel, img, x, y, size));
        }
        cout << threads.size()<< endl;

        imshow("Image", img);
        waitKey(10);
        imwrite("result.jpg", img);

        for (int j = 0; j < nThreads; j++){
          threads[j].join(); 
        }
        for (int j = 0; j < nThreads; j++){
          threads.pop_back();
        }
      }
    }
    imshow("Image", img);
    waitKey(0);
    imwrite("result.jpg", img);
    

    

    // for(int x = 0; x < img.cols; x+=size){
    //   for (int y = i*segmentRow; y < (i*segmentRow + segmentRow); y+=size, i++){
    //     thread t(quantizeImageParallel, img, x, y, size);
    //   }
    // }

    // imshow("Image", img);
    // waitKey(0);
    // imwrite("result.jpg", img);









    // int blockSize = size * img.rows / (size*nThreads);

    // vector<Mat> img_segments;
    // img_segments.resize(nThreads+1);
    // splitImage(img_segments, img, blockSize);

    // vector<thread> threads;

    // for (int x = 0; x < img.cols; x+=size) {
    //   for (int i = 0; i < nThreads; i++){
    //       // imshow("Image", img_segments[i]);
    //       // waitKey(1);
    //     for(int y = 0; y < img.rows; y+=size)
    //       threads.push_back(thread(quantizeImageParallel, img, size, x,y));

    //     threads[i].join();

    //   }

    // }
    // imwrite("result.jpg", img);
  }

  return 0;
}