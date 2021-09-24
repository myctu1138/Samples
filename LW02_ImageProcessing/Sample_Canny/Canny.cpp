#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_Canny.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *cannyWinName = "Canny detector";
    Mat img, grayImg, edgesImg;
    double lowThreshold = 70, uppThreshold = 260;
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }    
    // �������� �����������
    img = imread(argv[1], 1);
    // �������� �����
    blur(img, img, Size(3,3));
    // �������������� � ������� ������
    cvtColor(img, grayImg, CV_RGB2GRAY);
    // ���������� ��������� �������
    Canny(grayImg, edgesImg, lowThreshold, uppThreshold);
    
    // ����������� ����������
    namedWindow(cannyWinName, CV_WINDOW_AUTOSIZE);
    imshow(cannyWinName, edgesImg);
    waitKey();

    // �������� ����
    destroyAllWindows();
    // ������������� ������
    img.release();
    grayImg.release();
    edgesImg.release();
    return 0;
}
