#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include <stdio.h>
#include <time.h>

void Mirror(uchar *pSrc, uchar *pDst, int height, int width, int channel,bool direction)
{
	int offset, offset_mirror;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				offset = i*width*channel + j*channel + k;

				if (direction)
					offset_mirror = (i + 1)*width*channel - (j + 1)*channel + k;     // ���k�蹳
				else
					offset_mirror = (height - i - 1)*width*channel + j*channel + k;  // �W�U�蹳

				pDst[offset] = pSrc[offset_mirror];
			}
		}
	}
}

int main()
{
	/// �إ�IPL
	IplImage *imgSrc = cvLoadImage("C:\\Users\\user\\Desktop\\Mirror.jpg");
	IplImage *imgDst = cvCreateImage(cvGetSize(imgSrc), IPL_DEPTH_8U, 3);

	/// IPL����
	uchar *ptrSrc = (uchar*)imgSrc->imageData;
	uchar *ptrDst = (uchar*)imgDst->imageData;

	/// IPL�ؤo
	int height = imgSrc->height;
	int width = imgSrc->width;
	int channel = imgSrc->nChannels;

	clock_t timeStart = clock();
	/*===========================START===========================*/
	for (int i = 0; i < 100; i++)
	{
		Mirror(ptrSrc, ptrDst, height, width, channel, true);
	}
	/*============================END============================*/
	clock_t timeEnd = clock();

	/// �p��B��X�ɶ�
	float milliseconds = timeEnd - timeStart;
	printf("CPU Processing time: %lf (s) \n", milliseconds / CLOCKS_PER_SEC);

	/// Show image
	cvNamedWindow("", CV_WINDOW_NORMAL);
	cvShowImage("", imgDst);
	cvWaitKey(0);
	cvReleaseImage(&imgDst);

	return 0;
}