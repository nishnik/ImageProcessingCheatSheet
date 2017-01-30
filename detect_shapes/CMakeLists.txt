set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)
project( tf )
find_package( OpenCV REQUIRED )
add_executable(
	detect_objects
	detect_objects.cpp 
)
target_link_libraries(detect_objects  ${OpenCV_LIBS} )
