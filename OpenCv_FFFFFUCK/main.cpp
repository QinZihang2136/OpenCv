#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;
void callback (int pos, void*);
int main(int argc, char *argv[])
{

    src =imread("/home/qinzihang/OpenCV_Picture/detect/1.png");
    GaussianBlur(src,dst,Size(5,5),11);
    threshold(dst, dst, 30, 255, CV_THRESH_BINARY);
    imshow("GG",dst);

    waitKey(0);
}
