/**
 *@file: skeletal_algo.cpp
 *   
 *@description: cpp code that performs background subtaction and then uses neighbouring
 *		comparison to get a skeletal image output
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
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv/highgui.h>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
char difftext[20];
char fileppm_diff[50];
char fileppm_gray[50];

char fileppm[50];
int frames=1;
void skeletal_thinning(Mat image_bin);

int main( int argc, char** argv )
{
	Mat mat_frame, mat_gray, mat_diff, mat_gray_prev, COM_detect, med_frame;
	Mat gray, binary, mfblur;
	VideoCapture vcap(0);
	unsigned int diffsum, maxdiff;
	double percent_diff;
	if(!vcap.isOpened()) 
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}
	else
	{
	        std::cout << "Opened video stream" << std::endl;
	}

	while(!vcap.read(mat_frame)) 
	{
		std::cout << "No frame" << std::endl;
		cv::waitKey(33);
	}

	cv::cvtColor(mat_frame, mat_gray, CV_BGR2GRAY);

	mat_diff = mat_gray.clone();
	mat_gray_prev = mat_gray.clone();

	maxdiff = (mat_diff.cols)*(mat_diff.rows)*64;

	while(1)
	{
		if(!vcap.read(mat_frame)) {
			std::cout << "No frame" << std::endl;
			break;
			
		}
		/*---------------------------------------------
		 * Frame differencing is used for background
		 * elimination  adn stores in mat_gray so can
		 * be directly converted to binary format
		 *---------------------------------------------*/
		
		cv::cvtColor(mat_frame, mat_gray, CV_BGR2GRAY);
		absdiff(mat_gray_prev, mat_gray, mat_diff);

			/* worst case sum is resolution * 255*/
		diffsum = (unsigned int)cv::sum(mat_diff)[0]; /* single channel sum*/

		percent_diff = ((double)diffsum / (double)maxdiff)*100.0;

		
		sprintf(difftext, "%8d",  diffsum);

		// tested in ERAU Jetson lab
		if(percent_diff > 0.5) cv::putText(mat_diff, difftext, cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
		threshold(mat_gray, binary, 70, 255, CV_THRESH_BINARY);
	 	binary = 255 - binary;
		
		
		medianBlur(binary, mfblur, 1);

		skeletal_thinning(mfblur);
		
		/*--------------------------------------------
	 	* Converting the frames to images and saving
	 	* them in a .ppm format to be later converted
	 	* to a video stream
	 	* -------------------------------------------*/
		sprintf(fileppm,"images/image%04d.ppm",frames);
		imwrite(fileppm,mfblur);
		
		frames++;
		
		if(frames==300)
			break;
		char c = cvWaitKey(33);
	       	if( c == 27 ) break;
		mat_gray_prev = mat_gray.clone();
	}



};
/*--------------------------------------------
 * This function takes input as a Mat frame
 * and compares the neighbouring pixels in
 * a sequential way for thinning of the images
 * and the output is displayed in the end
 *--------------------------------------------*/

void skeletal_thinning(Mat image_bin)
{
	static bool intensity[9];
	for(int j=1; j<image_bin.rows;j++)
	{
		for(int i=1; i<image_bin.cols;i++)
		{

			intensity[0]=image_bin.at<uchar>(j,i);
			intensity[1]=image_bin.at<uchar>(j,i+1);
			intensity[2]=image_bin.at<uchar>(j+1,i+1);
			intensity[3]=image_bin.at<uchar>(j+1,i);
			intensity[4]=image_bin.at<uchar>(j+1,i-1);
			intensity[5]=image_bin.at<uchar>(j,i-1);
			intensity[6]=image_bin.at<uchar>(j-1,i-1);
			intensity[7]=image_bin.at<uchar>(j-1,i);
			intensity[8]=image_bin.at<uchar>(j-1,i+1);

			int sigma=intensity[1]+intensity[2]+intensity[3]+intensity[4]+intensity[5]+intensity[6]+intensity[7]+intensity[8];
			int phi=(int)(intensity[1]!=intensity[3])+(int)(intensity[3]!=intensity[5])+(int)(intensity[5]!=intensity[7])+(int)(intensity[7]!=intensity[1])
					+2*((int)((intensity[2]>intensity[1])&&(intensity[2]>intensity[3]))+(int)((intensity[4]>intensity[3])&&(intensity[4]>intensity[5]))
					+(int)((intensity[6]>intensity[5])&&(intensity[6]>intensity[7]))+(int)((intensity[8]>intensity[7])&&(intensity[8]>intensity[1])));

			if((intensity[0])&&(phi==2)&&(sigma!=1))
			{
				image_bin.at<uchar>(j,i)=0;
			}
		}
	}
	imshow("Skeletal",image_bin);
}


