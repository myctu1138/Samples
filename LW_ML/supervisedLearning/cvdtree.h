#ifndef DECISION_TREE
#define DECISION_TREE

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

/*
// ������� ������ ���������� ��������� �������� ������ ������� � �������.
// 
// API
// CvSVMParams readSVMParams()
// 
// ���������
// ��������� ��������� �������� ������ �������.
// 
*/
CvDTreeParams readDTreeParams();


/*
// ������� �������� ������ �������.
// 
// API
// void trainDTree(const cv::Mat & trainSamples,
//                 const cv::Mat & trainClasses,
//                 const CvDTreeParams & params,
//                 CvDTree & dtree)
// 
// ����
// trainSamples  - ������� ������� ��������� ��� �����������
//                 ��������� �������
// trainClasses  - ������ ����������� ��������� �������
// params        - ��������� ��������� ��������
// 
// ����
// dtree         - ��������� ������ �������
*/
void trainDTree(const cv::Mat & trainSamples,
                const cv::Mat & trainClasses,
                const CvDTreeParams & params,
                CvDTree & dtree);


/*
// ������� ������������ � ������� ������ �������.
// 
// API
// int getDTreePrediction(const cv::Mat & sample,
//                        const CvStatModel & model)
// 
// ����
// sample  - �������, ���������� ���������� ����� �����
//           � ������������ ���������
// model   - ��������� ������ �������
// 
// ���������
// ������������� �����
*/
int getDTreePrediction(const cv::Mat & sample,
                       const CvStatModel & model);


#endif