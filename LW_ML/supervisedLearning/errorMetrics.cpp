#include "errorMetrics.h"

using namespace cv;
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
float getClassificationError(const Mat & samples,
                             const Mat & classes,
                             const CvStatModel & model,
                             int (*predict) (const cv::Mat & sample,
                                      const CvStatModel & model))
{
    float error = 0.0f;
    /* =================================================================== */
    //  Напишите код вычисления ошибки классификации.
    //  Ошибка неправильной классификации вычисляется
    //  как отношение кол-ва правильно классифицированных
    //  объектов выборки к общему кол-ву объектов в выборке.
    //  Для получения предсказания требуется выполнить
    //  вызов (*predict)(sample, model), результатов которого будет
    //  номер предсказанного класса.
    /* ------------------------------------------------------------------- */








    /* =================================================================== */
    return error;
}