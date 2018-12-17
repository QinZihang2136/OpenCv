#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;
Mat roiImg;
Rect roi;
static void help()
{
    // print a welcome message, and the OpenCV version
    cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
            "Using OpenCV version " << CV_VERSION << endl;
    cout << "\nIt uses camera by default, but you can provide a path to video as an argument.\n";
    cout << "\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tr - auto-initialize tracking\n"
            "\tc - delete all the points\n"
            "\tn - switch the \"night\" mode on/off\n"
            "To add/remove a feature point click it\n" << endl;
}
char ad[128] = {0};
int id=0;

Point2f point;
bool addRemovePt = false;

 static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        point = Point2f((float)x, (float)y);
        addRemovePt = true;
    }
}

 void Pic2Gray(Mat camerFrame,Mat &gray)
 {
     //普通台式机3通道BGR,移动设备为4通道
     if (camerFrame.channels() == 3)
     {
         cvtColor(camerFrame, gray, CV_BGR2GRAY);
     }
     else if (camerFrame.channels() == 4)
     {
         cvtColor(camerFrame, gray, CV_BGRA2GRAY);
     }
     else
         gray = camerFrame;
 }

int main( int argc, char** argv )
{
    //加载Haar或LBP对象或人脸检测器
    CascadeClassifier faceDetector;
    std::string faceCascadeFilename = "/home/qinzihang/opencv-3.4.1/data/haarcascades/haarcascade_frontalface_alt.xml";

    //友好错误信息提示
    try{
        faceDetector.load(faceCascadeFilename);
    }
    catch (cv::Exception e){}
    if (faceDetector.empty())
    {
        std::cerr << "脸部检测器不能加载 (";
        std::cerr << faceCascadeFilename << ")!" << std::endl;
        exit(1);
    }




    help();

    VideoCapture cap;
    TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    const int MAX_COUNT = 500;
    bool needToInit = false;
    bool nightMode = false;

    if( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
        cap.open(argc == 2 ? argv[1][0] - '0' : 0);
    else if( argc == 2 )
        cap.open(argv[1]);
    cap.open(0);
    if( !cap.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return 0;
    }

    namedWindow( "LK Demo", 1 );
    setMouseCallback( "LK Demo", onMouse, 0 );

    Mat gray, prevGray, image;
    vector<Point2f> points[2];

    for(;;)
    {
        Mat frame;
        cap >> frame;
        Mat camerFrame = frame;
        if (frame.empty())
        {
            std::cerr << "无法获取摄像头图像" << std::endl;
            getchar();
            exit(1);
        }
        Mat displayedFrame(frame.size(),CV_8UC3);
        Mat gray;
        Pic2Gray(camerFrame, gray);
        //直方图均匀化(改善图像的对比度和亮度)
        Mat equalizedImg;
        equalizeHist(gray, equalizedImg);

        //人脸检测用Cascade Classifier::detectMultiScale来进行人脸检测

        int flags = CASCADE_FIND_BIGGEST_OBJECT|CASCADE_DO_ROUGH_SEARCH;	//只检测脸最大的人
        //int flags = CASCADE_SCALE_IMAGE;	//检测多个人
        Size minFeatureSize(30, 30);
        float searchScaleFactor = 1.1f;
        int minNeighbors = 4;
        std::vector<Rect> faces;
        faceDetector.detectMultiScale(equalizedImg, faces, searchScaleFactor, minNeighbors, flags, minFeatureSize);
        Mat useful = gray;
        cv::Mat face;
        cv::Point text_lb;
        for (size_t i = 0; i < faces.size(); i++)
        {
            if (faces[i].height > 0 && faces[i].width > 0)
            {
                face = gray(faces[i]);
                text_lb = cv::Point(faces[i].x, faces[i].y);
                 roi = Rect(faces[i]);
                            if (roi.width && roi.height)
                            {
                                 roiImg = frame(roi);
                                imshow("roi", roiImg);

                                vector<Mat>bgr_planes;
                                split(roiImg,bgr_planes);
                                imshow("single channel demo",bgr_planes[1]);

                                int histSize = 256;
                                float range[]={0,256};
                                const float *histRanges = {range};
                                Mat b_hist,g_hist,r_hist;
                                calcHist(&bgr_planes[0],1,0,Mat(),b_hist,1,&histSize,&histRanges,true,false);
                                calcHist(&bgr_planes[1],1,0,Mat(),g_hist,1,&histSize,&histRanges,true,false);
                                calcHist(&bgr_planes[2],1,0,Mat(),r_hist,1,&histSize,&histRanges,true,false);

                                int hist_h=400;
                                int hist_w=512;
                                int bin_w=hist_w/histSize;
                                Mat histImage(hist_w,hist_h,CV_8UC3,Scalar(0,0,0));
                                normalize(b_hist,b_hist,0,hist_h,NORM_MINMAX,-1,Mat());
                                normalize(g_hist,g_hist,0,hist_h,NORM_MINMAX,-1,Mat());
                                normalize(r_hist,r_hist,0,hist_h,NORM_MINMAX,-1,Mat());
                                //render histogram chart
                                for(int i=0;i<histSize;i++)
                                {
                                    line(histImage,Point((i-1)*bin_w,hist_h-cvRound(b_hist.at<float>(i-1))),
                                                    Point(i*bin_w,hist_h-cvRound(b_hist.at<float>(i))),Scalar(255,0,0),1,LINE_AA);
                                    line(histImage,Point((i-1)*bin_w,hist_h-cvRound(g_hist.at<float>(i-1))),
                                                    Point(i*bin_w,hist_h-cvRound(g_hist.at<float>(i))),Scalar(0,255,0),1,LINE_AA);
                                    line(histImage,Point((i-1)*bin_w,hist_h-cvRound(r_hist.at<float>(i-1))),
                                                    Point(i*bin_w,hist_h-cvRound(r_hist.at<float>(i))),Scalar(0,0,255),1,LINE_AA);
                                }
                                imshow("histImgae",histImage);

                                //sprintf(ad,"1%d.jpg",id);
                                //imshow(ad,roiImg);
                               // id++;
                                //imwrite(ad,roiImg);


                                //imwrite("D://video picture//1.jpg", roiImg);
                            }
                cv::rectangle(camerFrame, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);
                /*cv::rectangle(equalizedImg, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);
                cv::rectangle(gray, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);

                */
            }
        }

     imshow("原图", camerFrame);


if(!roiImg.empty())
{  frame.copyTo(image);

        Mat imangee,mask;
        mask = Mat::zeros(frame.size(),CV_8UC1);
        mask(roi).setTo(255);
        imshow("mask",mask);
        cvtColor(image, gray, COLOR_BGR2GRAY);

        if( nightMode )
            image = Scalar::all(0);

        if( needToInit )
        {
            // automatic initialization
            goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10,mask, 3, 0,false, 0.04);
            cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
            addRemovePt = false;
        }
        else if( !points[0].empty() )
        {
            vector<uchar> status;
            vector<float> err;
            if(prevGray.empty())
                gray.copyTo(prevGray);
            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
                                 3, termcrit, 0, 0.001);
            size_t i, k;
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( addRemovePt )
                {
                    if( norm(point - points[1][i]) <= 5 )
                    {
                        addRemovePt = false;
                        continue;
                    }
                }

                if( !status[i] )
                    continue;

                points[1][k++] = points[1][i];
                circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
            }
            points[1].resize(k);
        }

        if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
        {
            vector<Point2f> tmp;
            tmp.push_back(point);
            cornerSubPix( gray, tmp, winSize, cvSize(-1,-1), termcrit);
            points[1].push_back(tmp[0]);
            addRemovePt = false;
        }

        needToInit = false;
        imshow("LK Demo", image);

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch( c )
        {
        case 'r':
            needToInit = true;
            break;
        case 'c':
            points[0].clear();
            points[1].clear();
            break;
        case 'n':
            nightMode = !nightMode;
            break;
        }

        std::swap(points[1], points[0]);
        cv::swap(prevGray, gray);
}
}


    return 0;
}
