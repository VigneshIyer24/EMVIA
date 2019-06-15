#include <sys/utsname.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <semaphore.h>
#include <pthread.h>
#include <opencv/highgui.h>
#include<opencv2/imgproc.hpp>
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/opencv.hpp"
//#include <opencv2/core/core.hpp>

using namespace cv;
Mat src, dst;
int main(int argc, char** argv)
{
	src = imread( argv[1] );
	imwrite("Trees_convert.ppm",src);
}
