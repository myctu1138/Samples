//OpenCV
#include "cxcore.h"
#include "highgui.h"

//IO and time
#include "stdio.h"
#include "windows.h"
#include <conio.h>

#include <complex>
#include <vector>

#include "fft.h"

using namespace std; 

#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"highgui.lib")

#define _WAIT_TIME 30

char helpString[] = 
"\nUsage: \n\
\tfilter.exe <input video file> [options]\n\n\
Options:\n\
\t-s \
\t silent mode.\n\
\
\t-gl (default) \
\t Gauss lowpass filter.\n\
\
\t-gh \
\t Gauss highpass filter.\n\
\
\t-bl \
\t Battervort lowpass filter.\n\
\
\t-bh \
\t Battervort highpass filter.\n\
\n\
Example:\n\
\tfilter.exe video.avi -s -gh\
";

enum FilterType {GaussLow=0, GaussHigh=1, BattervortLow=2, BattervortHigh=3};
char filters[4][50] = {"Gauss lowpass filter", "Gauss highpass filter", "Battervort lowpass filter", "Battervort highpass filter"};
IplImage* inpFrame = NULL, *outFrame = NULL, *filter = NULL, *freq = NULL, *empty = NULL;
int chW = 0, chC = 0;
int frame=0, frames;
int r = 30;
bool pause = false;
HANDLE threadInitMutex;
FilterType fType = GaussLow;

void ProcessFrame(IplImage *frame, IplImage *outFrame, IplImage* filter, IplImage* freq, FilterType t, int *_r)
{
  int w = frame->width,
      h = frame->height,
      r = *_r,
      size = w * h;

  vector<double> f(size);
  vector<complex<double>> inpR, inpG, inpB,
                          outR(size), outG(size), outB(size);

  double maxR = 0.0 , maxG = 0.0, maxB = 0.0;
  double minR = 0.0 , minG = 0.0, minB = 0.0;

  //Fill inp<R|G|B> arrays
  for (int i=0; i<size; i++)
    inpB.push_back(pow(-1.0,i/w+i%w) * ((unsigned char*)frame->imageData)[i*3 + 0]);

  for (int i=0; i<size; i++)
    inpG.push_back(pow(-1.0,i/w+i%w) * ((unsigned char*)frame->imageData)[i*3 + 1]);

  for (int i=0; i<size; i++)
    inpR.push_back(pow(-1.0,i/w+i%w) * ((unsigned char*)frame->imageData)[i*3 + 2]);

  //Make filter
  for (int i=0; i<w; i++)
    for (int j=0;j<h; j++)
      switch(t)
      {
        case GaussLow:
          f[i+j*w]=exp( -(pow(double(i-w/2), 2) + pow(double(j-h/2), 2) ) / (2 * pow((double)r, 2) ) );//Gauss lowpass filter
          break;
        case GaussHigh:
          f[i+j*w]=1 - exp( -(pow(double(i-w/2), 2) + pow(double(j-h/2), 2) ) / (2 * pow((double)r, 2) ) );//Gauss highpass filter
          break;
        case BattervortLow:
          f[i+j*w]=1 / ( 1 + (pow(double(i-w/2), 2) + pow(double(j-h/2), 2) ) / pow((double)r, 2) );//Battervort lowpass filter
          break;
        case BattervortHigh:
          f[i+j*w]=1 / ( 1 + pow((double)r, 2) / (pow(double(i-w/2), 2) + pow(double(j-h/2), 2) ) );//Battervort highpass filter
          break;
      };

  //Convert to frequency area
  SerialFFT2D(inpR, outR, w, h);
  SerialFFT2D(inpG, outG, w, h);
  SerialFFT2D(inpB, outB, w, h);

  //Show filter
  for(int i=0; i<size; i++)
    filter->imageData[i] = (char) (255 * f[i]);

  //Apply filter
  for (int i=0; i<size; i++)
    outR[i] = outR[i] * f[i];

  for (int i=0; i<size; i++)
    outG[i] = outG[i] * f[i];

  for (int i=0; i<size; i++)
    outB[i] = outB[i] * f[i];

  //Find max value

  for (int i=0; i<size; i++)
    if (sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) != 0)
    {
      maxR = minR = log( sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()));
      break;
    }

  for (int i=0; i<size; i++)
  {
    if (sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) != 0)
      if(maxR < log( sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) ) )
        maxR = log( sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) );

    if (sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) != 0)
      if(minR > log( sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) ) )
        minR = log( sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) );
  }

  for (int i=0; i<size; i++)
    if (sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) != 0)
    {
      maxG = minG = log( sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) );
      break;
    }

  for (int i=0; i<size; i++)
  {
    if (sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) != 0)
      if(maxG < log( sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) ) ) 
        maxG = log( sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) ) ;

    if (sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) != 0)
      if(minG > log( sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) ) ) 
        minG = log( sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) ) ;
  }

  for (int i=0; i<size; i++)
    if (sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) != 0)
    {
      maxB = minB = log( sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()));
      break;
    }

  for (int i=0; i<size; i++)
  {
    if (sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) != 0)
      if(maxB < log( sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) ) ) 
        maxB = log( sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) );

    if (sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) != 0)
      if(minB > log( sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) ) ) 
        minB = log( sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) );
  }

  //Show frequency area with filter
  for (int i=0; i<size; i++)
    if (sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag()) != 0)
      freq->imageData[3*i+0] = (char) (255 * ( (log( sqrt(outR[i].real()*outR[i].real()+outR[i].imag()*outR[i].imag())) - minR ) / (maxR - minR) ) );
    else
      freq->imageData[3*i+0] = 0;

  for (int i=0; i<size; i++)
    if (sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag()) != 0)
      freq->imageData[3*i+1] = (char) (255 * ( (log( sqrt(outG[i].real()*outG[i].real()+outG[i].imag()*outG[i].imag())) - minG ) / (maxG - minG) ) );
    else
      freq->imageData[3*i+1] = 0;

  for (int i=0; i<size; i++)
    if (sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag()) != 0)
      freq->imageData[3*i+2] = (char) (255 * ( (log( sqrt(outB[i].real()*outB[i].real()+outB[i].imag()*outB[i].imag())) - minB ) / (maxB - minB) ) );
    else
      freq->imageData[3*i+2] = 0;

  //Convert to frequency area
  SerialInverseFFT2D(outR, inpR, w, h);
  SerialInverseFFT2D(outG, inpG, w, h);
  SerialInverseFFT2D(outB, inpB, w, h);

  for (int i=0; i<size; i++)
    outFrame->imageData[3*i+0] = (char) (pow(-1.0,i/w+i%w) * inpB[i].real());

  for (int i=0; i<size; i++)
    outFrame->imageData[3*i+1] = (char) (pow(-1.0,i/w+i%w) * inpG[i].real());

  for (int i=0; i<size; i++)
    outFrame->imageData[3*i+2] = (char) (pow(-1.0,i/w+i%w) * inpR[i].real());
}

