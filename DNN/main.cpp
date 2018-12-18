#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;

int main(int argc, char *argv[])
{

    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("GG",src);

    waitKey(0);
}
