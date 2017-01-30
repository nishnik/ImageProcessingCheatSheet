#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void erode_dilate_2x(Mat& final_img) {
    erode(final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}

int main( int argc, char** argv )
{
    // Creates a window for trackbars
    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    int iLowB = 117;
    int iHighB = 236;

    int iLowG = 73; 
    int iHighG = 160;

    int iLowR = 119;
    int iHighR = 255;
 
    //Create trackbars in "Control" window
    createTrackbar("LowB", "Control", &iLowB, 255); 
    createTrackbar("HighB", "Control", &iHighB, 255);
    createTrackbar("LowG", "Control", &iLowG, 255);
    createTrackbar("HighG", "Control", &iHighG, 255);
    createTrackbar("LowR", "Control", &iLowR, 255);
    createTrackbar("HighR", "Control", &iHighR, 255);

    while (true)
    {
        // Open the desired image
        Mat imgOriginal=imread("img.jpg");
         //Threshold the image
        Mat imgThresholded;
        inRange(imgOriginal, Scalar(iLowB, iLowG, iLowR), Scalar(iHighB, iHighG, iHighR), imgThresholded);
        // Removing Noise
        erode_dilate_2x(imgThresholded);
        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image
 
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            cout << "\n The values are (LowB, LowG, LowR), (HighB, HighG, HighR): ("<<iLowB<<", "<<iLowG<<", "<<iLowR<<"), ("<<iHighB<<", "<<iHighG<<", "<<iHighR<<")\n";
            break; 
        }
    }
   return 0;
}