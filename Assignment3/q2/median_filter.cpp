/**
 *@file: median_filter.cpp
 *   
 *@description: uses median filtering to enhance edges in the G band.
 *
 *@Author: Code written by Vignesh Iyer based on code written by Prof. Sam Siewert
 *
 *@Date: 07/01/2019
 */


#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#define MAX_KERNEL_LENGTH 31

using namespace cv;
using namespace std;

Mat org_img;
Mat src;
Mat bgr_img[3];
Mat G_img;
Mat gry_img;
Mat mod_img;
char window_name[]= "Median Filter G band";

int main(int argc, char **argv)
{
	namedWindow(window_name,WINDOW_AUTOSIZE);

	const char* filename =argc >=2 ? argv[1] : "image45.ppm";
	org_img=imread(filename,CV_LOAD_IMAGE_COLOR);
	namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );
 	imshow( "Original Image", org_img );
	/*This API splits an image into the BGR components that comprise it.
	*Here bgr_img is an array of size 3 which stores the BGR values respectively
	*/
	split(org_img,bgr_img);

	G_img=bgr_img[1];

	imwrite("green_img.jpg",bgr_img[1]);	
	if(org_img.empty())
	{
		printf("Error opening image\n");
		return -1;
	}
	/*
	*Median filter works by taking a pixel and differentiating it with neighbouring pixels
	*according to the size of the window of pixels. 
	*/
	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    	{
        	medianBlur ( bgr_img[1], mod_img, i );
    	}	
	
	namedWindow("G image",CV_WINDOW_AUTOSIZE);
	imshow("G image",bgr_img[1]);
	imshow(window_name,mod_img);
	
	waitKey(0);
	return 0;
}
