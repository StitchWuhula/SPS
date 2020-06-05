#include "header/Adjust.h"

using namespace cv;
using namespace std;

Adjust::Adjust()
{
	imgchangeClass = new ImgChange;
}

QImage Adjust::contrastAjust(Mat img, float alpha)
{
	Mat newImg;
	newImg = Mat::zeros(img.size(), img.type());
	if (img.channels() == 1) {
		
	}
	else {
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				for (int k = 0; k < 3; k++) {
					newImg.at<Vec3b>(i, j)[k] =
						saturate_cast<uchar>(alpha * (img.at<Vec3b>(i, j)[k]));
				}
			}
		}
	}
	QImage dst = imgchangeClass->cvMat2QImage(newImg);
	return dst;
}

QImage	Adjust::lightAdjust(Mat img, float beta)
{
	Mat newImg;

	newImg = Mat::zeros(img.size(), img.type());
	if (img.channels() == 1) {
		
	}
	else {
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				for (int k = 0; k < 3; k++) {
					newImg.at<Vec3b>(i, j)[k] =
						saturate_cast<uchar>(beta + (img.at<Vec3b>(i, j)[k]));
				}
			}
		}
	}
	QImage dst = imgchangeClass->cvMat2QImage(newImg);
	return dst;
}

Adjust::~Adjust()
{
	delete imgchangeClass;
}




