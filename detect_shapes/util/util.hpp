#pragma once
#include "util.ih"

stack<pair<int,int> > clicks;//for init_crop

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		clicks.push(make_pair(x,y));
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		clicks.pop();
	}
}

bool VAL_PRE =true;

Mat init_crop()
{
	Mat img = imread("img1.jpg");
	Point a, b;
	if (not VAL_PRE) {
	namedWindow("My Window", 1);
	setMouseCallback("My Window", CallBackFunc, NULL);
	imshow("My Window", img);
	waitKey(0);
	destroyWindow("My Window");
	pair<int,int> c= clicks.top();
	a.x = c.first;
	a.y =c.second; 
	clicks.pop();
	c= clicks.top();
	b.x = c.first;
	b.y = c.second;
	}
	else {
		a = Point(56.17);
		b= Point(607,432);
	}
	Rect crop(a,b);
	img = img(crop);
	std::cout<< "crop points\n";
	std::cout<<a.x<<" "<<a.y<<"\n";
	std::cout<<b.x<<" "<<b.y<<"\n";
	return img;
}

Point detectBallCenter_fin(const Mat& img)//,int thresh1,int thresh2)
{
	//	Point init;
	//	Point p[4]={Point(),Point(),Point(),Point()};
	//	Point init,center;
	bool done = false;
	int i,j;
	for(i=0;i<img.rows;i++)
	{
	for(j=0;j<img.cols;j++)
	{
	    if(img.at<uchar>(i,j)==255)
	    {
	//		    	p[0]=init = Point(i,j);
	    	cout << "init point: " << i << ' ' << j << '\n';
	    	done = true;break;
	    }
	}
	if(done)	
		break;
	}
	//we got first point
	int ii = i, jj = j;//,ar_i[10];
	vector<int> ar_i;//,ar_j;
	for(;j<img.cols;j++)
	{
	int i1=i,i2=i;
	while(i1>=0 && img.at<uchar>(i1,j)!=0){
		--i1;
	}
	while(i2<img.rows && img.at<uchar>(i2,j)!=0){
		++i2;
	}
	cout << "next point: " << i << " " << j << '\n';
	i = (i1+i2) /2 ;
	if(img.at<uchar>(i,j)==0)
		break;
	ar_i.push_back(i);	
	}
	Point center(0,0);
	center.y= (j+jj)/2;
	int& cx = center.x;
	for(int k=0;k<ar_i.size();++k)
	{
	cx += ar_i[k];
	}
	cx/=ar_i.size();


	float radi = (center.y - jj ) ;
	namedWindow("circle_ceter",CV_WINDOW_AUTOSIZE);
	Mat temp(img.rows,img.cols,CV_8UC3,Scalar(255,255,255));
	cout << center.x << ' ' << center.y << ' ' << radi << '\n';
	circle(temp,center,3,Scalar(255,0,255));
	imshow("circle_ceter",temp);
	return center;

}





/*
void hsv(const Mat& img)
{
Mat imgHSV;
Mat imgOriginal=img.clone(),imgShow;
cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

namedWindow("Control", MY_WINDOW_TYPE);
int iLowH = 0;
int iHighH = 255;

int iLowS = 0; 
int iHighS = 255;

int iLowV = 32;//alwa--> for dark areas
int iHighV = 255;//always--> for bright areas

int thresh1 = 0; 
int thresh2 = 255;
namedWindow("1");
namedWindow("orig");
imshow("orig",img);
createTrackbar("LowB", "Control", &iLowH, 179); //Hue (0 - 179)
createTrackbar("HighB", "Control", &iHighH, 179);

createTrackbar("LowG", "Control", &iLowS, 255); //Saturation (0 - 255)
createTrackbar("HighG", "Control", &iHighS, 255);
//for CAnny
createTrackbar("lowR", "Control", &iLowV, 255);
createTrackbar("highR", "Control", &iHighV, 255);
while(1){
inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgShow); //Threshold the image
imshow("1",imgShow);

waitKey(50);
}
}
*/
