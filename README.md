# Assignement 3: Concurrency - Image quantization

## Project overview

This project demonstrates the application of concurrency in image processing using STL threads and the OpenCV library. The primary goal is to perform image quantization, a process of reducing the number of colors in an image, in two distinct modes: sequential and concurrent.

### Tools

- **OpenCV** (Open Source Computer Vision Library): widely used for operations on images and videos. In this project, OpenCV is used for image processing and rendering.
- **STL** (Standard Template Library): used STL threads for handling the image processing in concurrent mode.

## Setup Prerequisites

For successful compilation and execution, the installations listed below are required:

- **C++ Compiler** (modern C++ compiler that supports C++11 standards or later)
- **OpenCV**
- **CMake** (used for building the application)

### Installing cmake

- For macOS, run:
```bash
brew install cmake
```
- For Linux, follow instructions [here](`https://cgold.readthedocs.io/en/latest/first-step/installation.html`)
- For Windows, follow instructions [here](`https://dev.to/arun3sh/install-cmake-on-windows-4eo5`)

### Installing OpenCV

- For macOS, run:

```bash
brew install opencv
```

- For Linux, follow instructions [here](`https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html`)
- For Windows, follow instructions [here](`https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html`)

## Compilation

After cloning the project and navigating to project directory, run the following commands to compile it:

```bash
cd build
```

```bash
cmake ..
```

```bash
make
```

## Running

To run the application, follow the template below:

```bash
./Assignment3 <image_path> <size> <exec_mode>
```
* *image_dir*: path to the file containing the image
* *size*: size of the kernel for image processing (if set to n, then the size is nxn square)
* *exec_mode*: single-threaded - 'S'; multi-threaded - 'M'.

__*Note*__: There are sample images in the `build` folder.

To close the image window at the end of the image processing, press any key.

## How Concurrency was achieved

In the provided concurrency.cpp file, the project implements concurrent execution for the task of image quantization. Here's a breakdown of the key steps:

* The program first determines the number of threads available on machine using `std::thread::hardware_concurrency()` function. If it returns 0 (which means it couldn't determine the number), a default value of 4 is used.

* Then, the program calculates the height of each segment to be passed to each thread. This heights are stored in  the vector of integers `SegementRows`. 

* Next, the program interates over each column of the image and sets threads at the begining of each segment. The threads execute `threadQuantize` function which quantizes a column of the segment.

* At the end of quantization of each column, the threads are joined and the image is re-rendered.  
