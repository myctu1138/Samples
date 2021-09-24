#ifndef ERROR_METRICS_H
#define ERROR_METRICS_H

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

/*
// ������� ���������� ������ �������������.
// 
// API
// float getClassificationError(const Mat & samples,
//                              const Mat & classes,
//                              const CvStatModel & model,
//                              int (*predict) (const cv::Mat & sample,
                                      const CvStatModel & model))
// �����
// samples    - ������� ������� ��������� ����������� �������.
// classes    - ������ ����������� �������
//              (��� �������� ������������ ������������).
// model      - ��������� ������.
// predict    - ������� ������������ ��� ������ ������.
// 
*/
float getClassificationError(const cv::Mat & samples,
                             const cv::Mat & classes,
                             const CvStatModel & model,
                             int (*predict) (const cv::Mat & sample,
                                      const CvStatModel & model));

#endif