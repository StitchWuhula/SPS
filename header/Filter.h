#pragma once
#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>

#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/opencv.hpp"    
#include <opencv2/core/core.hpp> 

#include "header/ImgChange.h"

class Filter
{
public:
	Filter();
	QImage frostedglass(cv::Mat img);
	QImage sketch(cv::Mat img);
	QImage emboss(cv::Mat img);
	QImage glare(cv::Mat src);
	QString currentPath;
	~Filter();

private:
	ImgChange* imgchangeClass;
};
