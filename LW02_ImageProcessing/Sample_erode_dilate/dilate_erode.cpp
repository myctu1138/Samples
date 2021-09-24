#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_erode_dilate.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image", 
               *erodeWinName = "erode", *dilateWinName = "dilate";
	Mat img, erodeImg, dilateImg, element;
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }
    // загрузка изображения
    img = imread(argv[1], 1);
    // вычисление эрозии и дилатации
    element = Mat();
    erode(img, erodeImg, element);
    dilate(img, dilateImg, element);
    // отображение исходного изображения и результата
    // применения морфологических операций "эрозия" и "дилатация"
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(erodeWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(dilateWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, img);
    imshow(erodeWinName, erodeImg);
    imshow(dilateWinName, dilateImg);
    waitKey();

    // закрытие окон
    destroyAllWindows();
	// освобождение ресурсов
    img.release();
    erodeImg.release();
    dilateImg.release();
	return 0;
}
