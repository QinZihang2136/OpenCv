#include <graphics.h>
#include <conio.h>
int main()
{
    int a, b;
    initgraph(600, 600);

    for (;;)
    {
        for (a = 300, b = 300; a > 200; a--, b++)
        {
            cleardevice();
            circle(300, 300, 100);
            ellipse(200, 270, 400, 330);
            ellipse(a, 200, b, 400);
            Sleep(50);

        }
        for (a = 200, b = 400; a < 300; a++, b--)
        {
            cleardevice();
            circle(300, 300, 100);
            ellipse(200, 270, 400, 330);
            ellipse(a, 200, b, 400);
            Sleep(50);
        }
    }
    _getch();
    closegraph();
}
