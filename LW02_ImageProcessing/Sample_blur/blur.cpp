#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_blur.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image", 
               *blurWinName = "blur";
    Mat img, blurImg;
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }
    
    // �������� �����������
    img = imread(argv[1], 1);
    // ���������� �������� ��������
    blur(img, blurImg, Size(5, 5));

    // ����������� ��������� ����������� � ���������� ��������
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(blurWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, img);
    imshow(blurWinName, blurImg);
    waitKey();

    // �������� ����
    destroyAllWindows();
	// ������������ ��������
    img.release();
    blurImg.release();
    return 0;
}
