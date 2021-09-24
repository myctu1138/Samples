#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

const char helper[] = 
    "Sample_calcHist.exe  <img_file>\n\
    \t<img_file> - image file name\n";

int main(int argc, char* argv[])
{
    const char *initialWinName = "Initial Image",
               *histWinName = "Histogram";
    Mat img, bgrChannels[3], bHist, gHist, rHist, histImg;
    int kBins = 256; // ���������� ����� �����������
    float range[] = {0.0f, 256.0f}; // �������� ��������� �������� �����
    const float* histRange = { range };
    bool uniform = true; // ����������� ������������� ��������� �� �����
    bool accumulate = false; // ������ �������� ����� ����������� �����������
    int histWidth = 512, histHeight = 400; // ������� ��� ����������� �����������
    int binWidth = cvRound((double)histWidth / kBins); // ���������� �������� �� ���
    int i, kChannels = 3;
    Scalar colors[] = {Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255)};
    if (argc < 2)
    {
        printf("%s", helper);
        return 1;
    }    
    // �������� �����������
    img = imread(argv[1], 1);
    // ��������� ������� �����������
    split(img, bgrChannels);
    // ���������� ����������� ��� ������� ������
    calcHist(&bgrChannels[0], 1, 0, Mat(), bHist, 1, &kBins, 
        &histRange, uniform, accumulate);
    calcHist(&bgrChannels[1], 1, 0, Mat(), gHist, 1, &kBins, 
        &histRange, uniform, accumulate);
    calcHist(&bgrChannels[2], 1, 0, Mat(), rHist, 1, &kBins, 
        &histRange, uniform, accumulate);

    // ���������� �����������
    histImg = Mat(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));
    // ������������ ���������� � ������������ � ��������� ���� ��� �����������
    normalize(bHist, bHist, 0, histImg.rows, 
        NORM_MINMAX, -1, Mat());
    normalize(gHist, gHist, 0, histImg.rows, 
        NORM_MINMAX, -1, Mat());
    normalize(rHist, rHist, 0, histImg.rows, 
        NORM_MINMAX, -1, Mat());
    
    for (i = 1; i < kBins; i++)
    {
        line(histImg, Point(binWidth * (i-1), histHeight - cvRound(bHist.at<float>(i-1))) ,
            Point(binWidth * i, histHeight - cvRound(bHist.at<float>(i)) ),
            colors[0], 2, 8, 0);
        line(histImg, Point(binWidth * (i-1), histHeight - cvRound(gHist.at<float>(i-1))) ,
            Point(binWidth * i, histHeight - cvRound(gHist.at<float>(i)) ),
            colors[1], 2, 8, 0);
        line(histImg, Point(binWidth * (i-1), histHeight - cvRound(rHist.at<float>(i-1))) ,
            Point(binWidth * i, histHeight - cvRound(rHist.at<float>(i)) ),
            colors[2], 2, 8, 0);
    }
    // ����������� ��������� ����������� � ����������
    namedWindow(initialWinName, CV_WINDOW_AUTOSIZE);
    namedWindow(histWinName, CV_WINDOW_AUTOSIZE);
    imshow(initialWinName, img);
    imshow(histWinName, histImg);
    waitKey();
    
    // �������� ����
    destroyAllWindows();
    // ������������� ������
    img.release();
    for (i = 0; i < kChannels; i++)
    {
        bgrChannels[i].release();
    }
    bHist.release();
    gHist.release();
    rHist.release();
    histImg.release();
    return 0;
}
