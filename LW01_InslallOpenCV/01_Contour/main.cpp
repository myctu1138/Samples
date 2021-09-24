#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
	const char *srcWinName = "src", *contourWinName = "contour";
	namedWindow(srcWinName, 1); 
	namedWindow(contourWinName,1);
	//load original image
	Mat src = imread("apple.bmp", 1);
	if (src.data == 0)
	{
		printf("Incorrect image name or format.\n");
		return 1;
	}
	//create a single channel 1 byte image (i.e. gray-level image)
	//make a copy of the original image to draw the detected contour
	Mat copy = src.clone();
	Mat gray, grayThresh;
	cvtColor(src, gray, CV_BGR2GRAY);
	threshold(gray, grayThresh, 120, 255, CV_THRESH_BINARY);
	//find the contour
	vector<vector<Point> > contours;
	findContours( grayThresh, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	Scalar color( 0, 255, 0 );
	drawContours(copy, contours, -1, color, 2);
	imshow(contourWinName, copy);
	imshow(srcWinName, src);
	waitKey(0);
	gray.release();
	grayThresh.release();
	copy.release();
	src.release();
	return 0;
}