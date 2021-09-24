#include "functions.h"

int applyBlur(const Mat &src, Mat &dst)
{
    int imgHeight, imgWidth;
    Size kernelSize(3, 3);
    imgHeight = src.size().height;
    imgWidth = src.size().width;
    do
    {
        printf("Set kernel size: \n");
        printf("\theight: ");
        scanf("%d", &kernelSize.height);
        printf("\twidth: ");
        scanf("%d", &kernelSize.width);
    }
    while (kernelSize.width < 3 && kernelSize.width > imgWidth &&
           kernelSize.height < 3 && kernelSize.height > imgHeight);
    // TODO: input another blur(...) parameters
    blur(src, dst, kernelSize);
    return 0;
}

int applyMedianBlur(const Mat &src, Mat &dst)
{
    int kSize = 3, minDim = -1;
    minDim = min(src.size().height, src.size().width);
    do
    {
        printf("Set kernel size (odd, > 3, < min(img.width, img.height)): ");
        scanf("%d", &kSize);
    }
    while (kSize < 3 && kSize > minDim && kSize %2 == 0);
    medianBlur(src, dst, kSize);
    return 0;
}

int applyOperation(const Mat &src, const int operationIdx)
{
    char key = -1;
    Mat dst;
    switch (operationIdx)
    {
    case 1:
        {
            // "1 - Apply linear filter"
			dst = src.clone();
            break;
        }
    case 2:
        {
            // "2 - Apply blur(...)"
            applyBlur(src, dst);
            break;
        }
    case 3:
        {
            // "3 - Apply medianBlur(...)"
            applyMedianBlur(src, dst);
            break;
        }
    case 4:
        {
            // "4 - Apply GaussianBlur(...)"
            break;
        }
    case 5:
        {
            // "5 - Apply erode(...)"
            break;
        }
    case 6:
        {
            // "6 - Apply dilate(...)"
            break;
        }
    case 7:
        {
            // "7 - Apply Sobel(...)"
            break;
        }
    case 8:
        {
            // "8 - Apply Laplacian(...)"
            break;
        }
    case 9:
        {
            // "9 - Apply Canny(...)"
            break;
        }
    case 10:
        {
            // "10 - Apply calcHist(...)"
            break;
        }
    case 11:
        {
            // "11 - Apply equalizeHist(...)"
            break;
        }
    }

    // show processed image
    namedWindow(winNames[operationIdx]);
    imshow(winNames[operationIdx], dst);

    return 0;
}
