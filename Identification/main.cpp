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
void  mouseRectHandler(int event, int x, int y, int flags, void* param)
{

    bool one = false;
    bool two = false;
    Point point1;
    Point point2;
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        lftd = true;
        point1.x =x;
        point1.y =y;
        one = true;
        cout << "55";
        printf("555");
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

int main(int argc, char *argv[])
{
    VideoCapture cap(0);
    namedWindow("T");
    setMouseCallback("T",mouseRectHandler,0);

    while(1)
    {
        cap >> frame;
        rectangle(frame,box,Scalar(255,255,0),2);
        imshow("T",frame);

   // setMouseCallback("T",onmouse,0);
        waitKey(10);

    }

}
//void onmouse(int event, int x, int y, int flags, void *img)
//{
//    bool one = false;
//    bool two = false;
//    Point point1;
//    Point point2;
//    if(event==CV_EVENT_LBUTTONDOWN)
//    {
//        lftd = true;
//        point1.x =x;
//        point1.y =y;
//        one = true;
//        cout << "55";
//        printf("555");
//    }
//    if(event == CV_EVENT_LBUTTONUP && lftd == true)
//    {
//        point2.x =x;
//        point2.y =y;
//        two = true;
//        lftd = false;
//    }
//    if(two && one)
//    {
//        box =Rect(point1,point2);
//        rectangle(frame,box,Scalar(255,0,0));
//        imshow("T",frame);
//    }


//}
