#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
Mat dst,src;
const int max_value_H = 360/2;
const int max_value = 255;
int num =1;
int num2 =1;
const String window_capture_name = "Video Capture";
const String window_named = "DST";
const String window_detection_name = "Object Detection";
int low_H = 68, low_S = 0, low_V = 187;
int high_H = 105, high_S = 255, high_V = max_value;
Mat frame, frame_HSV, frame_threshold;
void callback(int, void *);
static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);void callback(int, void *);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}
void callback(int, void *);

int main(int argc, char* argv[])
{
    VideoCapture cap(0);

    namedWindow(window_capture_name);
    namedWindow(window_detection_name);
    namedWindow(window_named);
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
    createTrackbar("Size",window_named,&num,11,callback);
    createTrackbar("Blur",window_named,&num2,11);

    while (true) {
        cap >> frame;
        imshow("input",frame);
       // GaussianBlur(frame,frame,Size(num2*2+1,num2*2+1),0,0);
        blur(frame,frame,Size(num2*2+1,num2*2+1));
        if(frame.empty())
        {
            break;
        }
        // Convert from BGR to HSV colorspace
        cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
        // Detect the object based on HSV Range Values
        inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
        // Show the frames
        imshow(window_capture_name, frame);
        imshow(window_detection_name, frame_threshold);
callback(0,0);
        imshow("DST",dst);
        char key = (char) waitKey(30);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
    return 0;
}
void callback(int, void *)
{
    Mat element = getStructuringElement(MORPH_RECT,Size(num*2+1,num*2+1));
    morphologyEx(frame_threshold,dst,MORPH_OPEN,element);
}