void OnWindowClick(int ev, int x, int y, int flags, void* param)
{
  if(ev == CV_EVENT_LBUTTONDOWN)
  {
    if(*(bool*)param)
      printf("Resume!\n");
    else
      printf("Pause!!!\n");
      
    *(bool*)param = !(*(bool*)param);
  }
}

DWORD WINAPI DrawingFunction( LPVOID lpParam ) 
{
  cvNamedWindow("Output",CV_WINDOW_AUTOSIZE);
  cvShowImage("Output", empty);
  cvCreateTrackbar( "Frame", "Output", &frame, frames, NULL );
  cvMoveWindow("Output", 50, 200);

  char fStr[100];
  sprintf(fStr, "Filter: %s", filters[fType]);
  cvNamedWindow(fStr,CV_WINDOW_AUTOSIZE);
  cvShowImage(fStr, empty);
  cvCreateTrackbar( "Radius", fStr, &r, cvGetSize(empty).width, 0 );
  cvMoveWindow(fStr, 400, 200);

  cvNamedWindow("Frequency area",CV_WINDOW_AUTOSIZE);
  cvMoveWindow("Frequency area", 750, 200);

  cvSetMouseCallback("Output", OnWindowClick, &pause);
  cvSetMouseCallback(fStr, OnWindowClick, &pause);
  cvSetMouseCallback("Frequency area", OnWindowClick, &pause);

  ReleaseSemaphore(threadInitMutex, 1, NULL);

  while(frame != frames)
  {
    cvShowImage("Output", outFrame);
    cvShowImage(fStr, filter);
	cvShowImage("Frequency area", freq);
    chW = cvWaitKey(_WAIT_TIME);
		
    if( chW == 27 || chC == 27)  //Esc
      break;
  }

  cvDestroyWindow("Frequency area");
  cvDestroyWindow("Output");

  return 0;
}

