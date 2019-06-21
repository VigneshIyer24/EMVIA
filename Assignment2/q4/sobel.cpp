#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "sobel.h"
using namespace cv;
using namespace std;

void Sobel_Operator(Mat imgOriginal)
{
		int scale = 1;
 		int delta = 0;
  		int ddepth = CV_16S;
		Mat grad_x, grad_y;
 		Mat abs_grad_x, abs_grad_y;
 		Mat grad;
		Mat imgGrayscale;
		GaussianBlur( imgOriginal, imgOriginal, Size(3,3), 0, 0, BORDER_DEFAULT );
		cvtColor( imgOriginal, imgGrayscale, CV_BGR2GRAY );
		namedWindow("Sobel_Edge", CV_WINDOW_AUTOSIZE );
		namedWindow("Original",CV_WINDOW_AUTOSIZE);
  		/// Generate grad_x and grad_y
   		Sobel( imgGrayscale, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  		convertScaleAbs( grad_x, abs_grad_x );

  		/// Gradient Y
  		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  		Sobel( imgGrayscale, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  		convertScaleAbs( grad_y, abs_grad_y );

  		/// Total Gradient (approximate)
  		addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
		imshow("Original",imgOriginal);
  		imshow( "SobelEdge", grad );
}
