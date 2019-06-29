/*
 *
 *  Example by Sam Siewert 
 *
 *  Updated 12/6/18 for OpenCV 3.1
 * This code uses frame differencing 
 * for background elemination and uses
 * moments to track the centroid and 
 * uchar to edit pixel values of the frame.
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

char difftext[20];
char fileppm[50];
int frames=0;
int X,Y;

int main( int argc, char** argv )
{
    Mat mat_frame, mat_gray, mat_diff, mat_gray_prev, COM_detect, med_frame;
    VideoCapture vcap("DK_Laser.mpeg");
    unsigned int diffsum, maxdiff;
    double percent_diff;
//    int X,Y;

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

    while(!vcap.read(mat_frame)) {
	std::cout << "No frame" << std::endl;
	cv::waitKey(33);
    }
	
    cv::cvtColor(mat_frame, mat_gray, CV_BGR2GRAY);

    mat_diff = mat_gray.clone();
    mat_gray_prev = mat_gray.clone();

    maxdiff = (mat_diff.cols)*(mat_diff.rows)*255;

    while(1)
    {
	if(!vcap.read(mat_frame)) {
		std::cout << "No frame" << std::endl;
		break;
		
	}
	
	cv::cvtColor(mat_frame, mat_gray, CV_BGR2GRAY);

	absdiff(mat_gray_prev, mat_gray, mat_diff);

	// worst case sum is resolution * 255
	diffsum = (unsigned int)cv::sum(mat_diff)[0]; // single channel sum

	percent_diff = ((double)diffsum / (double)maxdiff)*100.0;

        printf("percent diff=%lf\n", percent_diff);
        sprintf(difftext, "%8d",  diffsum);

        // tested in ERAU Jetson lab
	if(percent_diff > 0.5) cv::putText(mat_diff, difftext, cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
/*	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    	{*/
        //	medianBlur ( mat_diff, mat_diff, 3 );
    //	}	
	threshold( mat_diff, COM_detect, 200,255,THRESH_BINARY ); 
	// find moments of the image
	Moments m = moments(COM_detect,true);
	   
	//Define Centroids
	Point p(m.m10/m.m00, m.m01/m.m00);
 	X=m.m10/m.m00;
	Y=m.m01/m.m00;
	// coordinates of the centroid
	cout<< "The Centroid = ";
	cout << X ;
	cout << ",";
	cout << Y << endl;	
	/*------------------------------------
	 * Draw cross-hairs. uchar is used 
	 * since the image is a grayscale map
	 *-----------------------------------*/
	    
 	for(int i=1;i<4;i++)
	{
	/*	med_frame.at<Vec3b>(Y,X-i)[0]=255;
		med_frame.at<Vec3b>(Y,X+i)[0]=255;
		med_frame.at<Vec3b>(Y,X-i)[1]=255;
                med_frame.at<Vec3b>(Y,X+i)[1]=255;
		med_frame.at<Vec3b>(Y,X-i)[2]=255;
                med_frame.at<Vec3b>(Y,X+i)[2]=255;
*/
		mat_gray.at<uchar>(Y,X-i)=255;
		mat_gray.at<uchar>(Y,X+i)=255;
	}

	for(int j=1;j<4;j++)
	{
	/*	med_frame.at<Vec3b>(Y-j,X)[0]=255;
		med_frame.at<Vec3b>(Y+j,X)[0]=255;
  	        med_frame.at<Vec3b>(Y-j,X)[1]=255;
                med_frame.at<Vec3b>(Y+j,X)[1]=255;
                med_frame.at<Vec3b>(Y-j,X)[2]=255;
                med_frame.at<Vec3b>(Y+j,X)[2]=255;
	*/
		mat_gray.at<uchar>(Y-j,X)=255;
		mat_gray.at<uchar>(Y+j,X)=255;   
	}
	imshow("Gray Example", mat_gray);
//	imshow("Gray Previous", mat_gray_prev);
	imshow("Gray Diff", mat_diff);

	/*--------------------------------------------
	 * Converting the frames to images and saving
	 * them in a .ppm format to be later converted
	 * to a video stream
	 * -------------------------------------------*/

	sprintf(fileppm,"images_q6/image%04d.ppm",frames);
	imwrite(fileppm,mat_diff);
	frames++;
        char c = cvWaitKey(33); // take this out or reduce
        if( c == 'q' ) break;

	mat_gray_prev = mat_gray.clone();
    }

};
