#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <vector>
using namespace std;
using namespace cv;

int limit(int x)
{
  return (x > 255) ? 255 : x;
}

int findSum(vector<int> vect, int start, int end){
  int sum = 0;
  for (int i = start; i < end; i++)
    sum += vect[i];
  return  sum;
}

Vec3b rgbAverage(Mat img, int x, int y, int size_x, int size_y)
{
  int r = 0, g = 0, b = 0;
  for (int i = y; i < (y + size_y); i++)
  {
    for (int j = x; j < (x + size_x); j++)
    {
      r += img.at<Vec3b>(i, j)[0];
      g += img.at<Vec3b>(i, j)[1];
      b += img.at<Vec3b>(i, j)[2];
    }
  }
  Vec3b color;
  color[0] = limit(r / (size_x * size_y));
  color[1] = limit(g / (size_x * size_y));
  color[2] = limit(b / (size_x * size_y));

  return color;
}

void quantize(Mat img, int col, int row, int size_col, int size_row){
  Vec3b color;
  color = rgbAverage(img, col, row, size_col, size_row);
  for (int i = row; i < row + size_row; i++)
  {
    for (int j = col; j < col + size_col; j++)
    {
      img.at<Vec3b>(i, j) = color;
    }
  }
}

void threadQuantize(Mat img, int x, int y, int rows, int size_x, int size_y){

  int fittedCols = img.cols - img.cols % size_x;
  for (int i = 0; i < rows; i++, y+=size_y)
  {
    if (x < (fittedCols))
      quantize(img, x, y, size_x, size_y);
    else
      quantize(img, x, y, img.cols % size_x, size_y);
  }
  
}

void quantizeImageParallel(Mat img, int size){

  namedWindow("Image", WINDOW_NORMAL);
  unsigned int nThreads = thread::hardware_concurrency();
  if (nThreads == 0)
    nThreads = 4;
  vector<thread> threads;

  vector<int> SegementRows;
  SegementRows.resize(nThreads);
  fill(SegementRows.begin(), SegementRows.end(), 0);

  int imgRows = img.rows / size, i = 0;
  for (; imgRows > 0; i++, imgRows--)
  {
    if (i == nThreads)
      i = 0;
    SegementRows[i]++;
  }

  for (int x = 0; x < img.cols; x += size)
  {
    
    for (int t = 0; t < nThreads; t++)
    {
      if(t==0) threads.push_back(thread(threadQuantize, img, x, 0, SegementRows[t], size, size));
      else threads.push_back(thread(threadQuantize, img, x, size*findSum(SegementRows, 0, t), SegementRows[t], size, size));
    }
      
    for (int j = 0; j < nThreads; j++)
    {
        threads[j].join();
    }
      
    for (int j = 0; j < nThreads; j++)
    {
      threads.pop_back();
    }
    imshow("Image", img);
    waitKey(10);
    imwrite("result.jpg", img);
  }

  int rest = img.rows % size; cout << rest << endl; cout << img.rows << endl;
  if (rest > 0){
    thread t;
    int pos_y = img.rows - rest;
    for (int x = 0; x < img.cols; x += size)
    {
      t = thread(threadQuantize, img, x, pos_y, 1, size, rest);
      t.join();
    }
    imshow("Image", img);
    waitKey(10);
    imwrite("result.jpg", img);
  }

  imshow("Image", img);
  waitKey(0);
  imwrite("result.jpg", img);
}

void quantizeImageSequential(Mat img, int size)
{
  int height = img.rows;
  int width = img.cols;

  namedWindow("Image", WINDOW_NORMAL);
  int x, y;
  Vec3b color;
  for (y = 0; y < height; y += size)
  {
    for (x = 0; x < width; x += size)
    {
      imshow("Image", img);
      if (x < (width - width % size) && y < (height - height % size))
      {
        quantize(img, x, y, size, size);
      }
      if (x < (width - width % size) && y >= (height - height % size))
      {
        quantize(img, x, y, size, height % size);
      }
      if (x >= (width - width % size) && y < (height - height % size))
      {
        quantize(img, x, y, width % size, size);
      }
      else
      {
        quantize(img, x, y, width % size, height % size);
      }
      waitKey(1);
    }
  }
  imshow("Image", img);
  waitKey(0);
  imwrite("result.jpg", img);
}

int main(int argc, char *argv[])
{

  if (argc < 4)
  {
    cout << "Error: Not enough arguments! See the usage below:" << endl
         << endl;
    cout << "<program> [file] [size] [mode]" << endl
         << endl;
    cout << "\tfile \tThe name of the jpg file containing the image" << endl;
    cout << "\tsize \tThe side of the square for the averaging" << endl;
    cout << "\tmode \tProcessing mode which can be 'S' - single threaded and 'M' - multi threaded" << endl
         << endl;
    exit(0);
  }

  cout << "Good to go!" << endl;
  string fileName = argv[1];
  int size = stoi(argv[2]);
  string mode = argv[3];
  Mat img = imread(fileName, IMREAD_COLOR);

  if (mode == "S")
    quantizeImageSequential(img, size);
  if (mode == "M")
    quantizeImageParallel(img, size);
 
  destroyAllWindows();
  return 0;
}

