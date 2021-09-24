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

Mat initialImg;

void callbackButton(int state, void *data)
{
	// обрабатывать только случай, когда state=1
	if (state == 0) return;

	// получить идентификатор операции
	int operationIdx = *(int *)data;
	printf("%d\n", operationIdx);

	// применить операцию
	applyOperation(initialImg, operationIdx);
}

int main()
{
	int i, operationIdx[kMenuTabs];
	char initialImageName[maxFileNameLen];
	char key = -1;
	
	// получить название рабочего изображения
	printf("Input image name: ");
	scanf("%s", initialImageName);
	initialImg = imread(initialImageName);
	
	// создать окно для отображения исходного изображения 
	// и установки его свойств
	namedWindow(winNames[0], CV_WINDOW_AUTOSIZE);
	for (i = 1; i < kMenuTabs; i++)
	{
		operationIdx[i] = i;
		createButton(menu[i], callbackButton, &operationIdx[i], CV_RADIOBOX);
	}
	imshow(winNames[0], initialImg);
	key = waitKey();
	
	// цикл применения различных операций
	while (key != escCode)
	{
		initialImg.release();

		printf("Input image name: ");
		scanf("%s", initialImageName);
		initialImg = imread(initialImageName);

		imshow(winNames[0], initialImg);

		key = waitKey();
	}

	// закрыть все окна
	destroyAllWindows();

	// освободить память из-под изображений
	initialImg.release();
    return 0;
}
