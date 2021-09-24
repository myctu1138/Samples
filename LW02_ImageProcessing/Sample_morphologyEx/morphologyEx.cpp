#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_morphologyEx.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image", 
               *morphologyOpenWinName = "MORPH_OPEN",
               *morphologyCloseWinName = "MORPH_CLOSE",
               *morphologyGradientWinName = "MORPH_GRADIENT",
               *morphologyTopHatWinName = "MORPH_TOPHAT",
               *morphologyBlackHatWinName = "MORPH_BLACKHAT";
    Mat img, morphologyOpenImg, morphologyCloseImg, morphologyGradientImg,
        morphologyTopHatImg, morphologyBlackHatImg, element;
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }
    
    // загрузка изображения
    img = imread(argv[1], 1);

    // применение морфологических операций
    element = Mat();
    morphologyEx(img, morphologyOpenImg, MORPH_OPEN, element);
    morphologyEx(img, morphologyCloseImg, MORPH_CLOSE, element);
    morphologyEx(img, morphologyGradientImg, MORPH_GRADIENT, element);
    morphologyEx(img, morphologyTopHatImg, MORPH_TOPHAT, element);
    morphologyEx(img, morphologyBlackHatImg, MORPH_BLACKHAT, element);

    // отображение исходного изображения и результата выполнения операций
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(morphologyOpenWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(morphologyCloseWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(morphologyGradientWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(morphologyTopHatWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(morphologyBlackHatWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, img);
    imshow(morphologyOpenWinName, morphologyOpenImg);
    imshow(morphologyCloseWinName, morphologyCloseImg);
    imshow(morphologyGradientWinName, morphologyGradientImg);
    imshow(morphologyTopHatWinName, morphologyTopHatImg);
    imshow(morphologyBlackHatWinName, morphologyBlackHatImg);
    waitKey();

    // закрытие окон
    destroyAllWindows();
    // осовобождение памяти
    img.release();
    morphologyOpenImg.release();
    morphologyCloseImg.release();
    morphologyGradientImg.release();
    morphologyTopHatImg.release();
    morphologyBlackHatImg.release();
    return 0;
}
