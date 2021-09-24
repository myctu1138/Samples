#include <opencv2/opencv.hpp>
#define DELAY 30
#define ESC_KEY 27

using namespace cv;

const char* helper = 
	"02_FaceDetection.exe <model_file> [<video>]\n\
	\t<model_file> - model file name\n\
	\t<video> - video file name (video stream will be taken from \n\
				web-camera by default)\n\
	";

int main(int argc, char *argv[])
{
	const char* winName = "video";
	char *modelFileName = 0, *videoFileName = 0;
	int i, width, height;
	char key = -1;
	Mat image, gray;
	VideoCapture capture;
	vector<Rect> objects;

	if (argc < 2)
	{
		printf("%s", helper);
		return 1;
	}
	modelFileName = argv[1];
	if (argc > 2)
	{
		videoFileName = argv[2];
	}

	// создание классификатора и загрузка модели
	CascadeClassifier cascade;
	cascade.load(modelFileName);
	
	// загрузка видеофайла или перехват видеопотока
	if (videoFileName == 0)
	{
		capture.open(0);
	}
	else
	{
		capture.open(videoFileName);
	}
	if (!capture.isOpened())
	{
		printf("Incorrect capture name.\n");
		return 1;
	}

	// создание окна для отображения видео
	namedWindow(winName);

	// получение кадра видеопотока
	capture >> image;
	while (image.data != 0 && key != ESC_KEY)
	{
		cvtColor(image, gray, CV_BGR2GRAY);
		cascade.detectMultiScale(gray, objects);
		for (i = 0; i < objects.size(); i++)
		{
			rectangle(image, Point(objects[i].x, objects[i].y),
				Point(objects[i].x + objects[i].width, objects[i].y + objects[i].height),
				CV_RGB(255, 0, 0), 2);
		}
		imshow(winName, image);
		key = waitKey(DELAY);
		capture >> image;
		gray.release();
		objects.clear();
	}
	capture.release();
	return 0;
}
