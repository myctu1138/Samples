#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;

// размерность пространства признаков
const int d = 2;

// функция истинной зависимости целевого признака
// от остальных
int f(Mat sample)
{
    return (int)((sample.at<float>(0) < 0.5f &&
                  sample.at<float>(1) < 0.5f) ||
                 (sample.at<float>(0) > 0.5f &&
                  sample.at<float>(1) > 0.5f));
}

int main(int argc, char* argv[])
{
    // объем генерируемой выборки
    int n = 2000;
    // объем обучающей части выборки
    int n1 = 1000;
    
    // матрица признаковых описаний объектов
    Mat samples(n, d, CV_32F);
    // номера классов (матрица значений целевой переменной)
    Mat labels(n, 1, CV_32S);
    // генерируем случайным образом точки
    // в пространстве признаков
    randu(samples, 0.0f, 1.0f);

    // вычисляем истинные значения целевой переменной
    for (int i = 0; i < n; ++i)
    {
        labels.at<int>(i) = f(samples.row(i));
    }

    // создаем маску прецедентов, которые будут
    // использоваться для обучения: используем n1
    // первых прецедентов
    Mat trainSampleMask(1, n1, CV_32S);
    for (int i = 0; i < n1; ++i)
    {
        trainSampleMask.at<int>(i) = i;
    }

    // используем SVM типа C_SVC и радиальным ядром
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    params.gamma = 1.0;
    params.C = 1.0;

    CvSVM svm;
    svm.train(samples, labels,
              Mat(), trainSampleMask, params);
    svm.save("model.yml", "simpleSVMModel");
    
    // вычисляем ошибку на обучающей выборке
    Mat predictions;
    svm.predict(samples.rowRange(0, n1), predictions);
    float trainError = 0.0f;
    for (int i = 0; i < n1; ++i)
    {
        trainError += (labels.at<int>(i) != 
                       (int)(predictions.at<float>(i)));
    }
    trainError /= float(n1);

    // вычисляем ошибку на тестовой выборке
    predictions = Mat();
    svm.predict(samples.rowRange(n1, n), predictions);
    float testError = 0.0f;
    for (int i = 0; i < n - n1; ++i)
    {
        testError += (labels.at<int>(n1 + i) != 
                      (int)(predictions.at<float>(i)));
    }
    testError /= float(n - n1);

    printf("train error = %.4f\ntest error = %.4f\n",
           trainError, testError);
    
    return 0;
}
