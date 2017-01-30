set (CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 2.8)
project( tf )
find_package( OpenCV REQUIRED )
add_executable (
	manual_anno
	manual_anno.cpp 
)
target_link_libraries(manual_anno  ${OpenCV_LIBS} )
