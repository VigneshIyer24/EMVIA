/***********************************************************************************
*Copyright (C) Vignesh Iyer
*
*This code shall not be used or distributed for purposes other than academic or 
*creative.Vignesh Iyer or the University of Colorado Boulder is not responsible 
*for misuse of this code. 
*
************************************************************************************/

/**
*@file: brighten_thread
*
*@description: a file that brightens the image using multihtreaded programming
*
*@author: Vignesh Iyer
*
*@date: 17th July 2019
*/ 

#include <iostream>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <syslog.h>
#include <semaphore.h>
#define NSEC_TO_MSEC 1000000
#define NSEC_TO_SEC 1000000000

struct timespec start_time;
struct timespec frame_time;
typedef struct
{
int threadIdx;
} threadParams_t;

#define NUM_THREADS 1
pthread_mutex_t mymutex;
pthread_t thread[NUM_THREADS];
using namespace cv; using namespace std;
Size size(480,640);
double alpha=1.0; int beta=10; /* contrast and brightness control */
Mat image;

int frame_Cnt=0;
/**
*@func: brighten
*
*@description: a function that brightens the image
*
*@parameters: the row and the column limits of the image quadrant
*
*@return: void
*/
void *image_brighten(void *)
{
	image = imread("Musk.ppm",CV_LOAD_IMAGE_COLOR ); // read in image file
	cv::resize(image,image,size,0,0,INTER_LINEAR);
	Mat new_image = Mat::zeros( image.size(), image.type() );
	Vec3b intensity;
	intensity.val[0] = 255;
	intensity.val[1] = 255;
	intensity.val[2] = 255;

	std::cout<<"* Enter alpha brighten factor [1.0-3.0]: ";std::cin>>alpha;
	std::cout<<"* Enter beta contrast increase value [0-100]: "; std::cin>>beta;
	for( int y =0; y < image.rows; y++ )
	{
		for( int x = 0; x < image.cols; x++ )
		{ 
			for( int c = 0; c < 3; c++ )
				new_image.at<Vec3b>(y,x)[c] =saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
		}
	}

	namedWindow("Original Image", 1); namedWindow("New Image", 2);
	imshow("Original Image", image);
 	imshow("New Image", new_image);
 
}
int main( int argc, char** argv )
{
	int i, rc, scope;
	//image=imread(argv[1]);
	threadParams_t threadParams[NUM_THREADS];
    	pthread_attr_t rt_sched_attr[NUM_THREADS];
    	int rt_max_prio, rt_min_prio;
    	struct sched_param rt_param[NUM_THREADS];
    	struct sched_param main_param;
    	pthread_attr_t main_attr;
    	pid_t mainpid;

	cpu_set_t threadcpu;
/*--------------------------------------
* clock_gettime will calulcate the time 
* having variables in seconds and ns
* for struct timespec
* -------------------------------------*/
	clock_gettime(CLOCK_REALTIME,&start_time);
	for(int i=0;i<NUM_THREADS;i++)
{

/*
  * Set the scheduling policy, attributes, etc for the thread

  */

rc=pthread_attr_init(&rt_sched_attr[i]);
rc=pthread_attr_setinheritsched(&rt_sched_attr[i], PTHREAD_EXPLICIT_SCHED);
rc=pthread_attr_setschedpolicy(&rt_sched_attr[i], SCHED_FIFO);
rc=pthread_attr_setaffinity_np(&rt_sched_attr[i], sizeof(cpu_set_t), &threadcpu);

rt_param[i].sched_priority=rt_max_prio-i;
pthread_attr_setschedparam(&rt_sched_attr[i], &rt_param[i]);
}

pthread_create(&thread[0],NULL,image_brighten, NULL);
for(i=0;i<NUM_THREADS;i++)
       pthread_join(thread[i], NULL);



clock_gettime(CLOCK_REALTIME,&frame_time);
syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt,(int)(frame_time.tv_sec-start_time.tv_sec),(int)((start_time.tv_nsec)/NSEC_TO_MSEC));

waitKey();
return 0;

}
