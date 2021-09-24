#include "cvdtree.h"

using namespace cv;


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
void trainDTree(const Mat & trainSamples,
                const Mat & trainClasses,
                const CvDTreeParams & params,
                CvDTree & dtree)
{
    /* =================================================================== */
    //  �������� ���, ����������� �������� �������� ������ �������.
    //  ��� �������� ������ �������������� ��� ���������� ��������� �������
    //  � ��� ��������, ����������� �������� ���, ��� �������� ����� ��������
    //  ��������������.
    /* ------------------------------------------------------------------- */







    /* =================================================================== */
}


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
int getDTreePrediction(const Mat & sample, const CvStatModel & model)
{
    // �������� ��� ����. ������ � ������ �������
    const CvDTree & dtree = dynamic_cast<const CvDTree &>(model);
    int prediction = 0;
    /* =================================================================== */
    //  �������� ���, ����������� �������� ������������ ��� ������ �������.
    /* ------------------------------------------------------------------- */






    /* =================================================================== */
    return prediction;
}


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
CvDTreeParams readDTreeParams()
{
    CvDTreeParams params;
    
    // �� ������ ����������� ���������.
    params.use_surrogates = false;
    // �� ������ ����������, ��������� � ���� ������� ������ (pruning).
    params.truncate_pruned_tree = true;
    // �������� ��������� ��������� ������.
    params.use_1se_rule = true;

    // ������������ ������ (�������) ������.
    printf("maximal tree depth = ");
    scanf("%d", &(params.max_depth));

    // ����������� ���������� ����������� ��������� �������,
    // ��� ������� ����������� ���������� ���������.
    printf("minimal number of samples in leaf = ");
    scanf("%d", &(params.min_sample_count));

    // ���������� ������ � ���������� �������� (cross-validation).
    int doPruning = 0;
    printf("apply pruning (0/1) = ");
    scanf("%d", &(doPruning));
    // �.�. ������ � �������� ������� �������,
    // ���������� ����������� ���������� ��������.
    params.cv_folds = (doPruning == 0) ? 0 : 5;

    return params;
}

