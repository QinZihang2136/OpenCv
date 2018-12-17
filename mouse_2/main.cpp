#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst,frame;
bool drawing_box;
bool lftd = false;
bool lftu = true;
Rect box;
void onmouse(int event, int x, int y, int flags, void *img);

int main(int argc, char *argv[])
{

    namedWindow("GG", CV_WINDOW_AUTOSIZE);
    setMouseCallback("GG",onmouse,0);
    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    frame =src;
    imshow("GG",frame);

    waitKey(0);
}
void onmouse(int event, int x, int y, int flags, void *img)
{
    bool one = false;
    bool two = false;
    Point point1;
    Point point2;
    if(event==CV_EVENT_MOUSEMOVE)
    {
        lftd = true;
        point1.x =x;
        point1.y =y;
        one = true;
        cout << point1;
    }
    if(event == CV_EVENT_LBUTTONUP && lftd == true)
    {
        point2.x =x;
        point2.y =y;
        two = true;
        lftd = false;
    }
    if(two && one)
    {
        box =Rect(point1,point2);
        rectangle(frame,box,Scalar(255,0,0));
        imshow("T",frame);
    }


}
