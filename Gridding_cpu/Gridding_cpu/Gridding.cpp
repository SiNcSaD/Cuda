#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <time.h>

#define gridWidth 100
#define gridHeight 100

int main()
{
	/// 建立IPL
	IplImage *imgSrc = cvLoadImage("C:\\Users\\user\\Desktop\\Gridding.jpg");
	IplImage *imgOutput = cvCreateImage(cvGetSize(imgSrc), IPL_DEPTH_8U, 3);

	/// IPL指標
	uchar *ptrSrc = (uchar*)imgSrc->imageData;
	uchar *ptrOutput = (uchar*)imgOutput->imageData;

	/// IPL尺寸
	int height = imgSrc->height;
	int width = imgSrc->width;
	int channel = imgSrc->nChannels;

	/// 複製原圖
	for (int i = 0; i < width*height*channel; i++)
		ptrOutput[i] = ptrSrc[i];

	clock_t timeStart = clock();
	/*===========================START===========================*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channel; k++)
			{
				if (i % gridHeight == 0 || j % gridWidth == 0)
				{
					int offset = (i*width + j)*channel + k;
					ptrOutput[offset] = 255;
				}
			}
		}
	}
	/*============================END============================*/
	clock_t timeEnd = clock();

	/// 計算、輸出時間
	float milliseconds = timeEnd - timeStart;
	printf("CPU Processing time: %f (ms) \n", milliseconds);

	/// Show image
	cvNamedWindow("", CV_WINDOW_AUTOSIZE);	// 若用 CV_WINDOW_NORMAL 會誤以為網格化錯誤...
	cvShowImage("", imgOutput);
	cvWaitKey(0);
	cvReleaseImage(&imgOutput);

	return 0;
}