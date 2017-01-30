cmake_minimum_required(VERSION 2.8)
project( tf )
find_package( OpenCV REQUIRED )
add_executable(
    color_range color_range.cpp 
)
target_link_libraries(color_range  ${OpenCV_LIBS} )
