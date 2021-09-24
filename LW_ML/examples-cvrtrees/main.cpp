#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

// ����������� ������������ ���������
const int d = 2;

// ������� �������� ����������� �������� ��������
// �� ���������
int f(Mat sample)
{
    return (int)((sample.at<float>(0) < 0.5f &&
                  sample.at<float>(1) < 0.5f) ||
                 (sample.at<float>(0) > 0.5f &&
                  sample.at<float>(1) > 0.5f));
}

int main(int argc, char* argv[])
{
    // ����� ������������ �������
    int n = 2000;
    // ����� ��������� ����� �������
    int n1 = 1000;
    
    // ������� ����������� �������� ��������
    Mat samples(n, d, CV_32F);
    // ������ ������� (������� �������� ������� ����������)
    Mat labels(n, 1, CV_32S);
    // ���������� ��������� ������� �����
    // � ������������ ���������
    randu(samples, 0.0f, 1.0f);

    // ��������� �������� �������� ������� ����������
    for (int i = 0; i < n; ++i)
    {
        labels.at<int>(i) = f(samples.row(i));
    }

    // ������� ����� �����������, ������� �����
    // �������������� ��� ��������: ���������� n1
    // ������ �����������
    Mat trainSampleMask(1, n, CV_8U, Scalar(0));
    trainSampleMask.colRange(0, n1) = Scalar(1);

    // ����� ������� ��������� ��� �� 250 ��������
    // ������ �� ������ 10
    CvRTParams params;
    params.max_depth = 10;
    params.min_sample_count = 1;
    params.calc_var_importance = false;
    params.term_crit.type = CV_TERMCRIT_ITER;
    params.term_crit.max_iter = 250;

    CvRTrees rf;
    Mat varIdx(1, d, CV_8U, Scalar(1));
    Mat varTypes(1, d + 1, CV_8U, Scalar(CV_VAR_ORDERED));
    varTypes.at<uchar>(d) = CV_VAR_CATEGORICAL;
    rf.train(samples, CV_ROW_SAMPLE,
             labels, varIdx,
             trainSampleMask, varTypes,
             Mat(), params);
    rf.save("model-rf.yml", "simpleRTreesModel");
    
    // ��������� ������ �� ��������� �������
    float trainError = 0.0f;
    for (int i = 0; i < n1; ++i)
    {
        int prediction =
            (int)(rf.predict(samples.row(i)));
        trainError += (labels.at<int>(i) != prediction);
    }
    trainError /= float(n1);

    // ��������� ������ �� �������� �������
    float testError = 0.0f;
    for (int i = 0; i < n - n1; ++i)
    {
        int prediction = 
          (int)(rf.predict(samples.row(n1 + i)));
        testError +=
            (labels.at<int>(n1 + i) != prediction);
    }
    testError /= float(n - n1);

    printf("train error = %.4f\ntest error = %.4f\n",
           trainError, testError);
    
    return 0;
}
