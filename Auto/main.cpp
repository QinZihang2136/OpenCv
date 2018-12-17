#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;
Mat farme;
int main(int argc, char *argv[])
{
    VideoCapture video;
    video.open(0);
    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("GG",src);
    while(1)
    {
        video >> farme;
        imshow("video",farme);
        waitKey(1);
    }

    waitKey(0);
}
