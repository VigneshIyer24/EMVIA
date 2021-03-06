/**
 *@file: video_diff.cpp
 *   
 *@description: cpp code that performs background subtaction using
 *  			frame differencing in the G-band
 *
 *@Author: Code written by Vignesh Iyer based on code written by Prof. Sam Siewert
 *
 *@Date: 07/01/2019
 */


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

char difftext[20];
char fileppm[50];
int frames=0;
int main( int argc, char** argv )
{
    Mat mat_frame, mat_gray, mat_diff, mat_gray_prev;
    VideoCapture vcap("DK_Laser.mpeg");
    unsigned int diffsum, maxdiff;
    double percent_diff;


    /*open the video stream and make sure it's opened*/

    /* "0" is the default video device which is normally the built-in webcam*/
    if(!vcap.isOpened()) 
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }
    else
    {
	   std::cout << "Opened video stream" << std::endl;
    }

    while(!vcap.read(mat_frame)) {
	std::cout << "No frame" << std::endl;
	cv::waitKey(33);
    }
	
    cv::cvtColor(mat_frame, mat_gray, CV_BGR2GRAY);
    /*
    *Code for creating a clone. mat_diff stores the clone.
    *mat_gray_prev stores the grayscvale version of the mat_gray
    */
    mat_diff = mat_gray.clone();
    mat_gray_prev = mat_gray.clone();

    maxdiff = (mat_diff.cols)*(mat_diff.rows)*255;/*Calcualates the number pixels*/

    while(1)
    {
	if(!vcap.read(mat_frame)) {
		std::cout << "No frame" << std::endl;
		break;
		
	}
	
	cv::cvtColor(mat_frame, mat_gray, CV_BGR2GRAY);/*Converts from RGB to gray*/

	absdiff(mat_gray_prev, mat_gray, mat_diff);/*stores the difference between mat_gray_prev, mat_gray*/

	/* worst case sum is resolution * 255*/
	diffsum = (unsigned int)cv::sum(mat_diff)[0]; /*single channel sum*/

	percent_diff = ((double)diffsum / (double)maxdiff)*100.0;

        printf("percent diff=%lf\n", percent_diff);
        sprintf(difftext, "%8d",  diffsum);

        // tested in ERAU Jetson lab
	if(percent_diff > 0.5) cv::putText(mat_diff, difftext, cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
	/*
	*Displays the current frame
	*/
	cv::imshow("Gray Example", mat_gray);
	cv::imshow("Gray Previous", mat_gray_prev);
	cv::imshow("Gray Diff", mat_diff);

	/*--------------------------------------------
	 * Converting the frames to images and saving
	 * them in a .ppm format to be later converted
	 * to a video stream
	 * -------------------------------------------*/

	sprintf(fileppm,"images/image%04d.ppm",frames);
	imwrite(fileppm,mat_diff);
	frames++;
        char c = cvWaitKey(33); // take this out or reduce
        if( c == 'q' ) break;

	mat_gray_prev = mat_gray.clone();
    }

};
