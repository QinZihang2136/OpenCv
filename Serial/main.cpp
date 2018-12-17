#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<iostream>
#include <stdio.h> / *标准输入/输出定义* /
#include <string.h> / *字符串函数定义* /
#include <unistd.h> / * UNIX标准函数定义* /
#include <fcntl.h>   /* File control definitions */
#include <errno.h> / *错误号定义* /
#include <termios.h> / * POSIX终端控制定义* /
#include <termio.h>


using namespace cv;
using namespace std ;
using namespace cv::xfeatures2d;
Mat src,gray,dst;
int fd;
int open_port(void);
int SERIAL_SETUP(const char* filename = "/dev/ttyUSB0",int Baud_rate = B115200);
int SERIAL_R();
void SERIAL_W(int angle[10]);
int main(int argc, char *argv[])
{
    SERIAL_SETUP();
    while(1)
    {
        int DAT[2];
        DAT[0]=10;
        DAT[1] = 9;
        SERIAL_W(DAT);
        SERIAL_R();

    }

    waitKey(0);
}


int SERIAL_SETUP(const char* filename ,int Baud_rate)
{

    fd = open(filename,  O_RDWR | O_NOCTTY | O_SYNC);
    if (fd == -1)
    {
      perror("open_port: Unable to open /dev/ttyf1 - ");
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
        perror("port is open.\n");
    }
    struct termios options;
    cfsetispeed(&options, Baud_rate);
    cfsetospeed(&options, Baud_rate);
    options.c_cflag |= CREAD | CLOCAL;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag = 0;
    options.c_cc[VMIN]  = 10;
    options.c_cc[VTIME] = 5;

    options.c_iflag |= (INPCK | ISTRIP);
    tcsetattr(fd, TCSANOW, &options);
    return 1;

}
void SERIAL_W(int angle[10])
{

    write(fd, angle, sizeof (angle));
    return 0;
}
int SERIAL_R()
{
    int buffer_size;
        static bool get_head = false;
        unsigned char buffer[10],tmp;
        ioctl(fd, FIONREAD, &buffer_size);
        if(buffer_size > 20)
            cout << "buffer size = "<< buffer_size <<endl;
        if(buffer_size > 0)
        {

            while(buffer_size > 0)
            {
                read(fd, buffer, 1);
                printf("%x\n",buffer[0]);
                ioctl(fd, FIONREAD, &buffer_size);

            }
        }


    }





