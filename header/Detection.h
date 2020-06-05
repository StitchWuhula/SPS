#pragma once
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
#include "header/ImgChange.h"

using namespace cv;
using namespace std;

class Detection
{
public:
	Detection();
	~Detection();

	QImage FaceDetection(QImage src);
	QImage PersonDetection(QImage src);
	QImage BeautifulDetection(QImage src);
	QImage EdgeDetection(QImage src);

	Mat detectAndDraw(Mat& img, CascadeClassifier& cascade,
		CascadeClassifier& nestedCascade,
		double scale, bool tryflip);

	void whiteFace(Mat& matSelfPhoto, int alpha, int beta);

private:
	ImgChange* imgchangeClass;
};