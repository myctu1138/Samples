#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_equalizeHist.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image",
               *equalizedWinName = "Equalized Image";
    Mat img, grayImg, equalizedImg;
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }    
    // загрузка изображения
    img = imread(argv[1], 1);
    // преобразование в оттенки серого
    cvtColor(img, grayImg, CV_RGB2GRAY);
    // выравнивание гистограммы
    equalizeHist(grayImg, equalizedImg);

    // отображение исходного изображения и гистограмм
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(equalizedWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, grayImg);
    imshow(equalizedWinName, equalizedImg);
    waitKey();
    
    // закрытие окон
    destroyAllWindows();
    // осовобождение памяти
    img.release();
    grayImg.release();
    equalizedImg.release();
    return 0;
}
