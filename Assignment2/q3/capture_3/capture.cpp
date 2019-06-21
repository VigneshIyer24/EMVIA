/*
 *
 *  Example by Sam Siewert 
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <syslog.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define NSEC_TO_MSEC 1000000
#define NSEC_TO_SEC 1000000000
using namespace cv;
using namespace std;

int frame_Cnt=1;
struct timespec start_time;
struct timespec frame_time;
int main( int argc, char** argv )
{
    
	int FPS;
	double total_time;
	float jitter;
	cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    	CvCapture* capture = cvCreateCameraCapture(0);
    	IplImage* frame;
    	clock_gettime(CLOCK_REALTIME,&start_time);
    	while(1)
    	{
        	frame=cvQueryFrame(capture);
     
        	if(!frame) break;
	
        	cvShowImage("Capture Example", frame);
		/*--------------------------------------
		 * clock_gettime will calulcate the time 
		 * having variables in seconds and ns
		 * for struct timespec
		 * -------------------------------------*/
		clock_gettime(CLOCK_REALTIME,&frame_time);
		syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt,(int)(frame_time.tv_sec-start_time.tv_sec),(int)((frame_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_MSEC));
//		printf("Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt,(int)(frame_time.tv_sec-start_time.tv_sec),(int)((frame_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_MSEC));
		if(frame_Cnt==1800)
		{
			syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt,(int)(frame_time.tv_sec-start_time.tv_sec),(int)((frame_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_MSEC));
        		printf("Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt,(int)(frame_time.tv_sec-start_time.tv_sec),(int)((frame_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_MSEC));
			total_time=(int)(frame_time.tv_sec-start_time.tv_sec)+(double)((frame_time.tv_nsec-start_time.tv_nsec)/NSEC_TO_SEC);
			/*-----------------------------------------
			 * Considering an ideal FPS= 30, thus 
			 * 1800 frames in a minute i.e. 60 seconds
			 * ----------------------------------------*/
			
			jitter=total_time-60;
			
			/*-----------------------------------------------
			 * We are calculating for a value for 1800 frames
			 * ---------------------------------------------*/
			
			FPS=1800/total_time;
			printf("FPS=%d, Total jitter=%f\n",FPS,jitter);
			break;
		}	
		frame_Cnt++;
        	char c = cvWaitKey(33);
        	if( c == 27 ) break;
    	}

    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture Example");
    
};
