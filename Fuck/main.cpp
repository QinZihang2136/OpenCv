#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Point point1,point2;
bool leftd = false;
bool leftu = true;
Mat roi;
Mat src,gray,dst;
Rect box;

void callback(int event, int x,int y, int flags, void* param);
Mat templateImage = imread("/home/qinzihang/OpenCV_Picture/detect/1.png");

int main(int argc, char *argv[])
{
    namedWindow("GG",WINDOW_AUTOSIZE);
    setMouseCallback("GG",callback);
    src =imread("/home/qinzihang/OpenCV_Picture/detect/test.png");

    VideoCapture cap(0);

       while(1)
       {
//        cap >> src;
//        int result_cols =  src.cols - templateImage.cols + 1;
//        int result_rows = src.rows - templateImage.rows + 1;
//        Mat result = Mat( result_cols, result_rows, CV_32FC1 );
//           matchTemplate( src, templateImage, result, CV_TM_SQDIFF );

        double minVal, maxVal;
        Point minLoc, maxLoc, matchLoc;
//        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
//        matchLoc = minLoc;

        rectangle( src, cv::Rect(matchLoc, cv::Size(templateImage.cols, templateImage.rows) ), Scalar(0, 0, 255), 2, 8, 0 );

        imshow("", src);

    imshow("GG",src);
    waitKey(1);
}

}
void callback(int event, int x,int y, int flags, void* param)
{
    bool mark = false;

    if(event == CV_EVENT_LBUTTONDOWN)
    {
        cout<< "left button is down" << endl;
         point1= Point(x,y);
         cout << "point1 is " << point1;
         leftd =true;
         leftu = false;

    }

    if(event == CV_EVENT_LBUTTONUP && leftd == true)
    {
        point2 = Point(x,y);
        cout << "point2 is " << point2 << endl;
        box =Rect(point1,point2);
        leftd = false;
        leftu = true;
        mark = true;
    }
    if(mark == true)
    {
        cout << " BOX is " << box << endl;
        roi = src(box);
        imshow("END",roi);
    }
}
