#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst,frame,prev_frame;
vector<Point2f> features;
void detecFeatures(Mat &inFrame,Mat &ingray);
void drawFeature(Mat &inFrame);
int main(int argc, char *argv[])
{

    VideoCapture cap(0);
    while (1)
    {
        cap >> frame;
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        detecFeatures(frame,gray);
        drawFeature(frame);
        imshow("INPUT",frame);
        waitKey(1);

    }
    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("GG",src);

    waitKey(0);
}

void detecFeatures(Mat &inFrame,Mat &ingray)
{
    double maxCorners = 5000;
    double qualitylevel = 0.01;
    double minDistance = 10;
    double blockSize = 3;
    double k =0.04;
    goodFeaturesToTrack(ingray, features, maxCorners,qualitylevel,minDistance,Mat(),blockSize,false,k);

}

void drawFeature(Mat &inFrame)
{
    for (size_t t = 0; t < features.size(); t++)
    {
        circle(inFrame,features[t],2,Scalar(0,0,255),2,0);

    }
}
