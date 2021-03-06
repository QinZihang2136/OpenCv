#include<opencv.hpp>
using namespace cv;
#include<iostream>
#include<vector>
using namespace std;
/*----定义鼠标事件--画矩形区域：作用当两个车灯----*/
//第一步：全局变量
bool drawing_box = false;
bool gotBox =  false;
Rect box;
Point downPoint;


void mouseRectHandler(int event, int x, int y, int flags, void *param)
{
    switch (event)
    {
    case CV_EVENT_MOUSEMOVE:
        if (drawing_box)
        {
            //鼠标的移动到downPoint的右下角
            if( x >=downPoint.x && y >= downPoint.y)
            {
                box.x = downPoint.x;
                box.y = downPoint.y;
                box.width = x - downPoint.x;
                box.height =  y -  downPoint.y;
            }
            //鼠标的移动到downPoint的右上角
            if( x >= downPoint.x && y <= downPoint.y)
            {
                box.x =  downPoint.x;
                box.y = y;
                box.width = x - downPoint.x;
                box.height = downPoint.y - y;
            }
            //鼠标的移动到downPoint的左上角
            if( x <= downPoint.x && y <= downPoint.y)
            {
                box.x = x;
                box.y = y;
                box.width = downPoint.x - x;
                box.height = downPoint.y - y;
            }
            //鼠标的移动到downPoint的左下角
            if( x <= downPoint.x && y >= downPoint.y)
            {
                box.x = x;
                box.y = downPoint.y;
                box.width = downPoint.x -x;
                box.height = y - downPoint.y;
            }
        }
        break;

    case CV_EVENT_LBUTTONDOWN:
        //按下鼠标，代表可以可以开始画矩形
        drawing_box = true;
        //记录起点
        downPoint = Point(x,y);
        break;

    case CV_EVENT_LBUTTONUP:

        //松开鼠标，代表结束画矩形
        drawing_box = false;
        gotBox = true;
        break;
    default:
        break;
    }
}

int main(int argc,char*argv[])
 {
     //读取视频bool drawing_box;
    bool lftd = false;
    bool lftu = true;
    Rect box;
     VideoCapture video(0);

     //判断视频是否打开
     if( !video.isOpened())
         return 0;

     //视频帧
     Mat frame;


     //register
     namedWindow("video",1);
     setMouseCallback("video",mouseRectHandler,NULL);

     //画感兴趣区域
    for(;;)
     {
         //读取视频的第一帧
        video>>frame;
        if(!frame.data)
            break;
         rectangle(frame,box,Scalar(255,255,0),2);//画出感兴趣区域
         imshow("video",frame);
         if(gotBox)
             break;
         if(waitKey(50) == 'q')//---------很重要
             break;
     }
     //remove callback
     //setMouseCallback("video",NULL,NULL);

     //视频继续
     for(;;)
     {
         //读取视频
         video>>frame;
          //判断是否有当前帧
          if(!frame.data)
              break;
          //画出感兴趣区域
          rectangle(frame,box,Scalar(0,255,0),2);
          imshow("video",frame);
          if(waitKey(33) == 'q')
              break;
     }
     return 0;
 }
