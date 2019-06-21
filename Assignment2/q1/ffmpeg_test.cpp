#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <semaphore.h>
#include <pthread.h>
#include <opencv/highgui.h>
#include<opencv2/imgproc.hpp>



using namespace std;
using namespace cv;

int main()
{
	int frame_Cnt=0;
	char image_ppm[50];
	VideoCapture capture("big_buck.avi");
	
	if(!capture.isOpened())
	{
    		cout << "Error opening video stream or file" << endl;
    		return -1;
  	}

	while(1)
	{
		Mat frame;
		capture >> frame;
		frame_Cnt++;
		if(frame.empty())
			break;
		imshow( "Frame", frame );
		if(frame_Cnt==99)
		{
			sprintf(image_ppm,"frame_100th.ppm",frame_Cnt);
			imwrite(image_ppm,frame);
			imshow("100th_frame",frame);
			break;
		}
    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
  }

  // When everything done, release the video capture object
  capture.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;
}


