#ifndef ADJUST_H
#define ADJUST_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>

#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/opencv.hpp"    
#include <opencv2/core/core.hpp> 

#include "header/ImgChange.h"

using namespace cv;

class Adjust
{
public:
	Adjust();

	QImage contrastAjust(Mat img, float alpha);
	QImage lightAdjust(Mat img, float beta);

	~Adjust();

private:
	ImgChange* imgchangeClass;
};

#endif // ADJUST_H


