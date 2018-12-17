#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;

int main(int argc, char *argv[])
{
    VideoCapture cap(0);
    Mat frame;
    while (1)
    {
        cap >> frame;
        imshow("INPUT",frame);
        inRange(frame(0,127,0),Scalar(120))
        char c = waitKey(1);
        if(c == 27)
        {
            break;
        }
    }

    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("GG",src);

    waitKey(0);
}
