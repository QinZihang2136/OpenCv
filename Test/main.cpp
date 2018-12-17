#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
int value = 0;
int max_value = 25;
const char windows[] = "BLUR";
Mat src,gray,dst;
void callback(int,void*);
int main(int argc, char *argv[])
{

    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    VideoCapture cap(0);
    namedWindow(windows);
    createTrackbar("SIZE",windows,&value,max_value,callback);
    callback(0,0);
    while (1)
    {
        cap >> src;
        imshow("GG",src);
        imshow(windows,dst);
        waitKey(1);
    }

    waitKey(0);
}
void callback(int, void*)
{
    GaussianBlur(src,dst,Size(value*2+1,value*2+1),0,0);
}
