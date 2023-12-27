cmake_minimum_required(VERSION 3.22.1)
project( video_processing )
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
message("CMAKE_CXX23_STANDARD_COMPILE_OPTION = ${CMAKE_CXX23_STANDARD_COMPILE_OPTION}")
#set(CMAKE_CXX_EXTENSIONS Off)
add_subdirectory(src)

#cmake . -DCMAKE_BUILD_TYPE=Debug/Release