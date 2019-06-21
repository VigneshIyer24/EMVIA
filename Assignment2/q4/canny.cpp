/*------------------------
* Code edited and referred
* from OpenCV webiste
 -------------------------*/



#include "canny.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

using namespace cv;

void Canny_Edge(Mat img, Mat imgCanny)
{
 	int lowTh = 45;
 	int highTh = 90;
        // input image
 	Mat imgGrayscale;       // grayscale image
	Mat imgBlurred;         // blured image
 	//Mat imgCanny;           // Canny edge image

	
	cvtColor(img, imgGrayscale, CV_BGR2GRAY);                   // convert to grayscale

  		GaussianBlur(imgGrayscale,imgBlurred,cv::Size(5, 5),1.8);           // Blur Effect

  		Canny(imgBlurred,imgCanny,lowTh,highTh);       // Canny Edge Image

  		//declare windows
 		namedWindow("imgOriginal", CV_WINDOW_NORMAL);
  		namedWindow("imgCanny", CV_WINDOW_NORMAL);
  		//declare trackbar
  		createTrackbar("LowTh", "imgCanny", &lowTh, 100);
  		createTrackbar("HighTh", "imgCanny", &highTh, 100);
		imshow("imgOriginal", img);
                imshow("imgCanny", imgCanny);


}
