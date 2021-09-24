#ifndef ERROR_METRICS_H
#define ERROR_METRICS_H

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"

/*
// Функция вычисления ошибки классификации.
// 
// API
// float getClassificationError(const Mat & samples,
//                              const Mat & classes,
//                              const CvStatModel & model,
//                              int (*predict) (const cv::Mat & sample,
                                      const CvStatModel & model))
// ВЫХОД
// samples    - матрица входных признаков прецедентов выборки.
// classes    - классы прецедентов выборки
//              (для проверки правильности предсказания).
// model      - обученная модель.
// predict    - функция предсказания для данной модели.
// 
*/
float getClassificationError(const cv::Mat & samples,
                             const cv::Mat & classes,
                             const CvStatModel & model,
                             int (*predict) (const cv::Mat & sample,
                                      const CvStatModel & model));

#endif