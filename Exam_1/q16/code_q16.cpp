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


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sched.h>
#include <syslog.h>
#include <semaphore.h>
#define NSEC_TO_MSEC 1000000
#define NSEC_TO_SEC 1000000000


#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv; using namespace std;
double alpha=1.0;  int beta=10;  /* contrast and brightness control */

/**
*The image shall have four threads. The four threads
*
*will be working threads, brighten all the four quadrants 
*
*of the image. The fifth thread shall display the image.
*
*The Image matrix shall be a shared resource.
*
*/

#define 	QUADRANTS    4 /*Macro that defines the number of quardrants the image would be broken into*/
unsigned char imagebuffer[1440*2560*3];
Mat image(480,640,CV_8UC3,imagebuffer); /*global resource_1*/
Mat new_image; /*global resource_2*/
Size size(480,640);
struct timespec start_time;
struct timespec frame_time;
int frame_Cnt=0;
/**/
pthread_t mythread[2];/*Variable to store pthread*/
pthread_attr_t fifo_sched_attr;

pthread_attr_t orig_sched_attr;

/*
*This struct type shall pass the lower limit and upper
*
*limit of the pixel for each of the four quadrants
*
*of the image
*/

typedef struct param
{
	int LOWER_LIMIT_ROW;
	int UPPER_LIMIT_ROW;
	int LOWER_LIMIT_COL;
	int UPPER_LIMIT_COL;

}threadparams;
typedef struct
{
    int threadIdx;
    unsigned long long sequencePeriods;
} threadParams_t;

threadparams *my_params[QUADRANTS];/*An array of type threadparams that stores the details for each thread*/
threadParams_t threadParams[4];


/**
*@func: brighten
*
*@description: a function that brightens the image
*
*@parameters: the row and the column limits of the image quadrant
*
*@return: void
*/
void *brighten0(void *)
{
	
	for( int y = 0; y < (image.rows)/2; y++ )
    	{
        	for( int x = 0; x <(image.cols)/2 ; x++ )
        	{ 
            		for( int c = 0; c < 3; c++ )
               		{ new_image.at<Vec3b>(y,x)[c] =saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );}
      
		}
    	}
	 
	return 0;

}
	
void *brighten1(void *)
{
	
	for( int y = (image.rows)/2; y < image.rows; y++ )
    	{
        	for( int x = 0; x <(image.cols)/2 ; x++ )
        	{ 
            		for( int c = 0; c < 3; c++ )
               		{ new_image.at<Vec3b>(y,x)[c] =saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );}
      
		}
    	}

	return 0;

}
void *brighten2(void *)
{
	
	for( int y = (image.rows)/2; y < image.rows; y++ )
    	{
        	for( int x = (image.cols)/2; x <image.cols ; x++ )
        	{ 
            		for( int c = 0; c < 3; c++ )
               		{ new_image.at<Vec3b>(y,x)[c] =saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );}
      
		}
    	}
	
	return 0;

}
void *brighten3(void *)
{
	
	for( int y = 0; y < (image.rows)/2; y++ )
    	{
        	for( int x = (image.cols)/2; x <image.cols ; x++ )
        	{ 
            		for( int c = 0; c < 3; c++ )
               		{ new_image.at<Vec3b>(y,x)[c] =saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );}
      
		}
    	}
	return 0;

}
/**
*@func: main func
*
*@description: function that provides program entry point
*
*@parameters: command line arguments to parse file name
*
*@return: int
*/
int main(int argc, char** argv)
{
	/**
	*First, let's pass in all the parameters for the image.
	*
	*I shall not do this within the threads, so as to reduce latency.
	*
	*The image matrix is a global variable and hence a shared resouce
	*
	*passed on to different threads
	*/
	
	

  	

  

    /**
    *Let us now define the four quadrants of each thread
    *
    */
//    cv::resize(image,image,size,0,0,INTER_LINEAR);
	  
	threadParams_t threadParams[QUADRANTS];
        pthread_attr_t rt_sched_attr[QUADRANTS];
        int rt_max_prio, rt_min_prio;
        struct sched_param rt_param[QUADRANTS];
        struct sched_param main_param;
        pthread_attr_t main_attr;
        pid_t mainpid;
	
	image = imread("Musk.ppm",CV_LOAD_IMAGE_COLOR ); // read in image file
	cv::resize(image,image,size,0,0,INTER_LINEAR);
        cpu_set_t threadcpu;
	int rc; 
   
	new_image = Mat::zeros( image.size(), image.type() );
  	std::cout<<"* Enter alpha brighten factor [1.0-3.0]: ";std::cin>>alpha;
    	std::cout<<"* Enter beta contrast increase value [0-100]: "; std::cin>>beta;

 
	clock_gettime(CLOCK_REALTIME,&start_time);
   
	rc= pthread_create(&mythread[0],NULL,brighten0,NULL);
       	if(rc<0)
    	{
    		printf( "problem while creating thread[0]\n");
    		return -1;
	}
	rc= pthread_create(&mythread[1],NULL,brighten1,NULL);
       	if(rc<0)
    	{
    		printf( "problem while creating thread[1]\n");
    		return -1;
	}
	 
	rc= pthread_create(&mythread[2],NULL,brighten2,NULL);
       	if(rc<0)
    	{
    		printf( "problem while creating thread[0]\n");
    		return -1;
	}
	rc= pthread_create(&mythread[3],NULL,brighten3,NULL);
       	if(rc<0)
    	{
    		printf( "problem while creating thread[1]\n");
    		return -1;
	}

       for(int i=0;i<4;i++)
    {
    	pthread_join(mythread[i], NULL);
    }
	
  imshow("Bright",new_image);
clock_gettime(CLOCK_REALTIME,&frame_time);
syslog(LOG_CRIT,"Frame=%d,Time: sec=%d, msec=%d\n",frame_Cnt,(int)(frame_time.tv_sec-start_time.tv_sec),(int)((start_time.tv_nsec)/NSEC_TO_MSEC));
   cvWaitKey();
   return 0;
}

