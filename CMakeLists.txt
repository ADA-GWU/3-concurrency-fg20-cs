cmake_minimum_required(VERSION 3.12)
project(Assignment_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Assignment3 concurrency.cpp)

find_package(OpenCV REQUIRED)
target_include_directories(Assignment3 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Assignment3 PRIVATE ${OpenCV_LIBS})

find_package(Threads REQUIRED)
target_link_libraries(Assignment3 PRIVATE Threads::Threads)