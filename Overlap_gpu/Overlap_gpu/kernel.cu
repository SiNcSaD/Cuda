#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include <stdio.h>
/*
__global__ void Overlap(uchar *src1, uchar *src2, uchar *dst)
{
	int tid = blockIdx.x*blockDim.x*3 + threadIdx.x*3;
	for (int i = 0; i < 3; i++)
	{
		dst[tid + i] = (uchar)(0.5*(src1[tid + i] + src2[tid + i]));
	}
}*/

__global__ void Overlap(uchar *src1, uchar *src2, uchar *dst)
{
	int tid = blockIdx.x*blockDim.x + threadIdx.x;
	dst[tid] = 0.5*(src1[tid] + src2[tid]);
}

int main()
{
	/// 建立IPL
	IplImage *imgSrc1 = cvLoadImage("C:\\Users\\user\\Desktop\\overlap01.jpg");
	IplImage *imgSrc2 = cvLoadImage("C:\\Users\\user\\Desktop\\overlap02.jpg");
	IplImage *imgDst = cvCreateImage(cvGetSize(imgSrc1), IPL_DEPTH_8U, 3);

	/// IPL指標
	uchar *ptrSrc1 = (uchar*)imgSrc1->imageData;
	uchar *ptrSrc2 = (uchar*)imgSrc2->imageData;
	uchar *ptrDst = (uchar*)imgDst->imageData;

	/// IPL尺寸
	int height = imgSrc1->height;
	int width = imgSrc1->width;
	int channel = imgSrc1->nChannels;

	/// 分配GPU記憶體
	uchar *dev_Src1, *dev_Src2, *dev_Dst;
	cudaMalloc((void**)&dev_Src1, height*width*channel*sizeof(uchar));
	cudaMalloc((void**)&dev_Src2, height*width*channel*sizeof(uchar));
	cudaMalloc((void**)&dev_Dst, height*width*channel*sizeof(uchar));
	cudaMemcpy(dev_Src1, ptrSrc1, height*width*channel*sizeof(uchar), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_Src2, ptrSrc2, height*width*channel*sizeof(uchar), cudaMemcpyHostToDevice);

	/// 計時
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	//if (height > 1024) return 0;    //預防 thread 超過 GPU 的 Max threads per block.
	int N = height*width*channel;

	cudaEventRecord(start);
	/*===========================START===========================*/	
	//Overlap << <width, height >> >(dev_Src1, dev_Src2, dev_Dst);
	Overlap << <(N + 1023) / 1024, 1024 >> >(dev_Src1, dev_Src2, dev_Dst);
	/*============================END============================*/
	cudaEventRecord(stop);

	/// 計算、輸出時間
	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("GPU Processing time: %f (ms) \n", milliseconds);

	cudaMemcpy(ptrDst, dev_Dst, height*width*channel*sizeof(uchar), cudaMemcpyDeviceToHost);
	
	/// 釋放GPU記憶體
	cudaFree(dev_Src1);
	cudaFree(dev_Src2);
	cudaFree(dev_Dst);

	/// Show image
	cvNamedWindow("", CV_WINDOW_NORMAL);
	cvShowImage("", imgDst);
	cvWaitKey(0);
	cvReleaseImage(&imgDst);

	return 0;
}