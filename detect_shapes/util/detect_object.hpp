// firstly run color_range and get the color range for tower and shapes.
// move order.dat to run_bot
#pragma once
#include "util.ih"
//#define MY_WINDOW_TYPE CV_WINDOW_AUTOSIZE
#define MY_WINDOW_TYPE 0
#define DEBUG 3
#include <fstream>
#include <iterator>
#include <set>
RNG rng(12345);

int THRESH_AREA = 300;
vector<Point > shapeImages;
Point tower_center;

Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
int angle(const Point B) { // angle from towncenter
    Point A = tower_center;
    double val;
    if (B.x == A.x) {
        if (B.y > A.y)
        return 90;
        else
        return 270;
    }
    else 
        val = double(B.y-A.y)/double(B.x-A.x); // calculate slope between the two points
        val = atan(val);
    int val_i = ((int)(val*180/3.14)) % 360; // Convert the angle in radians to degrees
    return val_i;
}

// Distance from Town Center
int dist(Point p) {
    p.x = p.x - tower_center.x;
    p.x *= p.x;
    p.y = p.y - tower_center.y;
    p.y *= p.y;
    return (p.x + p.y);
}

// Distance between two points
int dist2(Point p, Point q) {
    p.x = p.x - q.x;
    p.x *= p.x;
    p.y = p.y - q.y;
    p.y *= p.y;
    return (p.x + p.y);
}

struct cmp1 {
    bool operator() (const Point& lhs, const Point& rhs) const{
        return dist(lhs) < dist(rhs);
    }
};


struct cmp2 {
    bool operator() (const Point& lhs, const Point& rhs) const{
        return angle(lhs) < angle(rhs);
    }
};



namespace detect_object
{
    namespace object_type
    {
        static const int town_center=0;
        static const int triangles_and_squares=1;
    };

    inline void erode_dilate_2x(Mref final_img)
    {
        erode(final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( final_img, final_img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    }

    void get(int x,cMref imgOriginal,Mref final_img)
    {
        using namespace object_type;
        switch(x)
        {
        case triangles_and_squares:
            inRange(imgOriginal, Scalar(0, 76, 91), Scalar(144, 255, 255), final_img);
            break;
        case town_center:
            inRange(imgOriginal, Scalar(18, 33, 38), Scalar(64, 59, 75), final_img);
            break;
        }
        erode_dilate_2x(final_img);
        erode_dilate_2x(final_img);
    }
};

void detectShapes(Mat canny_output)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly(contours.size());

    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    drawContours( drawing, contours, -1, color);
    for(int i = 0; i< contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        Point center = contours_poly[i][0];

        for(int j=1;j<contours_poly[i].size();j++)
        {
            center.x += contours_poly[i][j].x;
            center.y += contours_poly[i][j].y;
        }
        center.x /= contours_poly[i].size();
        center.y /= contours_poly[i].size();
        if (contours_poly[i].size() > 2) {
            shapeImages.push_back(center);
        }
        else {
            std::cout << "POSSIBILIITY OF ERROR FROM DETECT SHAPES";
        }
        std::cout<<"\n";
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        circle( drawing, center, 3, color, 1, 8, 0 );
    }
    imshow( "Contours", drawing );
}


Point getCenter(Mat imgOriginal)
{
    Mat canny_output = imgOriginal;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly(contours.size());

    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;
    for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
    {
        double a=contourArea( contours[i],false);  //  Find the area of contour
        if(a>largest_area)
        {
            largest_area=a;
            largest_contour_index=i;                //Store the index of largest contour
            bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
        }
    }
    Scalar color( 255,255,255);
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    Mat src = Mat::zeros( canny_output.size(), CV_8UC3 );

    drawContours( drawing, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
    rectangle(drawing, bounding_rect,  Scalar(0,255,0),1, 8,0);  
    if (largest_area < THRESH_AREA)
       std::cout<<"POSSIBILIITY OF ERROR from getCenter";
    return Point((bounding_rect.x + bounding_rect.width/2), (bounding_rect.y + bounding_rect.height/2));
}

using namespace detect_object;
using namespace object_type;

void _do(const Mat& imgOriginal)
{
    imshow("Original Image", imgOriginal);

    // TODO: Document here
    int tot=2;
    Mat im[2];
    while(tot--)
    {
        string s = "0";
        s[0]+=tot;
        detect_object::get(tot,imgOriginal,im[tot]);
#if DEBUG > 2
        namedWindow(s,MY_WINDOW_TYPE);
        imshow(s,im[tot]);
#endif
    }
     int thresh1=0;
     int thresh2=255;

    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    createTrackbar("thresh1", "Control", &thresh1, 255);
    createTrackbar("thresh2", "Control", &thresh2, 255);
    Mat canny;
    while (true)
    {
        // TODO: maybe some bug here no change on thresh chnge
        Mat imgOriginal=im[triangles_and_squares];
        Mat canny_output;
        Canny( imgOriginal, canny_output, thresh1, thresh2, 3 );
        imshow("canny",canny_output);
        imshow("Original", imgOriginal); //show the original image
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            canny = canny_output;
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }       
    detectShapes (canny);
    erode_dilate_2x(im[0]);
    tower_center = getCenter(im[0]);

    // Now calculate the order
    vector<Point> Que;
    Que.push_back(tower_center);

    std::cout<< "\n triangles_and_squares size"<< shapeImages.size()<<"\n";

    set<Point, cmp1> all_set(shapeImages.begin(), shapeImages.end());
    std::cout<< "\n all_set size"<< all_set.size()<<"\n";

    std::set<Point>::iterator it, it2;
    it2 = all_set.begin();
    ++it2;

    std::vector<set<Point>::iterator> to_erase;
    for (it = all_set.begin(); it2 != all_set.end(); ++it, ++it2)
    {
        int a = dist2(*it, *it2);
        if (a < 10) {
            to_erase.push_back(it2);
        }
    }
    for (auto it = to_erase.begin(); it != to_erase.end(); ++it) {
        all_set.erase(*it);
    }

    std::cout<< "\n all_set size"<< all_set.size()<<"\n";
    
    set<Point, cmp2> all_set_new(all_set.begin(), all_set.end());
    std::cout<< "\n all_set_new size"<< all_set_new.size()<<"\n";
    
    int r = 2;


    for (auto it = all_set_new.begin(); it != all_set_new.end(); ++it)
    {
        std::cout<<(*it).x<< " " <<(*it).y << "\n"; // Note the "*" here
        circle( canny, (*it), r, color, 1, 8, 0 );
        r += 1;
        Que.push_back(*it);
        Que.push_back(tower_center);
    }

    for (auto it = all_set_new.begin(); it != all_set_new.end(); ++it)
    
    {
        std::cout<<(*it).x<< " " <<(*it).y << "\n"; // Note the "*" here
        circle( canny, (*it), r, color, 1, 8, 0 );
        r += 1;
        Que.push_back(*it);
        Que.push_back(tower_center);
    }

    std::cout << "\n Que"<<Que.size()<<"\n";
    circle (imgOriginal, tower_center, r, color, 1, 8, 0);
    imshow("canny", canny);
    imshow("Original", imgOriginal);
    waitKey(0);
    


    ofstream fout("order.dat", ios::out | ios::binary);
   
    fout.write(reinterpret_cast<const char*>(&Que[0]), Que.size()*sizeof(Point));
    fout.close();

    // For reading the vector do this:
    // std::vector<Point> vec(count);
    // strm.read(reinterpret_cast<char*>(&vec[0]), count*sizeof(Point));
}
