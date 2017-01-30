#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stack>
#include <fstream>
using namespace cv;
using namespace std;

// Click mouse left button to insert in queue, right button if inserted wrong
vector<Point> clicks;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        clicks.push_back(Point(x,y));
    }
    else if ( event == EVENT_RBUTTONDOWN )
    {
      cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
      clicks.pop_back();
    }
}

template < typename T >
void pop_front(std::vector < T > & vec)
{
    if (vec.empty())
        return;
    vec.erase(vec.begin());
}

int main( int argc, char** argv )
{
    // Load the image
    Mat img = imread("../color_range/img.jpg");
    namedWindow("My Window", 1);
    setMouseCallback("My Window", CallBackFunc, NULL);
    imshow("My Window", img);
    waitKey(0);
    destroyWindow("My Window");
    // write them to read it from another program
    ofstream fout("order_new_test.dat", ios::out | ios::binary);
    fout.write(reinterpret_cast<const char*>(&clicks[0]), clicks.size()*sizeof(Point));
    fout.close();
    return 0;
}