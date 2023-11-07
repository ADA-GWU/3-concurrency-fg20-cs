#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;


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
  string image_path = samples::findFile(file_name);
  Mat img = imread(image_path, IMREAD_COLOR);

  if(img.empty()){
    cout << "Could not read the image: " << image_path << endl;
    return 1;
  }

  imshow("Display window", img);
  int k = waitKey(0); // Wait for a keystroke in the window
  if(k == 's')
    imwrite("starry_night.png", img);
  return 0;
}



// int main(int argc, char** argv) {

//     // Load the video file
//     VideoCapture cap("/Users/fidanguliyeva/Downloads/file_example_MP4_480_1_5MG.mp4");

//     // Check if the video file was opened successfully
//     if (!cap.isOpened()) {
//         cerr << "Error: could not open video file" << endl;
//         return -1;
//     }

//     // Create a window to display the video frames
//     namedWindow("Video", WINDOW_NORMAL);

//     // Loop over the video frames and display them in the window
//     while (true) {

//         // Read the next frame from the video file
//         Mat frame;
//         cap.read(frame);

//         // Check if the frame was read successfully
//         if (frame.empty()) {
//             break;
//         }

//         // Display the current frame in the window
//         imshow("Video", frame);

//         // Wait for a key press (or 30 milliseconds) to allow the frame to be displayed
//         if (waitKey(30) >= 0) {
//             break;
//         }
//     }

//     // Release the video file and destroy the window
//     cap.release();
//     destroyAllWindows();

//     return 0;
// }