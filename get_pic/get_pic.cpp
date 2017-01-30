#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    // Open usb video capture
    VideoCapture vcap(1); 
    if(!vcap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    while(1) {
        Mat frame;
        vcap >> frame;
        imshow( "Frame", frame );
        if (waitKey(33) == 27) {
            // On pressing 'ESC' -> write to img.jpg
            imwrite("img.jpg", frame);
            break;
        }
    }
    return 0;
}