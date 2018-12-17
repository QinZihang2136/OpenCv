#ifndef ARMOR_H
#define ARMOR_H
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class LED_Stick{
public:
    LED_Stick():matched(false){}

    LED_Stick(const RotatedRect& R){
        rect.angle = R.angle;
        rect.center = R.center;
        rect.size = R.size;
        matched = false;
    }

    RotatedRect rect;
    bool matched;
    int match_index;
    float match_factor;
};

class armor{
public:
    armor();
    armor(const LED_Stick& L1, const LED_Stick& L2,Mat& img);

    void draw_rect(Mat &img) const;
    void draw_spot(Mat &img) const;
    int get_average_intensity(const Mat& img);
    void max_match(vector<LED_Stick>& LED, const int & i, const int& j);
    bool is_suitable_size(void) const;

    LED_Stick Led_stick[2];
    int error_angle;
    Point2i center;
    Rect2i rect;
    int average_intensity;
};

void filiter_smail_rect(vector<LED_Stick>& LED);

#endif // ARMOR_H
