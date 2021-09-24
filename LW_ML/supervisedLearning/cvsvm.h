#ifndef SUPPORT_VECTOR_MACHINE
#define SUPPORT_VECTOR_MACHINE

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

/*
// ������� ������ ���������� ��������� �������� SVM � �������.
// 
// API
// CvSVMParams readSVMParams()
// 
// ���������
// ��������� ��������� �������� SVM.
// 
*/
CvSVMParams readSVMParams();


/*
// ������� �������� SVM-������.
// 
// API
// void trainSVM(const cv::Mat & trainSamples,
//               const cv::Mat & trainClasses,
//               const CvSVMParams & params,
//               CvSVM & svm)
// 
// ����
// trainSamples  - ������� ������� ��������� ��� �����������
//                 ��������� �������
// trainClasses  - ������ ����������� ��������� �������
// params        - ��������� ��������� ��������
// 
// ����
// svm           - ��������� SVM-������
*/
void trainSVM(const cv::Mat & trainSamples,
              const cv::Mat & trainClasses,
              const CvSVMParams & params,
              CvSVM & svm);


/*
// ������� ������������ SVM-������.
// 
// API
// int getSVMPrediction(const cv::Mat & sample,
//                      const CvStatModel & model)
// 
// ����
// sample  - �������, ���������� ���������� ����� �����
//           � ������������ ���������
// model   - ��������� SVM-������
// 
// ���������
// ������������� �����
*/
int getSVMPrediction(const cv::Mat & sample,
                     const CvStatModel & model);



cv::Mat getSupportVectors(const CvSVM & svm);



#endif