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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

// If you have enough USB 2.0 bandwidth, then run at higher resolution
//#define HRES_COLS (640)
//#define VRES_ROWS (480)

// Should always work for uncompressed USB 2.0 dual cameras
#define HRES_COLS (320)
#define VRES_ROWS (240)

#define ESC_KEY (27)

char snapshotname[80]="snapshot_xxx.jpg";

int main( int argc, char** argv )
{
    double prev_frame_time, prev_frame_time_l, prev_frame_time_r;
    double curr_frame_time, curr_frame_time_l, curr_frame_time_r;
    struct timespec frame_time, frame_time_l, frame_time_r;
    CvCapture *capture;
    CvCapture *capture_l;
    CvCapture *capture_r;
    IplImage *frame, *frame_l, *frame_r;
    int dev=0, devl=0, devr=1;

    Mat disp;
    Mat fr_l,fr_r;
    Mat f_l,f_r;
    Ptr<StereoBM> myStereoVar= StereoBM::create();


    if(argc == 1)
    {
        printf("Will open DEFAULT video device video0\n");
        capture = cvCreateCameraCapture(0);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
    }

    if(argc == 2)
    {
        printf("argv[1]=%s\n", argv[1]);
        sscanf(argv[1], "%d", &dev);
        printf("Will open SINGLE video device %d\n", dev);
        capture = cvCreateCameraCapture(dev);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
    }

    if(argc >= 3)
    {

        printf("argv[1]=%s, argv[2]=%s\n", argv[1], argv[2]);
        sscanf(argv[1], "%d", &devl);
        sscanf(argv[2], "%d", &devr);
        printf("Will open DUAL video devices %d and %d\n", devl, devr);
        capture_l = cvCreateCameraCapture(devl);
        capture_r = cvCreateCameraCapture(devr);
        cvSetCaptureProperty(capture_l, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
        cvSetCaptureProperty(capture_l, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);
        cvSetCaptureProperty(capture_r, CV_CAP_PROP_FRAME_WIDTH, HRES_COLS);
        cvSetCaptureProperty(capture_r, CV_CAP_PROP_FRAME_HEIGHT, VRES_ROWS);

        /*---------------------------------
	 * set parameters for disparity using
	 * min disparity, sobel filter etc.
	 *----------------------------------*/
	    
        myStereoVar->setSmallerBlockSize(3);
        myStereoVar->setMinDisparity(-16);
        myStereoVar->setNumDisparities(16);
        myStereoVar->setUniquenessRatio(3);
        myStereoVar->setPreFilterType(1);


        cvNamedWindow("Capture LEFT", CV_WINDOW_AUTOSIZE);
        cvNamedWindow("Capture RIGHT", CV_WINDOW_AUTOSIZE);
        namedWindow("Capture DISPARITY", CV_WINDOW_AUTOSIZE);

        while(1)
        {
            frame_l=cvQueryFrame(capture_l);
            frame_r=cvQueryFrame(capture_r);
	    Mat fr_l(cvarrToMat(frame_l));
	    Mat fr_r(cvarrToMat(frame_r)); 
	    cvtColor(fr_l,f_l,CV_BGR2GRAY);
	    cvtColor(fr_r,f_r,CV_BGR2GRAY);
	    /*----------------------------------
	     * The compute variable for StereoBM
	     *  accepts images of type CV_8UC1
	     * thus converted to grayscale for
	     * computation
	     *-----------------------------------*/
            myStereoVar->compute(f_l, f_r, disp);
 
            if(!frame_l) break;
            else
            {
                clock_gettime(CLOCK_REALTIME, &frame_time_l);
                curr_frame_time_l=((double)frame_time_l.tv_sec * 1000.0) + 
                                  ((double)((double)frame_time_l.tv_nsec / 1000000.0));
            }
            if(!frame_r) break;
            else
            {
                clock_gettime(CLOCK_REALTIME, &frame_time_r);
                curr_frame_time_r=((double)frame_time_r.tv_sec * 1000.0) + 
                                  ((double)((double)frame_time_r.tv_nsec / 1000000.0));
            }
	    /*--------------------------
	     *  Displaying the grayscale
	     *  to understand the depth 
	     * difference
	     *--------------------------*/
		
            imshow("Capture LEFT", f_l);
            imshow("Capture RIGHT", f_r);
            imshow("Capture DISPARITY", disp);
            printf("LEFT dt=%lf msec, RIGHT dt=%lf\n", 
                   (curr_frame_time_l - prev_frame_time_l),
                   (curr_frame_time_r - prev_frame_time_r));


            // Set to pace frame display and capture rate
           /*---------------------------------------------
	    * Saving image using the timestamp for saving 
	    * format for both left and right image
	    *--------------------------------------------*/
            char c = cvWaitKey(10);
            if( c == ESC_KEY )
            {
                sprintf(&snapshotname[9], "left_%8.4lf.jpg", curr_frame_time_l);
                cvSaveImage(snapshotname, frame_l);
                sprintf(&snapshotname[9], "right_%8.4lf.jpg", curr_frame_time_r);
                cvSaveImage(snapshotname, frame_r);
            	sprintf(&snapshotname[9], "right_%8.4lf.jpg", "Disp");
                cvSaveImage(snapshotname, disp);

	    }

            prev_frame_time_l=curr_frame_time_l;
            prev_frame_time_r=curr_frame_time_r;
        }

        cvReleaseCapture(&capture_l);
        cvReleaseCapture(&capture_r);
        cvDestroyWindow("Capture LEFT");
        cvDestroyWindow("Capture RIGHT");
    }

    else
    {
        cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);

        while(1)
        {
            if(cvGrabFrame(capture)) frame=cvRetrieveFrame(capture);
            // frame=cvQueryFrame(capture); // short for grab and retrieve
     
            if(!frame) break;
            else
            {
                clock_gettime(CLOCK_REALTIME, &frame_time);
                curr_frame_time=((double)frame_time.tv_sec * 1000.0) + 
                                ((double)((double)frame_time.tv_nsec / 1000000.0));
            }

            cvShowImage("Capture Example", frame);
            printf("Frame time = %u sec, %lu nsec, dt=%lf msec\n", 
                   (unsigned)frame_time.tv_sec, 
                   (unsigned long)frame_time.tv_nsec,
                   (curr_frame_time - prev_frame_time));

            // Set to pace frame capture and display rate
            char c = cvWaitKey(10);
            if( c == ESC_KEY )
            {
                sprintf(&snapshotname[9], "%8.4lf.jpg", curr_frame_time);
                cvSaveImage(snapshotname, frame);
            }

            prev_frame_time=curr_frame_time;
        }

        cvReleaseCapture(&capture);
        cvDestroyWindow("Capture Example");
    }
 
};

