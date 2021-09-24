#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_Laplacian.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image",
               *laplacianWinName = "Laplacian";
    Mat img, grayImg, laplacianImg, laplacianImgAbs;
    int ddepth = CV_16S;
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
    // ���������� ��������� �������
    Laplacian(grayImg, laplacianImg, ddepth);
    convertScaleAbs(laplacianImg, laplacianImgAbs);
    
    // ����������� ����������
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(laplacianWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, img);
    imshow(laplacianWinName, laplacianImgAbs);
    waitKey();

     // �������� ����
    destroyAllWindows();
    // ������������� ������
    img.release();
    grayImg.release();
    laplacianImg.release();
    laplacianImgAbs.release();
    return 0;
}
