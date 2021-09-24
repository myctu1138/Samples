#include <stdio.h>
#include <map>
#include <assert.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "drawingFunctions.h"

using namespace cv;
using std::map;


int main(int argc, char** argv)
{
    //  ��������� ��������� ������:
    //  <��� ����� � �������> <���������� ���������>
    assert(argc == 3);
    //  ������ ������ �� �����
    const char * dataFileName = argv[1];
    FileStorage fs;
    fs.open(dataFileName, FileStorage::READ);
    assert(fs.isOpened());
    Mat points;
    fs["points"] >> points;
    assert(points.empty() == false);
    
    int K = atoi(argv[2]);
    assert(K > 0);
    
    Mat labels;
    Mat centers;

    /* =================================================================== */
    //  �������� ���, ����������� ������������� ������� ������� �������,
    //  ��������� ������� kmeans. � ������� labels ��������� ������
    //  ���������, � ������� ���� �������� ����� �������. � �������
    //  centers -- ���������� �������, ��������� ���������.
    /* ------------------------------------------------------------------- */












    /* =================================================================== */
    
    // ����������� ������� ��������� � BGR-�����
    map<int, Scalar> clusterColors;
    clusterColors[0] = Scalar(255, 191, 0);
    clusterColors[1] = Scalar(0, 215, 255);
    clusterColors[2] = Scalar(71, 99, 255);
    clusterColors[3] = Scalar(0, 252, 124);
    clusterColors[4] = Scalar(240, 32, 160);

    // ����������� ������������� ����� �� ���������
    // � ������� ���������
    Mat img(500, 500, CV_8UC3, Scalar(255, 255, 255));
    drawPoints(img, points, labels, getRanges(points), clusterColors, 0);
    drawPoints(img, centers, labels, getRanges(points), clusterColors, 2);
    namedWindow("clusters");
    imshow("clusters", img);
    waitKey();
    destroyAllWindows();

    return 0;
}