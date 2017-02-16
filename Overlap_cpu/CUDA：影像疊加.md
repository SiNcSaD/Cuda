# CUDA：影像疊加
###### tags: `CUDA` `OpenCV`
+ 將不同影像重新疊加整合。
+ 應用於地形整合如傳統地圖與衛星影像整合等等。

![](https://i.imgur.com/eLiEQOg.jpg)


兩個影像的像素權重為0.5，相加得到新的疊加影像：
$$
Dst(x,y,z)=0.5\times (Src1(x,y,z)+Src2(x,y,z))
$$

## 寫法比較
+ CPU version
```shell=
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
```
+ GPU version
```shell=
__global__ void Overlap(uchar *src1, uchar *src2, uchar *dst)
{
	int tid = blockIdx.x*blockDim.x + threadIdx.x;
	dst[tid] = 0.5*(src1[tid] + src2[tid]);
}
```

## 結果
| No. | 1 | 2 |
|:--:|:--:|:--:|
| **Resolution** | 1280x800 pixels | 3000x1688 pixels |
| **CPU Exe. time (ms)** | 12 | 56 |
| **GPU Exe. time (ms)** | 1.87 | 9.16 |
| **Speedup (x tims)** | 6.41 | 6.11 |


![](http://i.imgur.com/lv6jcqu.jpg)

![](https://i.imgur.com/3oQ6Tlu.jpg)

## CPU完整程式碼
{%gist SiNcSaD/4d4cb5c5425e60b1a2a6adcf70d7f35c%}

## GPU完整程式碼
{%gist SiNcSaD/87f88f4879e5624af00e1e14e5d28ddb%}