int main(int argc, char** argv)
{
  const char* fileName = argc>=2 ? argv[1] : "Videos\\TestVideo1.avi";
  CvCapture* film = NULL;
  int w, h;
  int msec;
  LARGE_INTEGER start, finish, totStart;
  LARGE_INTEGER fr;
  double wrkTime = 0.0;
  CvSize size;
  HANDLE threadH;
  DWORD threadId;
  char codec[5];
  bool silent = false;
  FILE *f;
  int totFrames = 0;

  //1. Parse each string in "argv" array
  for(int i=2; i < argc; i++)
  {
    //Silent mode
    if( string(argv[i]) == "-s" )
    {
      silent = true;
      continue;
    }

    //Gauss lowpass filter
    if( string(argv[i]) == "-gl" )
    {
      fType = GaussLow;
      continue;
    }

    //Gauss highpass filter
    if( string(argv[i]) == "-gh" )
    {
      fType = GaussHigh;
      continue;
    }

    //Battervort lowpass filter
    if( string(argv[i]) == "-bl" )
    {
      fType = BattervortLow;
      continue;
    }

    //Battervort highpass filter
    if( string(argv[i]) == "-bh" )
    {
      fType = BattervortHigh;
      continue;
    }

    printf("%s\n", helpString);
    exit(1);
  }

  //2.1. Video file is present?
  printf("Video file: %s\n", fileName);
  f = fopen(fileName,"r");
  if (f == NULL)
  {
    printf("Error: File not found!\n");
    printf("%s\n", helpString);
    return -1;
  } 


  //2.2. Load video file
  film=cvCreateFileCapture(fileName);
  if(!film)
  {
    printf("Error: Unsupported video codec!\n");
    printf("%s\n", helpString);
    return -1;
  } 

  QueryPerformanceFrequency(&fr);

  //3. Get video info
  frames = cvGetCaptureProperty(film, CV_CAP_PROP_FRAME_COUNT);
  int val = (int) cvGetCaptureProperty(film, CV_CAP_PROP_FOURCC);
  memcpy(codec, &val, 4);
  codec[4]=0;

  w = (int) cvGetCaptureProperty(film, CV_CAP_PROP_FRAME_WIDTH);
  h = (int) cvGetCaptureProperty(film, CV_CAP_PROP_FRAME_HEIGHT);
  size.height = h;
  size.width = w;

  //4.1. Check up height and width for correct value
  int bits = 0;
  for( int tmp = h; tmp > 1; tmp/=2, bits++);
  if( h != 1<<bits )
  {
    printf("Error: Height should be power of 2!\n");
    cvReleaseCapture(&film);
    return -1;
  }

  bits = 0;
  for( int tmp = w; tmp > 1; tmp/=2, bits++);
  if( w != 1<<bits )
  {
    printf("Error: Width should be power of 2!\n");
    cvReleaseCapture(&film);
    return -1;
  }

  //4.2. Create mutex for thread init
  threadInitMutex = CreateSemaphore(NULL, 0, 1, NULL);
  if(threadInitMutex==NULL)
  {
    printf("Error: CreateMutex error: %d\n", GetLastError());
    return -1;
  }

  //5. Print info
  printf("Loaded!\n");
  printf("Codec: \t%s\n", codec);
  printf("Total frames: %i\n", frames);
  printf("Picture size: %i x %i\n", w, h);
  printf("Filter: %s\n", filters[fType]);
  printf("\n");
  printf("Click on window to \"Pause\"\n");
  printf("Press ESC to \"Exit\"\n");
  printf("\n");

  //6. Create images
  empty = cvCreateImage(size, IPL_DEPTH_8U, 1);
  outFrame = cvCreateImage(size, IPL_DEPTH_8U, 3);
  filter = cvCreateImage(size, IPL_DEPTH_8U, 1);
  freq = cvCreateImage(size, IPL_DEPTH_8U, 3);
      
  //7. Create and wait thread for drawing
  if (!silent)
  {
    threadH = CreateThread(NULL, 0, DrawingFunction, NULL, 0, &threadId); 
    WaitForSingleObject(threadInitMutex, INFINITE);
  }

  printf("Log:\n");

  //8. Variable of start time is installed
  QueryPerformanceCounter(&totStart);

  //9. Process frames
  while(frame != frames)
  {
    //9.1. Get next frame
    cvSetCaptureProperty( film, CV_CAP_PROP_POS_FRAMES, frame);
    msec = (int) cvGetCaptureProperty( film, CV_CAP_PROP_POS_MSEC);
    inpFrame = cvQueryFrame(film);

    //9.2. Process current frames
    QueryPerformanceCounter(&start);
    ProcessFrame(inpFrame, outFrame, filter, freq, fType, &r);
    QueryPerformanceCounter(&finish);

    //9.3. Increase count of total frames
    totFrames++;

    //9.4. Compute and print time
    printf("Frame: %5i, position: %6i ms, processing time: %f \n", frame, msec, (start.QuadPart - finish.QuadPart) / (double)fr.QuadPart);
    wrkTime += (finish.QuadPart-start.QuadPart) / (double)fr.QuadPart;
		
    //9.5. "ESC" is pressed?
    if(_kbhit())
      chC = _getch();

    //9.6. The thread is completed?
    if( WaitForSingleObject(threadH, 1) == WAIT_OBJECT_0) 
      break;

  	//9.7. Break if ESC pressed
    if (silent) 
      if( chC == 27 )  //Esc
        break;

    //9.8. Compute next frame and move trackbar
    if(!pause)
    {
      frame++;
      cvSetTrackbarPos( "Frame", "Output", frame);
    }
  }

  QueryPerformanceCounter(&finish);

  //10. Print summary
  printf("Done!\n");
  printf("\n");
  printf("Total time: %f\n", (finish.QuadPart-totStart.QuadPart) / (double)fr.QuadPart);
  printf("Total frames: %i\n", totFrames);
  printf("Total processing time: %f\n", wrkTime);
  if(totFrames)
    printf("Average processing time per frame: %f\n", wrkTime/totFrames);

  //11. Release resources
  if (!silent)
    CloseHandle(threadH);


  cvReleaseImage(&freq);
  cvReleaseImage(&outFrame);
  cvReleaseImage(&empty);

  CloseHandle(threadInitMutex);

  return 0;
}