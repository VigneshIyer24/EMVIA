#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "canny.h" 
#include "sobel.h"
#include<iostream>
#include <time.h>
#include <syslog.h>

#define NSEC_TO_MSEC 1000000
#define NSEC_TO_SEC 1000000000

using namespace cv;

struct timespec start_time;
struct timespec canny_time, sobel_time;

int frame_Cnt_canny=1;
int frame_Cnt_sobel=1;
int FPS_canny,FPS_sobel;
double total_time_canny,total_time_sobel;
float jitter_canny,jitter_sobel;
int main()
{
 	cv::VideoCapture capWebcam(0);   // declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam

 	if (capWebcam.isOpened() == false)  //  To check if object was associated to webcam successfully
 	{
  		std::cout << "error: Webcam connect unsuccessful\n"; // if not then print error message
 		 return(0);            // and exit program
 	} 	
	Mat imgOriginal;
	Mat imgCanny;
	
	/*-----------------------------------
	 * For checking if the key pressed
	 * Esc key or key including uppercase
	 * and lowercase 'c' and 's'
	 * ----------------------------------*/
	char charCheckForEscKey = 0;
 	char EscKey=0;

	printf("Press 'c' or 'C' for Canny Edge operation\n");
	printf("Press 's' or 'S' for Sobel Operator\n");
	
	char m=std::cin.get();

	 /*-------------------------------------------
         * Start intial measurement of time with the
         * variables of the struct timespec having
         * time measured in seconds and nanoseconds
         * ------------------------------------------*/

        clock_gettime(CLOCK_REALTIME,&start_time);

 	
	/*--------------------
	 * Canny Edge detector
	 * -------------------*/

	while (charCheckForEscKey != 27 && capWebcam.isOpened() && (m==67 ||m==99))
	{            // until the Esc key is pressed or webcam connection is lost
  		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);   // get next frame

  		if (!blnFrameReadSuccessfully || imgOriginal.empty())
		{    // if frame read unsuccessfully
   			std::cout << "error: frame can't read \n";      // print error message
   			break;
  		}

		/*------------------------------------
		 * The Canny edge operator included in
		 * the header file canny.h
		 * -----------------------------------*/

		Canny_Edge(imgOriginal,imgCanny);

  		clock_gettime(CLOCK_REALTIME,&canny_time);
		syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt_canny,(int)(canny_time.tv_sec-start_time.tv_sec),(int)(canny_time.tv_nsec)/NSEC_TO_MSEC);
		if(frame_Cnt_canny==1800)
		{
			syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt_canny,(int)(canny_time.tv_sec-start_time.tv_sec),(int)((canny_time.tv_nsec)/NSEC_TO_MSEC));
        	//	printf("Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt_canny,(int)(canny_time.tv_sec-start_time.tv_sec),(int)((canny_time.tv_nsec)/NSEC_TO_MSEC));
			total_time_canny=(int)(canny_time.tv_sec-start_time.tv_sec)+(double)((canny_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_SEC);
			/*-----------------------------------------
			 * Considering an ideal FPS= 30, thus 
			 * 1800 frames in a minute i.e. 60 seconds
			 * ----------------------------------------*/
			
			jitter_canny=total_time_canny-60;
			
			/*-----------------------------------------------
			 * We are calculating for a value for 1800 frames
			 * ---------------------------------------------*/
			
			FPS_canny=1800/total_time_canny;
			syslog(LOG_CRIT,"FPS=%d, Total jitter=%f\n",FPS_canny,jitter_canny);
			break;
		}
	//	printf("Frame=%d\n",frame_Cnt_canny);	
		frame_Cnt_canny++;
		charCheckForEscKey = cvWaitKey(1);        // delay and get key press
 	}
	
	/*---------------
	 * Sobel Operator
	 * --------------*/

	while (EscKey != 27 && capWebcam.isOpened() && (m==115|| m== 83) )
	{            // until the Esc key is pressed or webcam connection is lost
  		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);   // get next frame
		if (!blnFrameReadSuccessfully || imgOriginal.empty())
                {    // if frame read unsuccessfully
                        std::cout << "error: frame can't read \n";      // print error message
                        break;
                }

		/*-------------------------------
		 * The Sobel operator included in
		 * the header file sobel.h
		 * ------------------------------*/
		Sobel_Operator(imgOriginal);

		clock_gettime(CLOCK_REALTIME,&sobel_time);
                syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt_sobel,(int)(sobel_time.tv_sec-start_time.tv_sec),(int)(sobel_time.tv_nsec)/NSEC_TO_MSEC);
                if(frame_Cnt_sobel==1800)
                {
                        syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt_sobel,(int)(sobel_time.tv_sec-start_time.tv_sec),(int)((sobel_time.tv_nsec)/NSEC_TO_MSEC));
//                        printf("Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt_sobel,(int)(sobel_time.tv_sec-start_time.tv_sec),(int)((sobel_time.tv_nsec)/NSEC_TO_MSEC));
                        total_time_sobel=(int)(sobel_time.tv_sec-start_time.tv_sec)+(double)((sobel_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_SEC);
                        /*-----------------------------------------
                         * Considering an ideal FPS= 30, thus 
                         * 1800 frames in a minute i.e. 60 seconds
                         * ----------------------------------------*/

                        jitter_sobel=total_time_sobel-60;

                        /*-----------------------------------------------
                         * We are calculating for a value for 1800 frames
                         * ---------------------------------------------*/

                        FPS_sobel=1800/total_time_sobel;
                        syslog(LOG_CRIT,"FPS=%d, Total jitter=%f\n",FPS_sobel,jitter_sobel);
                        break;
                }
	//	printf("Frame=%d\n",frame_Cnt_sobel);
                frame_Cnt_sobel++;
		EscKey=cvWaitKey(1);
	}

	return 0;
};
