/**
 *@file: G_grayscale.cpp
 *   
 *@description: this cpp code shows the grayscale video of the G-band.
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

char fileppm[50];
int frames=0;
int main( int argc, char** argv )
{
    Mat mat_frame; 
    Mat mat_bgr[3];
    VideoCapture vcap("Dark_room_LS.mpeg");
    unsigned int diffsum, maxdiff;
    double percent_diff;


    //open the video stream and make sure it's opened
    // "0" is the default video device which is normally the built-in webcam
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
    while(1)
    {
	    
	while(!vcap.read(mat_frame))
    	{
        std::cout << "No frame" << std::endl;
        break;

    	}
    /*This API splits an image into the BGR components that comprise it.
    *Here bgr_img is an array of size 3 which stores the BGR values respectively
    */
	    split(mat_frame,mat_bgr);
	    imshow("Original",mat_frame);
	    imshow("Green_Grayscale",mat_bgr[1]);

	     /*--------------------------------------------
         * Converting the frames to images and saving
         * them in a .ppm format to be later converted
         * to a video stream
         * -------------------------------------------*/

        sprintf(fileppm,"images_pgm/image%04d.pgm",frames);
        imwrite(fileppm,mat_bgr[1]);
        frames++;
        char c = cvWaitKey(33); // take this out or reduce
        if( c == 'q' ) break;
     }
};

