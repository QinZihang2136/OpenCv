#include "core/core.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include<iostream>

using namespace cv;
using namespace std;

void OnMouseAction(int event,int x,int y,int flags,void *ustc);  //鼠标回调事件函数
int static times; //记录调用次数

int rowsBegin,rowsEnd;
int colsBegin,colsEnd;
Mat image;

int main(int argc,char*argv[])
{
    image=imread("/home/qinzihang/OpenCV_Picture/1.jpg");
    imshow("image",image);
    setMouseCallback("image",OnMouseAction);
    waitKey();
}

//*******************************************************************//
//鼠标回调函数
void OnMouseAction(int event,int x,int y,int flags,void *ustc)
{
    if(event==CV_EVENT_MOUSEMOVE)
    {
        cout<<"触发鼠标移动事件"<<endl;
    }
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        rowsBegin=y;
        colsBegin=x;
        cout<<"触发左键按下事件"<<endl;
    }
    if(event==CV_EVENT_LBUTTONUP)
    {
        rowsEnd=y;
        colsEnd=x;
        line(image,Point(colsBegin,rowsBegin),Point(colsEnd,rowsEnd),Scalar(255,0,0),2,8,0);
        rectangle(image,Rect(colsBegin,rowsBegin,colsEnd-colsBegin,rowsEnd-rowsBegin),Scalar(0,255,0),2,8,0);
        circle(image,Point((colsEnd+colsBegin)/2,(rowsEnd+rowsBegin)/2),rowsEnd-rowsBegin,Scalar(0,0,255),2,8,0);
        imshow("image",image);
        cout<<"触发左键抬起事件"<<endl;
    }
    if(event==CV_EVENT_RBUTTONDOWN)
    {
        cout<<"触发右键按下事件"<<endl;
    }
    if(event==CV_EVENT_RBUTTONUP)
    {
        cout<<"触发右键抬起事件"<<endl;
    }
    if(event==CV_EVENT_LBUTTONDBLCLK)
    {
        cout<<"触发左键双击事件"<<endl;
    }
    if(event==CV_EVENT_RBUTTONDBLCLK)
    {
        cout<<"触发右键双击事件"<<endl;
    }
}
