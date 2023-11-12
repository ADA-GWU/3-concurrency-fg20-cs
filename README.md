# Assignement 3: Concurrency - Image quantization

## Project overview

The aim of the project is to quantize a JPG image in two modes: sequential and concurrent.

## Set Up
To run this application, 2 installations are required:
- opencv
- cmake

### cmake
- For macOS, run `brew install cmake`
- For Linux, follow instructions in `https://cgold.readthedocs.io/en/latest/first-step/installation.html`
- For Windows, follow instructions in `https://dev.to/arun3sh/install-cmake-on-windows-4eo5`

### OpenCV
- For macOS, run `brew install opencv`
- For Linux, follow instructions in `https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html`
- For Windows, follow instructions in `https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html`

## Run the application

```cd build```

```cmake ..```

```make```

```./Assignment3 cat.jpg 100 S```

```./Assignment3 cat.jpg 100 M```

To exit the execution and close the image window, press any key.
