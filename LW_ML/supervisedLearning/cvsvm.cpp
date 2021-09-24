#include <stdio.h>
#include "cvsvm.h"

#pragma warning(disable : 4996)

using namespace cv;


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
              CvSVM & svm)
{
    /* =================================================================== */
    //  �������� ���, ����������� �������� �������� SVM-��������������.
    //  ��� �������� ������ �������������� ��� ���������� ��������� �������
    //  � ��� ��������.
    /* ------------------------------------------------------------------- */





    /* =================================================================== */
}


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
int getSVMPrediction(const Mat & sample,
                     const CvStatModel & model)
{
    const CvSVM & svm = dynamic_cast<const CvSVM &>(model);
    int prediction = 0;
    /* =================================================================== */
    //  �������� ���, ����������� �������� ������������ SVM-��������������.
    /* ------------------------------------------------------------------- */





    /* =================================================================== */
    return prediction;
}


Mat getSupportVectors(const CvSVM & svm)
{
    Mat sv;
    /* =================================================================== */
    //  �������� ���, ��������� ����������� ������� sv ���� CV_32F
    //  �������� ��������� ������ svm.
    /* ------------------------------------------------------------------- */





    /* =================================================================== */
    return sv;
}


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
CvSVMParams readSVMParams()
{
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;

    // ������������� ��������� ��������� ������������� ������ �����������
    params.term_crit =
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000000, 0.0001);

    // ����������� �������� ��������� ������������ �������������
    printf("C = ");
    scanf("%lf", &(params.C));

    // �������� ����
    do
    {
        printf("Choose kernel:\n\t0 - Linear kernel K(u, v) = <u, v>\n");
        printf("\t1 - Polinomial kernel K(u, v) = (gamma * <u, v> + coef0) ^ degree\n");
        printf("\t2 - RBF kernel K(u, v) = exp(-gamma * ||u - v|| ^ 2)\n");
        printf("\t3 - Sigmoid kernel K(u, v) = tanh(gamma * <u, v> + coef0)\n");
        printf("kernel type = ");
        scanf("%d", &(params.kernel_type));
    }
    while (params.kernel_type < 0 || params.kernel_type > 3);
    
    // �������� ��������� ����
    switch (params.kernel_type)
    {
    case CvSVM::LINEAR: /* linear: u'*v */
        break;
    case CvSVM::POLY: /* polynomial: (gamma*u'*v + coef0)^degree */
        {
            printf("gamma = ");
            scanf("%lf", &(params.gamma));
            printf("coef0 = ");
            scanf("%lf", &(params.coef0));
            printf("degree = ");
            scanf("%lf", &(params.degree));
        }; break;
    case CvSVM::RBF: /* radial basis function: exp(-gamma*|u-v|^2) */
        {
            printf("gamma = ");
            scanf("%lf", &(params.gamma));
        }; break;
    case CvSVM::SIGMOID: /* sigmoid: tanh(gamma*u'*v + coef0) */
        {
            printf("gamma = ");
            scanf("%lf", &(params.gamma));
            printf("coef0 = ");
            scanf("%lf", &(params.coef0));
        }; break;
    }

    return params;
}

