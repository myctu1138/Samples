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
    // �������� �����������
    img = imread(argv[1], 1);
    // �������������� � ������� ������
    cvtColor(img, grayImg, CV_RGB2GRAY);
    // ������������ �����������
    equalizeHist(grayImg, equalizedImg);

    // ����������� ��������� ����������� � ����������
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(equalizedWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, grayImg);
    imshow(equalizedWinName, equalizedImg);
    waitKey();
    
    // �������� ����
    destroyAllWindows();
    // ������������� ������
    img.release();
    grayImg.release();
    equalizedImg.release();
    return 0;
}
