#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_filter2D.exe <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    // константы для определения названия окон
    const char *initialWinName = "Initial Image", *resultWinName = "Filter2D";    
    // константы для хранения ядра фильтра
    const float kernelData[] = {-0.1f, 0.2f, -0.1f,
                                 0.2f, 3.0f,  0.2f,
                                -0.1f, 0.2f, -0.1f};
    const Mat kernel(3, 3, CV_32FC1, (float *)kernelData);

    // объекты для хранения исходного и результирующего изображений
	Mat src, dst;
    
    // проверка аргументов командной строки
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }
    
    // загрузка изображения
	src = imread(argv[1], 1); 
    
    // применение фильтра
    filter2D(src, dst, -1, kernel);
	
    // отображение исходного изображения и результата применения фильтра
	namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
	imshow(initialWinName, src);
    namedWindow(resultWinName, CV_WINDOW_AUTOSIZE);
    imshow(resultWinName, dst);
	waitKey();
    
    // закрытие окон
    destroyAllWindows();
    // освобождение ресурсов
    src.release();
    dst.release();
	return 0;
}
