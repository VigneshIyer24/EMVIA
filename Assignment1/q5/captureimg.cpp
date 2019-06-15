#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

// Default resolution is 180p
#define VRES_ROWS (180)
#define HRES_COLS (240)

#define ESC_KEY (27)

// Buffer for highest resolution visualization possible
unsigned char imagebuffer[1440*2560*3]; // 1440 rows, 2560 cols/row, 3 channel

int main(int argc, char **argv)
{
    int hres = HRES_COLS;
    int vres = VRES_ROWS;
    Size size(180,240);
    Mat basicimage(vres, hres, CV_8UC3, imagebuffer);
    Mat editImage(vres, hres, CV_8UC3, imagebuffer);
    Mat src1(vres, hres, CV_8UC3, imagebuffer);

    Scalar value;
    int frameCnt=0;
    int up,down,side_left,side_right;

    printf("hres=%d, vres=%d\n", hres, vres);

    // interactive computer vision loop 
    namedWindow("Profile Visualization", CV_WINDOW_AUTOSIZE);

    // read in default image
    if(vres == 180)
	basicimage = imread("Cows.jpg", CV_LOAD_IMAGE_COLOR);
    else if(vres == 360)
        basicimage = imread("Cows.jpg", CV_LOAD_IMAGE_COLOR);
    else if(vres == 720)
        basicimage = imread("Cows.jpg", CV_LOAD_IMAGE_COLOR);
    else if(vres == 1080)
        basicimage = imread("Cows.jpg", CV_LOAD_IMAGE_COLOR);
    else if(vres == 1440)
        basicimage = imread("Cows.jpg", CV_LOAD_IMAGE_COLOR);

    if(!basicimage.data)  // Check for invalid input
    {
        printf("Could not open or find the refernece starting image\n");
        exit(-1);
    }
	cv::resize(basicimage,basicimage,size,0,0,INTER_LINEAR);	
	up= 4;
	down=4;
	side_left=4;
	side_right=4;
	editImage=basicimage;
	src1=editImage;
	value = Scalar(0,0,255);
	copyMakeBorder( basicimage, editImage, up, down, side_left, side_right, BORDER_CONSTANT, value );
	//Assinging intensity of BGR to 3 variables	
	/*Vec3b changeInt=editImage.at<Vec3b>(92,162);
	uchar blue = changeInt.val[0];
	uchar green = changeInt.val[1];
	uchar red = changeInt.val[2];
	*/
	
		editImage.at<Vec3b>(122,91)[0]=0;
		editImage.at<Vec3b>(122,91)[1]=255;
		editImage.at<Vec3b>(122,91)[2]=255;
		
		editImage.at<Vec3b>(122,93)[0]=0;
		editImage.at<Vec3b>(122,93)[1]=255;
		editImage.at<Vec3b>(122,93)[2]=255;
		
		editImage.at<Vec3b>(121,92)[0]=0;
		editImage.at<Vec3b>(121,92)[1]=255;
		editImage.at<Vec3b>(121,92)[2]=255;
		
		editImage.at<Vec3b>(123,92)[0]=0;
		editImage.at<Vec3b>(123,92)[1]=255;
		editImage.at<Vec3b>(123,92)[2]=255;
		
		
	
	cout << "Width : " << basicimage.cols << endl;
	cout << "Height: " << basicimage.rows << endl;
	// Create an IplImage mapping onto the basicimage Mat object
    //
    IplImage basicimageImg = basicimage;


    // Interactive LOOP
    //
    while(1)
    {
        frameCnt++;
	
        // Write a zero value into the image buffer
        //
        basicimageImg.imageData[frameCnt] = (unsigned char)0;
	 imshow("Original", basicimage);
        imshow("Edit", editImage);  
	// imshow("Edit1", src1);  

        // set pace on shared memory sampling in msecs
        char c = cvWaitKey(10);

        if(c == ESC_KEY)
        {
            exit(1);
        }

    }
 
    return 1;

}
