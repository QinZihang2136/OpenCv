#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst,frame,MOG2,KNN;

int main(int argc, char *argv[])
{
    VideoCapture cap(0);

    Ptr<BackgroundSubtractor>pMOG2 = createBackgroundSubtractorMOG2();
    Ptr<BackgroundSubtractor>pKNN = createBackgroundSubtractorKNN();
    namedWindow("INPUT",CV_WINDOW_AUTOSIZE);
    Mat kennel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));
    while (1)
    {
        cap >> frame;
        imshow("INPUT",frame);
        pMOG2 ->apply(frame,MOG2);
        morphologyEx(MOG2,MOG2,MORPH_OPEN,kennel,Point(-1,-1));
        pKNN ->apply(frame,KNN);
        imshow("KNN",KNN);
        imshow("MOG2",MOG2);
        char c = waitKey(1);
        if(c == 27)
        {
            break;
        }
    }


    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("GG",src);

    waitKey(0);
    return 0;
}
