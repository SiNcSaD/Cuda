#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <time.h>

int main()
{
	/// 建立IPL
	IplImage *imgSrc1 = cvLoadImage("C:\\Users\\user\\Desktop\\overlap03.jpg");
	IplImage *imgSrc2 = cvLoadImage("C:\\Users\\user\\Desktop\\overlap04.jpg");
	IplImage *imgDst = cvCreateImage(cvGetSize(imgSrc1), IPL_DEPTH_8U, 3);

	/// IPL指標
	uchar *ptrSrc1 = (uchar*)imgSrc1->imageData;
	uchar *ptrSrc2 = (uchar*)imgSrc2->imageData;
	uchar *ptrDst = (uchar*)imgDst->imageData;

	/// IPL尺寸
	int height = imgSrc1->height;
	int width = imgSrc1->width;
	int channel = imgSrc1->nChannels;
	
	uchar *overlap = (uchar*)malloc(width*height*channel*sizeof(uchar));

	clock_t timeStart = clock();
	/*===========================START===========================*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channel; k++)
			{
				/// 注意i要多乘channel
				int offset = i*width*channel + j*channel + k;
				overlap[offset] = 0.5*(ptrSrc1[offset] + ptrSrc2[offset]);
			}
		}
	}
	/*============================END============================*/
	clock_t timeEnd = clock();
	
	/// 計算、輸出時間
	float milliseconds = timeEnd - timeStart;
	printf("CPU Processing time: %f (ms) \n", milliseconds);

	memcpy(ptrDst, overlap, width*height*channel*sizeof(uchar));

	/// Show image
	cvNamedWindow("", CV_WINDOW_NORMAL);
	cvShowImage("", imgDst);
	cvWaitKey(0);
	cvReleaseImage(&imgDst);
	return 0;
}