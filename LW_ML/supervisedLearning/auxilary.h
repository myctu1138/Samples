#ifndef AUXILARY_FUNCTIONS
#define AUXILARY_FUNCTIONS

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include <map>

// ������������ ����� ������ � ������ �����
const int maxFileNameLen = 1000;

/*
// ��� ������: ������� getPredictedClassLabel
// ���������� ����� ��� �������, �������������� ������������
// � ������� ���������� ��������������.
//
// ����
// sample       - �������, ���������� ����������
//                ����� ����� � ������������ ���������
// model        - ��������� ������
// ���������
// ������������� ����� ������.
*/
typedef int (getPredictedClassLabel) (const cv::Mat & sample,
                                      const CvStatModel & model);


/*
// ������� ��������� ��������, �� �������
// ��������� ������������ ��������� �������������
// 
// API
// drawPartition(cv::Mat & img,
//               std::map<int, cv::Scalar> & classColors,
//               const cv::Mat & dataRanges,
//               const cv::Size stepsNum,
//               const CvStatModel & model,
//               getPredictedClassLabel * predictLabel)
// ����
// classColors       - ����������� ����� ������� ������� � �������.
//                     ���� ��� ������������� ������ �� ������ ������������,
//                     ����� �������� ��������� ����.
// dataRanges        - ���������� ������� ������� ��������� � �����������
//                     ������������ ���������. ������� 2x2: ������ �������
//                     �������� max � min ������ ����������, ������ -- ������.
// stepsNum          - ���������� ����� �� ������ ����������.
// model             - ��������� �������������
// predictLabel      - ��������� �� ������� ������������ ��� �������������
//                     ��������������
// 
// �����
// img               - ����������� � ������������� ���������
*/
void drawPartition(cv::Mat & img,
                   std::map<int, cv::Scalar> & classColors,
                   const cv::Mat & dataRanges,
                   const cv::Size stepsNum,
                   const CvStatModel & model,
                   getPredictedClassLabel * predictLabel);


/*
// ������� ��������� ����� ���������� ������������ ���������
// 
// API
// drawPoints(cv::Mat & img,
//            const cv::Mat & data,
//            const cv::Mat & classes,
//            const cv::Mat & ranges,
//            std::map<int, cv::Scalar> & classColors,
//            int drawingMode = 0);
// ����
// data              - �������, � ������ ������ ������� ���������� ����� �����
//                     � ����������� ������������
// classes           - ����� ������� �����
// ranges            - ���������� ������� ������� ��������� � �����������
//                     ������������ ���������. ������� 2x2: ������ ������� ��������
//                     max � min ������ ����������, ������ -- ������.
// classColors       - ����������� ����� ������� ������� � �������.
//                     ���� ��� ������������� ������ �� ������ ������������,
//                     ����� �������� ��������� ����.
// drawingMode       - ��� ��������� �����: 0 - ����,
//                     1 - ����������, 2 - ������ �����.
//
// �����
// img               - ����������� � ������������� �������
*/
void drawPoints(cv::Mat & img,
                const cv::Mat & data,
                const cv::Mat & classes,
                const cv::Mat & ranges,
                std::map<int, cv::Scalar> & classColors,
                int drawingMode = 0);


/*
// ������� ��������� ������������ � ����������� �������� ������ ����������
// 
// API
// cv::Mat getRanges(const cv::Mat & data);
//
// ����
// data              - �������, � ������ ������ ������� ���������� ����� �����
//                     � ����������� ������������
//
// ���������
// ������� 2x<���-�� ����������>: � ������ ������� max � min
// ��� ��������������� ����������.
*/
cv::Mat getRanges(const cv::Mat & data);


/*
// ������� ������ ��������� � �������� ������� �� YAML-�����.
// 
// API
// void readDatasetFromFile(Mat & featuresTrain,
//                          Mat & classesTrain,
//                          Mat & featuresTest,
//                          Mat & classesTest)
// �����
// featuresTrain - ������� ������� ��������� ��� �����������
//                 ��������� �������
// classesTrain  - ������ ����������� ��������� �������
// featuresTest  - ������� ������� ��������� ��� �����������
//                 �������� �������
// classesTest   - ������ ����������� �������� �������
// 
*/
void readDatasetFromFile(cv::Mat & featuresTrain,
                         cv::Mat & classesTrain,
                         cv::Mat & featuresTest,
                         cv::Mat & classesTest);

#endif