#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>
#include <termio.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>      // standard input / output functions
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitionss
#include <iostream>
using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;

int main(int argc, char *argv[])
{

    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("GG",src);
ioc
    waitKey(0);
}
