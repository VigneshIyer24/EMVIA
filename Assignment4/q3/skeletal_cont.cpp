    
/**
 *@file: skeletal_cont.cpp
 *   
 *@description: cpp code that performs skeletal transform
 *
 *@Author: Code written by Vignesh Iyer based on code written by Prof. Sam Siewert
 *
 *@Date: 07/13/2019
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

#include <opencv/highgui.h>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

char fileppm[50];
int frames=1;

int main( int argc, char** argv )
{
	cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    	CvCapture* capture = cvCreateCameraCapture(0);
    	IplImage* frame;
    	Mat gray, binary, mfblur;

    	while(1)
    	{
        	frame=cvQueryFrame(capture);
     	
        	if(!frame) break;
		Mat src(cvarrToMat(frame));
	
		if(src.empty())
 		{
     			cout << "can not open video" << endl;
     			return -1;
 		}


 
 	
		cvtColor(src, gray, CV_BGR2GRAY);
	
		threshold(gray, binary, 70, 255, CV_THRESH_BINARY);
 		binary = 255 - binary;

	
		medianBlur(binary, mfblur, 1);
		Mat skel(mfblur.size(), CV_8UC1, Scalar(0));
 		Mat temp;
 		Mat eroded;
 		Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
 		bool done;
 		int iterations=0;
		/*----------------------------------------------
		 *This loop performs the skeletal transform for
		 * a binary image 
		 * ----------------------------------------------*/
		 
 		do
 		{
   			erode(mfblur, eroded, element);
   			dilate(eroded, temp, element);
   			subtract(mfblur, temp, temp);
   			bitwise_or(skel, temp, skel);
   			eroded.copyTo(mfblur);

   			done = (countNonZero(mfblur) == 0);
   			iterations++;

 		} while (!done && (iterations < 100));

 		cout << "iterations=" << iterations << endl;

 		imshow("skeleton", mat_diff);
		
		/*---------------------------------------------
		 * Frame differencing is used for background
		 * elimination  adn stores in mat_gray so can
		 * be directly converted to binary format
		 *---------------------------------------------*/
		
		sprintf(fileppm,"images/image%04d.ppm",frames);
		imwrite(fileppm,skel);
		frames++;
		if(frames==3000)
			break;
		char c = cvWaitKey(33);
        	if( c == 27 ) break;
	}

    	cvReleaseCapture(&capture);
    	cvDestroyWindow("Capture Example");
    
};
