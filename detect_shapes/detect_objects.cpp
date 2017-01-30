#include <iostream>
#include <stack>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <utility>
using namespace cv;
using namespace std;
#include "util/util.hpp"
#include "util/detect_object.hpp"
int factor_x,factor_y;

int main()
{
	// If you crop image, remember to crop with same rectangle
	// values where you write code for running the bot, or where you use these positions.
	Mat img = init_crop(); 
	// If you don't want to crop do this:
	// Mat img = imread("conquest.png");
	if(!img.data){
		cout << "Cannot read image\nExiting now\n\n";
		return -1;
	}
	_do(img);
	return 0;
}
