/**
 *@file: cap_stereo.cpp
 *   
 *@description: cpp code that finds disparty between left and right images
 * run: ./cap_stereo 0 1
 *
 *@Author: Code written by Vignesh Iyer based on code written by Prof. Sam Siewert
 *
 *@Date: 07/13/2019
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

// variables to store images
Mat dst, cimg, gray, img, edges;

int initThresh;
const int maxThresh = 1000;
double th1,th2;
char fileppm[50];
int frames=1;
// create a vector to store points of line
vector<Vec4i> lines;
/*-------------------------------------------------------------------
 *@Function: void TrackDetect
 *@Input: int, void pointer
 *@Function: This function takes input as a frame and uses the 
             Canny for edge Detection and then uses the Hough Lines
	     to detect the lines for lane and the values as well
	     threshold have been taken on basis of trail and error.
	     Further to be improved using Guassian Blur for 
	     smoothening the images and exact detection using a 
	     binary mask by conversionfrom gray to binary image.
	     Then applying Hough Lines.
*-------------------------------------------------------------------*/
void TrackDetect( int , void* )
{ 
  	cimg = img.clone();
  	dst = img.clone();

	th1 = initThresh;
  	th2 = th1 * 0.4;

  	Canny(img,edges,th1,th2);
  
  	// apply hough line transform
  	HoughLinesP(edges, lines, 1, CV_PI/180, 50, 50, 150);

  	/*-----------------------------------------------
	 * Draw lines on the detected points. Creating
	 * new Vec4i because values also need to be 
	 * stored
	 *----------------------------------------------*/
   	for( size_t i = 0; i < lines.size(); i++ )
    	{
        	Vec4i l = lines[i];
        	line( dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, LINE_AA);
    	}
   
 
}

int main(int argc, char** argv) 
{  
	/*--------------------------------------
	 * Captured Video is pre-recorded
	 * video given in the same folder.
	 * Further work for higher size videos
	 * is being done.
	 *-------------------------------------*/
	VideoCapture cap("1.mp4" );
  
  	while(1)
  	{
  		cap >>img;
        	dst = img.clone();

        	if(img.empty())
        	{
                	cout << "Error in reading video" << endl;
                	break;
        	}

		/*-----------------------
		 * Convert to gray-scale
		 *----------------------*/
  		cvtColor(img, gray, COLOR_BGR2GRAY);

  		

  		
  		namedWindow("Edges",1);
  		namedWindow("Result Image", 1);
 
  		initThresh = 500;

  		/*---------------------------------------------------------------
		 * Create trackbar to change threshold values which takes input 
		 * as the threshold, window name, threshold values
		 *---------------------------------------------------------------*/
 		createTrackbar("threshold", "Result Image", &initThresh, maxThresh, TrackDetect);
  		onTrackbarChange(initThresh, 0);
		
		/*--------------------------------------------
	 	* Converting the frames to images and saving
	 	* them in a .ppm format to be later converted
	 	* to a video stream
	 	* -------------------------------------------*/
		
		sprintf(fileppm,"images/image%04d.ppm",frames);
		imwrite(fileppm,dst);
		frames++;
		if(frames==3000)
			break;
		imshow("Result Image", dst);
        	imshow("Edges", edges);

    		int key;
    		key = waitKey( 1 );
    		if( (char)key == 27 )
			break;
	}
	
  	destroyAllWindows();
	return 0;	
}
