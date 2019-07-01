/**
 *@file: threshold_video.cpp
 *   
 *@description: background substraction using frame differencing and detection of the laser point using 
 *              threshold function and identification of the laser point centroid.
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

#define MAX_KERNEL_LENGTH 31

using namespace cv;
using namespace std;

char fileppm[50];
int frames=0;
int main( int argc, char** argv )
{
    Mat mat_frame;
    Mat COM_detect;
    Mat mod_img;  
    Mat mat_bgr[3];
    int X,Y;
    VideoCapture vcap("Dark_room_LS.mpeg");
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

    while(!vcap.read(mat_frame)) 
    {
        std::cout << "No frame" << std::endl;
	break;
    
    }
    while(vcap.read(mat_frame))
    {
	    
split(mat_frame,mat_bgr);
	    

	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    	{
        	medianBlur ( mat_bgr[1], mod_img, i );
    	}	
	threshold( mod_img, COM_detect, 25,255,THRESH_BINARY ); 
	/*find moments of the image*/
	Moments m = moments(COM_detect,true);
	Point p(m.m10/m.m00, m.m01/m.m00);
 	X=m.m10/m.m00;
	Y=m.m01/m.m00;
	/*coordinates of the centroid*/
	cout<< "The Centroid = ";
	cout << X ;
	cout << ",";
	cout << Y << endl;	
	/*Draw cross-hairs. uchar is used for a single channel image.*/
 	for(int i=1;i<4;i++)
	{
		mod_img.at<uchar>(Y,X-i)=255;
		mod_img.at<uchar>(Y,X+i)=255;
	}

	for(int j=1;j<4;j++)
	{
		mod_img.at<uchar>(Y-j,X)=255;
		mod_img.at<uchar>(Y+j,X)=255;   
	}

	/*show the image with a point mark at the centroid*/

	
	/*--------------------------------------------
    * Converting the frames to images and saving
    * them in a .ppm format to be later converted
    * to a video stream
    * -------------------------------------------
    */

	sprintf(fileppm,"images_pgm_q5/image%04d.pgm",frames);
        imwrite(fileppm,mod_img);

	imshow("COM image",mod_img);
        frames++;
        char c = cvWaitKey(33); // take this out or reduce
        if( c == 'q' ) break;
     }
};

