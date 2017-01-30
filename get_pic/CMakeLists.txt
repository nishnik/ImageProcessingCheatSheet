cmake_minimum_required(VERSION 2.8)
project( tf )
find_package( OpenCV REQUIRED )
add_executable (
	get_pic 
	get_pic.cpp 
)
target_link_libraries(get_pic  ${OpenCV_LIBS} )
