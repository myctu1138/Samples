#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <opencv2/opencv.hpp>

using namespace cv;

extern const int kMenuTabs;
extern const char* winNames[];

int applyBlur(const Mat &src, Mat &dst);
int applyMedianBlur(const Mat &src, Mat &dst);
int applyOperation(const Mat &src, const int operationIdx);

#endif
