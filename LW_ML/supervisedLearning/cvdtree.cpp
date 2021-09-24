#include "cvdtree.h"

using namespace cv;


/*
// Функция обучения дерева решений.
// 
// API
// void trainDTree(const cv::Mat & trainSamples,
//                 const cv::Mat & trainClasses,
//                 const CvDTreeParams & params,
//                 CvDTree & dtree)
// 
// ВХОД
// trainSamples  - матрица входных признаков для прецедентов
//                 обучающей выборки
// trainClasses  - классы прецедентов обучающей выборки
// params        - параметры алгоритма обучения
// 
// ВХОД
// dtree         - обученное дерево решений
*/
void trainDTree(const Mat & trainSamples,
                const Mat & trainClasses,
                const CvDTreeParams & params,
                CvDTree & dtree)
{
    /* =================================================================== */
    //  Напишите код, запускающий алгоритм обучения дерева решений.
    //  Для обучения должны использоваться все прецеденты обучающей выборки
    //  и все признаки, пропущенных значений нет, все признаки кроме целевого
    //  количественные.
    /* ------------------------------------------------------------------- */







    /* =================================================================== */
}


/*
// Функция предсказания с помощью дерева решений.
// 
// API
// int getDTreePrediction(const cv::Mat & sample,
//                        const CvStatModel & model)
// 
// ВХОД
// sample  - матрица, содержащая координаты одной точки
//           в пространстве признаков
// model   - обученное дерево решений
// 
// РЕЗУЛЬТАТ
// Предсказанный класс
*/
int getDTreePrediction(const Mat & sample, const CvStatModel & model)
{
    // Приводим тип стат. модели к дереву решений
    const CvDTree & dtree = dynamic_cast<const CvDTree &>(model);
    int prediction = 0;
    /* =================================================================== */
    //  Напишите код, запускающий алгоритм предсказания для дерева решений.
    /* ------------------------------------------------------------------- */






    /* =================================================================== */
    return prediction;
}


/*
// Функция чтения параметров алгоритма обучения дерева решений с консоли.
// 
// API
// CvSVMParams readSVMParams()
// 
// РЕЗУЛЬТАТ
// Параметры алгоритма обучения дерева решений.
// 
*/
CvDTreeParams readDTreeParams()
{
    CvDTreeParams params;
    
    // Не строим суррогатные разбиения.
    params.use_surrogates = false;
    // Не храним поддеревья, удаленные в ходе обрезки дерева (pruning).
    params.truncate_pruned_tree = true;
    // Параметр алгоритма обрезания дерева.
    params.use_1se_rule = true;

    // Максимальная высота (глубина) дерева.
    printf("maximal tree depth = ");
    scanf("%d", &(params.max_depth));

    // Минимальное количество прецедентов обучающей выборке,
    // при котором выполняется дальнейшее разбиение.
    printf("minimal number of samples in leaf = ");
    scanf("%d", &(params.min_sample_count));

    // Количество блоков в скользящем контроле (cross-validation).
    int doPruning = 0;
    printf("apply pruning (0/1) = ");
    scanf("%d", &(doPruning));
    // Т.к. данных в тестовых задачах немного,
    // используем пятикратный скользящий контроль.
    params.cv_folds = (doPruning == 0) ? 0 : 5;

    return params;
}

