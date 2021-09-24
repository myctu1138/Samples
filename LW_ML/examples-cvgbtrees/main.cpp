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

    // будем обучать модель градиентного бустинга
    // деревьев решений из 250 деревьев высоты 3 и
    // скоростью обучения 0.5, без использования подвыборок.
    // Т.к. решается задача классификации используем
    // функцию потерь DEVIANCE_LOSS
    CvGBTreesParams params;
    params.max_depth = 3;
    params.min_sample_count = 1;
    params.weak_count = 250;
    params.shrinkage = 0.5f;
    params.subsample_portion = 1.0f;
    params.loss_function_type = CvGBTrees::DEVIANCE_LOSS;

    CvGBTrees gbt;
    Mat varIdx(1, d, CV_8U, Scalar(1));
    Mat varTypes(1, d + 1, CV_8U, Scalar(CV_VAR_ORDERED));
    varTypes.at<uchar>(d) = CV_VAR_CATEGORICAL;
    gbt.train(samples, CV_ROW_SAMPLE,
              labels, varIdx,
              trainSampleMask, varTypes,
              Mat(), params);
    gbt.save("model-gbt.yml", "simpleGBTreesModel");
    
    // вычисляем ошибку на обучающей выборке
    float trainError = 0.0f;
    for (int i = 0; i < n1; ++i)
    {
        int prediction =
            (int)(gbt.predict(samples.row(i)));
        trainError += (labels.at<int>(i) != prediction);
    }
    trainError /= float(n1);

    // вычисляем ошибку на тестовой выборке
    float testError = 0.0f;
    for (int i = 0; i < n - n1; ++i)
    {
        int prediction = 
          (int)(gbt.predict(samples.row(n1 + i)));
        testError +=
            (labels.at<int>(n1 + i) != prediction);
    }
    testError /= float(n - n1);

    printf("train error = %.4f\ntest error = %.4f\n",
           trainError, testError);
    
    return 0;
}
