#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>

#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/opencv.hpp"    
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/core.hpp> 
#include "header/Detection.h"

using namespace cv;
using namespace std;

Detection::Detection()
{
	imgchangeClass = new ImgChange;
}

Detection::~Detection()
{
	;
}

QImage Detection::FaceDetection(QImage src) {
	CascadeClassifier cascade, nestedCascade;
	string cascadeName = "data/haarcascades/haarcascade_frontalface_alt.xml";
	string nestedCascadeName = "data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
	nestedCascade.load(samples::findFileOrKeep(nestedCascadeName));
	cascade.load(samples::findFile(cascadeName));
	Mat srcImg, dstImg, grayImg;
	srcImg = imgchangeClass->QImage2cvMat(src);
	dstImg = detectAndDraw(srcImg, cascade, nestedCascade, 1, true);
	QImage dst = imgchangeClass->cvMat2QImage(dstImg);
	return dst;
}

QImage Detection::PersonDetection(QImage src) {
	CascadeClassifier cascade, nestedCascade;
	string cascadeName = "data/haarcascades/haarcascade_frontalface_alt.xml";
	string nestedCascadeName = "data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
	nestedCascade.load(samples::findFileOrKeep(nestedCascadeName));
	cascade.load(samples::findFile(cascadeName));
	Mat srcImg, dstImg, grayImg;
	srcImg = imgchangeClass->QImage2cvMat(src);
	dstImg = detectAndDraw(srcImg, cascade, nestedCascade, 1, true);
	QImage dst = imgchangeClass->cvMat2QImage(dstImg);
	return dst;
}

QImage Detection::BeautifulDetection(QImage src) {

	Mat matResult;
	Mat srcImg = imgchangeClass->QImage2cvMat(src);
	const int bilateralFilterVal = 30;
	whiteFace(srcImg, 1.1, 68);
	GaussianBlur(srcImg, srcImg, Size(9, 9), 0, 0);
	//bilateralFilter(srcImg, matResult, bilateralFilterVal, bilateralFilterVal * 2, bilateralFilterVal / 2);

	//Mat matFinal;
	//cv::GaussianBlur(matResult, matFinal, cv::Size(0, 0), 9);
	//cv::addWeighted(matResult, 1.5, matFinal, -0.5, 0, matFinal);

	QImage dst = imgchangeClass->cvMat2QImage(srcImg);
	return dst;
}

QImage Detection::EdgeDetection(QImage src) {

	Mat srcImg, dstImg, grayImg;
	srcImg = imgchangeClass->QImage2cvMat(src);
	srcImg = Mat::zeros(srcImg.size(), CV_8UC3);
	blur(srcImg, srcImg, Size(3, 3));
	cvtColor(srcImg, srcImg, COLOR_BGR2GRAY);
	Canny(srcImg, srcImg, 20, 80, 3, false);
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(srcImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	RNG rng(0);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dstImg, contours, i, color, 2, 8, hierarchy, 0, Point(0, 0));
	}
	QImage dst = imgchangeClass->cvMat2QImage(dstImg);
	return dst;
}

void Detection::whiteFace(Mat& matSelfPhoto, int alpha, int beta)
{
	for (int y = 0; y < matSelfPhoto.rows; y++)
	{
		for (int x = 0; x < matSelfPhoto.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				matSelfPhoto.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * (matSelfPhoto.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}
}


Mat Detection::detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip)
{
	double t = 0;
	vector<Rect> faces, faces2;
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);
	t = (double)getTickCount();
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE,
		Size(30, 30));
	if (tryflip)
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale(smallImg, faces2,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r)
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)getTickCount() - t;
	printf("detection time = %g ms\n", t * 1000 / getTickFrequency());
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
		int radius;
		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width * 0.5) * scale);
			center.y = cvRound((r.y + r.height * 0.5) * scale);
			radius = cvRound((r.width + r.height) * 0.25 * scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, Point(cvRound(r.x * scale), cvRound(r.y * scale)),
				Point(cvRound((r.x + r.width - 1) * scale), cvRound((r.y + r.height - 1) * scale)),
				color, 3, 8, 0);
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(r);
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			//|CASCADE_DO_CANNY_PRUNING
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (size_t j = 0; j < nestedObjects.size(); j++)
		{
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
			center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
			radius = cvRound((nr.width + nr.height) * 0.25 * scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
	}
	return img;
}