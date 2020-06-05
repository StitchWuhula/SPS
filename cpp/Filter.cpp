#include "..\header\Filter.h"

Filter::Filter()
{
	imgchangeClass = new ImgChange;
}

QImage Filter::frostedglass(cv::Mat img)
{
	int width = img.cols;
	int heigh = img.rows;
	RNG rng;
	Mat newImg(img.size(), CV_8UC3);
	for (int y = 1; y < heigh - 1; y++)
	{
		uchar* P0 = img.ptr<uchar>(y);
		uchar* P1 = newImg.ptr<uchar>(y);
		for (int x = 1; x < width - 1; x++)
		{
			int tmp = rng.uniform(0, 9);
			P1[3 * x] = img.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3));
			P1[3 * x + 1] = img.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 1);
			P1[3 * x + 2] = img.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 2);
		}
	}
	QImage dst = imgchangeClass->cvMat2QImage(newImg);
	return dst;
	
}

QImage Filter::sketch(cv::Mat img)
{

	int width = img.cols;
	int heigh = img.rows;
	Mat gray0, gray1;
	//去色  
	cvtColor(img, gray0, COLOR_BGR2GRAY);
	//反色  
	addWeighted(gray0, -1, NULL, 0, 255, gray1);
	//高斯模糊,高斯核的Size与最后的效果有关  
	GaussianBlur(gray1, gray1, Size(11, 11), 0);

	//融合：颜色减淡  
	Mat newImg(gray1.size(), CV_8UC1);
	for (int y = 0; y < heigh; y++)
	{

		uchar* P0 = gray0.ptr<uchar>(y);
		uchar* P1 = gray1.ptr<uchar>(y);
		uchar* P = newImg.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			int tmp0 = P0[x];
			int tmp1 = P1[x];
			P[x] = (uchar)min((tmp0 + (tmp0 * tmp1) / (256 - tmp1)), 255);
		}
	}
	QImage dst = imgchangeClass->cvMat2QImage(newImg);
	return dst;
}

QImage Filter::emboss(cv::Mat img)
{

	Mat img0(img.size(), CV_8UC3);
	for (int y = 1; y < img.rows - 1; y++)
	{
		uchar* p0 = img.ptr<uchar>(y);
		uchar* p1 = img.ptr<uchar>(y + 1);

		uchar* q = img0.ptr<uchar>(y);
		for (int x = 1; x < img.cols - 1; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				int tmp0 = p1[3 * (x + 1) + i] - p0[3 * (x - 1) + i] + 128;//浮雕
				if (tmp0 < 0)
					q[3 * x + i] = 0;
				else if (tmp0 > 255)
					q[3 * x + i] = 255;
				else
					q[3 * x + i] = tmp0;
			}
		}
	}
	QImage dst = imgchangeClass->cvMat2QImage(img0);
	return dst;
}

QImage Filter::glare(cv::Mat src)
{
	int width = src.cols;
	int heigh = src.rows;
	Mat img;
	src.copyTo(img);

	Mat dst(img.size(), CV_8UC3);

	float tmp, r;
	for (int y = 0; y < heigh; y++)
	{
		uchar* imgP = img.ptr<uchar>(y);
		uchar* dstP = dst.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			r = (float)imgP[3 * x];
			if (r > 127.5)
				tmp = r + (255 - r) * (r - 127.5) / 127.5;
			else
				tmp = r * r / 127.5;
			tmp = tmp > 255 ? 255 : tmp;
			tmp = tmp < 0 ? 0 : tmp;
			dstP[3 * x] = (uchar)(tmp);

			r = (float)imgP[3 * x + 1];
			if (r > 127.5)
				tmp = r + (255 - r) * (r - 127.5) / 127.5;
			else
				tmp = r * r / 127.5;
			tmp = tmp > 255 ? 255 : tmp;
			tmp = tmp < 0 ? 0 : tmp;
			dstP[3 * x + 1] = (uchar)(tmp);

			r = (float)imgP[3 * x + 2];
			if (r > 127.5)
				tmp = r + (255 - r) * (r - 127.5) / 127.5;
			else
				tmp = r * r / 127.5;
			tmp = tmp > 255 ? 255 : tmp;
			tmp = tmp < 0 ? 0 : tmp;
			dstP[3 * x + 2] = (uchar)(tmp);
		}
	}
	QImage result = imgchangeClass->cvMat2QImage(dst);
	return result;
}

Filter::~Filter()
{
	delete imgchangeClass;
}