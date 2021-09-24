#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.h"

using namespace cv;

const int kMenuTabs = 12;
const char* menu[] = 
{
    "0 - Read image", 
    "1 - Apply linear filter", 
    "2 - Apply blur(...)",
    "3 - Apply medianBlur(...)",
    "4 - Apply GaussianBlur(...)",
    "5 - Apply erode(...)",
    "6 - Apply dilate(...)",
    "7 - Apply Sobel(...)",
    "8 - Apply Laplacian(...)",
    "9 - Apply Canny(...)",
    "10 - Apply calcHist(...)",
    "11 - Apply equalizeHist(...)"
};
const char* winNames[] = 
{
    "Initial image", 
    "filter2d", 
    "blur",
    "medianBlur",
    "GaussianBlur",
    "erode",
    "dilate",
    "Sobel",
    "Laplacian",
    "Canny",
    "calcHist",
    "equalizeHist"
};
const int maxFileNameLen = 1000;
const int escCode = 27;

void printMenu();
void chooseMenuTab(int &activeMenuTab, Mat &srcImg);
void loadImage(Mat &srcImg);

int main(int argc, char** argv)
{
    Mat srcImg; // исходное изображение
    char ans;
    int activeMenuTab = -1;
    do
    {
        // choose menu item
        chooseMenuTab(activeMenuTab, srcImg);
        // apply operation
        applyOperation(srcImg, activeMenuTab);
        // ask a question
        printf("Do you want to continue? ESC - exit\n");
        // waiting for key will be pressed
        ans = waitKey();
    }
    while (ans != escCode);
    destroyAllWindows(); // destroy all windows
    srcImg.release(); // release memory allocated for storing image
    return 0;
}

void printMenu()
{
    int i = 0;
    printf("Menu items:\n");
    for (i; i < kMenuTabs; i++)
    {
        printf("\t%s\n", menu[i]);
    }
    printf("\n");
}

void loadImage(Mat &srcImg)
{
    char fileName[maxFileNameLen];
    do 
    {
        printf("Input full file name: ");
        scanf("%s", &fileName);
        srcImg = imread(fileName, 1);
    }
    while (srcImg.data == 0);    
    printf("The image was succesfully read\n\n");
}

void chooseMenuTab(int &activeMenuTab, Mat &srcImg)
{
    int tabIdx;
    while (true)
    {
        // print menu items
        printMenu();
        // get menu item identifier to apply operation
        printf("Input item identifier to apply operation: ");
        scanf("%d", &tabIdx);
        if (tabIdx == 0)
        {
            // read image
            loadImage(srcImg);            
        }
        else if (tabIdx >=1 && tabIdx < kMenuTabs && srcImg.data == 0)
        {
            // read image
            printf("The image should be read to apply operation!\n");
            loadImage(srcImg);
        }
        else if (tabIdx >=1 && tabIdx < kMenuTabs)
        {
            activeMenuTab = tabIdx;
            break;
        }
    }
}
