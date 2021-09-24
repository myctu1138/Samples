#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_filter2D.exe <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    // ��������� ��� ����������� �������� ����
    const char *initialWinName = "Initial Image", *resultWinName = "Filter2D";    
    // ��������� ��� �������� ���� �������
    const float kernelData[] = {-0.1f, 0.2f, -0.1f,
                                 0.2f, 3.0f,  0.2f,
                                -0.1f, 0.2f, -0.1f};
    const Mat kernel(3, 3, CV_32FC1, (float *)kernelData);

    // ������� ��� �������� ��������� � ��������������� �����������
	Mat src, dst;
    
    // �������� ���������� ��������� ������
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }
    
    // �������� �����������
	src = imread(argv[1], 1); 
    
    // ���������� �������
    filter2D(src, dst, -1, kernel);
	
    // ����������� ��������� ����������� � ���������� ���������� �������
	namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
	imshow(initialWinName, src);
    namedWindow(resultWinName, CV_WINDOW_AUTOSIZE);
    imshow(resultWinName, dst);
	waitKey();
    
    // �������� ����
    destroyAllWindows();
    // ������������ ��������
    src.release();
    dst.release();
	return 0;
}
