#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

#define VRES_ROWS (240)
#define HRES_COLS (320)

char buffername[50];
unsigned char imagebuffer[1440*2560*3]; // 1440 rows, 2560 cols/row, 3 channel

int main(int argc, char **argv)
{
    int hres = HRES_COLS;
    int vres = VRES_ROWS;
    Size size(240,320);
    Mat basicimage(vres, hres, CV_8UC3, imagebuffer);
    namedWindow("Image", CV_WINDOW_AUTOSIZE);

    basicimage = imread("Bears.ppm", CV_LOAD_IMAGE_COLOR);
    cv::resize(basicimage,basicimage,size,0,0,INTER_LINEAR);
    cvtColor(basicimage,basicimage,CV_BGR2GRAY);
    sprintf(buffername,"Bears_320x240.pgm");
    imwrite(buffername,basicimage);
    imshow("Image",basicimage);
    waitKey();
}