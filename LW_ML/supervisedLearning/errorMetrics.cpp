#include "errorMetrics.h"

using namespace cv;
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
float getClassificationError(const Mat & samples,
                             const Mat & classes,
                             const CvStatModel & model,
                             int (*predict) (const cv::Mat & sample,
                                      const CvStatModel & model))
{
    float error = 0.0f;
    /* =================================================================== */
    //  �������� ��� ���������� ������ �������������.
    //  ������ ������������ ������������� �����������
    //  ��� ��������� ���-�� ��������� ������������������
    //  �������� ������� � ������ ���-�� �������� � �������.
    //  ��� ��������� ������������ ��������� ���������
    //  ����� (*predict)(sample, model), ����������� �������� �����
    //  ����� �������������� ������.
    /* ------------------------------------------------------------------- */








    /* =================================================================== */
    return error;
}