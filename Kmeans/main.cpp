#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>

using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;
Mat frame;
Mat hsv_video;
int H = 150;
int S = 120;
int V = 10;
void OnMouseAction(int event,int x,int y,int flags,void *ustc);
void callback(int , void*);
int main(int argc, char *argv[])
{


    src =imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("src",src);
    Mat hsv_src;
    cvtColor(src,hsv_src,CV_BGR2HSV_FULL);
    imshow("HSV",hsv_src);
    VideoCapture video;

    video.open(0);

    while (1)
    {
        video >> frame;

        hsv_video;
        cvtColor(frame,hsv_video,CV_BGR2HSV_FULL);
        namedWindow("HSV_VIDEO");

        vector<Mat> hsv_channels;
        split(hsv_video, hsv_channels);
        Mat H_img = hsv_channels.at(0);
        Mat S_img = hsv_channels.at(1);
        Mat V_img = hsv_channels.at(2);
       // createTrackbar("H","INPUT Video",&H,255,callback);
       // createTrackbar("S","INPUT Video",&S,255,callback);
       // createTrackbar("V","INPUT Video",&V,255,callback);
        callback(0,0);

  //     imshow("H_img",H_img);
   //    imshow("S_img",S_img);
    //   imshow("V_img",V_img);
        setMouseCallback("HSV_VIDEO",OnMouseAction);
        imshow("INPUT Video",dst);
        imshow("HSV_VIDEO",hsv_video);

        waitKey(1);
    }

    waitKey(0);
}

void OnMouseAction(int event,int x,int y,int flags,void *ustc)
{
    if(event == CV_EVENT_MOUSEMOVE )
    {


        cout << hsv_video.channels() <<  "  ";
        cout << "X = " << x << "  ";
        cout << "Y = " << y << "  ";
        printf("H = %d",hsv_video.at<cv::Vec3b>(x, y)[0]);
        cout  << "  ";
        printf("S = %d",hsv_video.at<cv::Vec3b>(x, y)[1]);
        cout  << "  ";
        printf("V = %d",hsv_video.at<cv::Vec3b>(x, y)[2]);

        cout << "  " <<endl;
    }
}
void callback(int , void*)
{
    #pragma omp parallel for
    for(int i = 0; i < hsv_video.rows; i++)
    {
        #pragma omp parallel for
        for(int j = 0; j < hsv_video.cols; j++ )
        {
            if( (hsv_video.at<cv::Vec3b>(i, j)[0] < 124 && hsv_video.at<cv::Vec3b>(i, j)[0] >100 ) )
            {
                if((hsv_video.at<cv::Vec3b>(i, j)[1] < 255 && hsv_video.at<cv::Vec3b>(i, j)[1] > 43))
                {
                    if((hsv_video.at<cv::Vec3b>(i, j)[2] < 255 + 40 && hsv_video.at<cv::Vec3b>(i, j)[2] > 46 ))
                    {
                        frame.at<cv::Vec3b>(i, j)[0] = 0;
                        frame.at<cv::Vec3b>(i, j)[1] = 0;
                        frame.at<cv::Vec3b>(i, j)[2] = 0;
                    }
                }
            }
        }
    }
    dilate(frame,dst,(29,29));
}

