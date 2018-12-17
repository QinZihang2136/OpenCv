#include <main.h>
using namespace std;
using namespace cv;
int car_color = 1; //0red 1blue
float x_a, y_a, x_a_p, y_a_p;
int offset_y = 100, offset_x =100, latency = 8;
int ksize = 5;
int main(int argc, char** argv)
{

    VideoCapture cap(1);
    //VideoCapture cap("/home/cz/Pictures/BlueArmorVideo.avi");
    //VideoCapture cap("/home/cz/Pictures/RedArmorVideo.avi");
    if (!cap.isOpened())
    {
        printf("could not find video file");
        return -1;
    }

    namedWindow("Control", WINDOW_AUTOSIZE);
    namedWindow("dilate",CV_WINDOW_NORMAL);
    namedWindow("Blue",CV_WINDOW_NORMAL);
    int iLowH = 160;int iHighH = 255;
    int iLowS = 90;int iHighS = 255;
    int iLowV = 3;int iHighV = 3;
    float dx,dy;
    //double exposures = cap.get(CV_CAP_PROP_AUTO_EXPOSURE);
    //double exposure = cvGetCaptureProperty(cap,CV_CAP_PROP_EXPOSURE);
    //cout<<exposures<<endl;

    //Create trackbars in "Control" window
    {
        cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
        cvCreateTrackbar("HighH", "Control", &iHighH, 255);
        cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
        cvCreateTrackbar("HighS", "Control", &iHighS, 255);
        cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
        cvCreateTrackbar("HighV", "Control", &iHighV, 256);
        createTrackbar("kernel_size", "Control", &ksize, 50);
        createTrackbar("offset_y", "Control", &offset_y, 200);
        createTrackbar("offset x" , "Control", &offset_x, 200 );
        createTrackbar("latency" , "Control", &latency, 100 );
    }


    Mat frame,gray;
    //cap.set(CV_CAP_PROP_EXPOSURE,1);
    vector<vector<Point>> contour;
    vector<Vec4i> hier;
    vector<LED_Stick> LED_Stick_v;
    while (cap.read(frame))
    {
        //kernel = getStructuringElimgement(MORPH_RECT,Size(iLowV,iHighV));
        Mat imgHSV;
        cvtColor(frame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        cvtColor(frame,gray,CV_BGR2GRAY);
        Mat mask;
        Mat BGRbluemask;
        if(car_color){
            //blue
            //inRange(imgHSV, Scalar(61, 0 ,218), Scalar(126, 182, 255), mask); //Threshold the image
            inRange(imgHSV,Scalar(60,0,240), Scalar(150, 255, 255),mask);

            //inRange(imgHSV,Scalar(107,0,39), Scalar(135, 255, 255),mask);
        }
        else{
            //red
            Mat1b mask1, mask2;
            inRange(imgHSV, Scalar(0,0,150), Scalar(50,255,255), mask1);
            inRange(imgHSV, Scalar(150,0,150), Scalar(180,255,255), mask2);
            mask = mask1| mask2;
        }
        if(ksize==0)
            ksize = 1;
        Mat kernel = getStructuringElement(2,Size(ksize,ksize));
        //inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), mask); //Threshold the image
        morphologyEx(mask,mask,MORPH_CLOSE,kernel);
        //dilate(mask,mask,kernel);
        imshow("dilate", mask);
        //        erode(mask,mask,kernel);
        //        imshow("track mask", mask);
        cvtColor(mask,BGRbluemask,CV_GRAY2BGR);
        findContours(mask, contour, hier, RETR_TREE, CHAIN_APPROX_SIMPLE );
        LED_Stick_v.clear();
        vector<vector<Point>> convexs(contour.size());
        for(size_t i = 0; i < contour.size(); i++)
        {
            convexHull(contour[i], convexs[i], false, true);
            //approxPolyDP(Mat(contour[i]), convexs[i],4,true);
        }

        //cout<<contour.size()<<endl;
#pragma omp for
        for( size_t i = 0; i< contour.size(); i++ )
        {
            //            drawContours(BGRbluemask,contour,i,Scalar(0,0,255));
            //            drawContours(BGRbluemask,convexs,i,Scalar(0,255,0));
            if(hier.at(i)[3] != -1) continue;
            double length = arcLength( Mat(contour[i]), true);

            //            cout<<convexs.size()<<endl;
            //            cout<< i <<":"<<length << endl;
            //LED Stick detect
            if (length > 1 && length <430)
            {
                RotatedRect RRect = minAreaRect( Mat(convexs[i]));
                //Rect NRect = boundingRect( Mat(contour[i]));
                //                cout<< "rwidth:"<<RRect.size.width <<"rheight"<<RRect.size.height<<" angle:"<< RRect.angle << endl;
                //rotated Rect angle transform
                if(RRect.size.height < RRect.size.width)    // convert angle to
                {

                    //rectangle(BGRbluemask,NRect,Scalar(0,0,255));
                    RRect.angle+= 90;
                    double tmp = RRect.size.height;
                    RRect.size.height = RRect.size.width;
                    RRect.size.width = tmp;
                }
                        if (abs(RRect.angle) <= 25)   // angle in range (-20,20) degree
                {
                        //draw LED stick
                        LED_Stick r(RRect);
                        LED_Stick_v.push_back(r);
                        Point2f rect_point[4];
                RRect.points(rect_point);
                for (int i = 0; i < 3 ; i++)
                {
                    line(frame, Point_<int>(rect_point[i]), Point_<int>(rect_point[(i+1)%4]), Scalar(255,0,255),2);
                }
            }
        }
    }

    for(int i = 0; i < LED_Stick_v.size(); i++)
    {
        for(int j = i + 1; j < LED_Stick_v.size(); j++)
        {
            armor arm_tmp(LED_Stick_v.at(i), LED_Stick_v.at(j),frame);
            if(abs(arm_tmp.error_angle) < 8)
            {
                if(arm_tmp.is_suitable_size())
                {
                    if(arm_tmp.get_average_intensity(gray)<100)
                        arm_tmp.max_match(LED_Stick_v, i, j);
                    arm_tmp.draw_rect(frame);
                    arm_tmp.draw_spot(frame);
                }
            }
        }
    }
    //final armor list
//    vector<armor> final_armor_list;
//    for(int i = 0; i < LED_Stick_v.size(); i++)
//    {
//        if(LED_Stick_v.at(i).matched)
//        {
//            LED_Stick_v.at(LED_Stick_v.at(i).match_index).matched = false;
//            armor arm_tmp(LED_Stick_v.at(i),LED_Stick_v.at(LED_Stick_v.at(i).match_index));
//            final_armor_list.push_back(arm_tmp);
//        }
//    }
    //for draw
//    float dist = 1e8;

//    armor target;
//    target.center.x = 640 + offset_x - 100;
//    target.center.y = 480 + offset_y - 100;
//    int8_t found = 0;
//    for (int i = 0; i < final_armor_list.size(); i++)
//    {
//        dx = pow(final_armor_list.at(i).center.x - 640, 2);
//        dy = pow(final_armor_list.at(i).center.y - 360, 2);
//        if(dx + dy < dist)
//            target = final_armor_list.at(i);
//        final_armor_list.at(i).draw_rect(frame);
//        found = 1;
//    }
//    dx = target.center.x - 640 + offset_x - 100;
//    dy = target.center.y - 480 + offset_x - 100;
//    x_a = atan(dx * tan(25/57.295) / 640) * 57.295;
//    y_a = atan(dy * tan(25 / 57.295) / 360) * 57.295;
//    target.draw_spot(frame);
    imshow("Blue",frame);
    // trigger exit

    if(waitKey(2)>0)
        break;
}

cap.release();
waitKey(0);
return 0;
}
