#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_Sobel.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image", 
               *xGradWinName = "Gradient in the direction Ox",
               *yGradWinName = "Gradient in the direction Oy",
               *gradWinName = "Gradient";
    int ddepth = CV_16S;
    double alpha = 0.5, beta = 0.5;
    Mat img, grayImg, xGrad, yGrad, xGradAbs, yGradAbs, grad;
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }    
    // �������� �����������
    img = imread(argv[1], 1);
    // �������� ����� � ������� ������� ������
    GaussianBlur(img, img, Size(3,3), 0, 0, BORDER_DEFAULT);
    // �������������� � ������� ������
    cvtColor(img, grayImg, CV_RGB2GRAY);
    // ���������� ����������� �� ���� ������������
    Sobel(grayImg, xGrad, ddepth, 1, 0); // �� Ox
    Sobel(grayImg, yGrad, ddepth, 0, 1); // �� Oy
    // �������������� ���������� � 8-������ �����������
    convertScaleAbs(xGrad, xGradAbs);
    convertScaleAbs(yGrad, yGradAbs);
    // ������������ ���������� ���������� ����� ���� ��������
    addWeighted(xGradAbs, alpha, yGradAbs, beta, 0, grad);

    // ����������� ����������
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(xGradWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(yGradWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(gradWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, img);
    imshow(xGradWinName, xGradAbs);
    imshow(yGradWinName, yGradAbs);
    imshow(gradWinName, grad);
    waitKey();

     // �������� ����
    destroyAllWindows();
    // ������������� ������
    img.release();
    grayImg.release();
    xGrad.release();
    yGrad.release();
    xGradAbs.release();
    yGradAbs.release();
    return 0;
}
