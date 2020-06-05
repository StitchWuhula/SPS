#pragma execution_character_set("utf-8")		
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>
#include <QSpinBox>

#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/opencv.hpp"    
#include <opencv2/core/core.hpp>  

#include "header/MainWindow.h"
#include "header/CreateMenu.h"
#include "header/ImgChange.h"
#include "header/Geom.h"
#include "header/Gray.h"
#include "header/Enhance.h"
#include "header/Morp.h"
#include "header/Adjust.h"
#include "header/Detection.h"

using namespace cv;
using namespace std;

ImageProcessing::ImageProcessing(QWidget *parent) :
	QMainWindow(parent)
{
	ui.setupUi(this);
	init();
	Show(); 
}

ImageProcessing::~ImageProcessing()
{
}

// 初始化
void ImageProcessing::init()
{
	imgVector.resize(2);

	// 软件图标
	setWindowIcon(QIcon("../Image/icon/draw.png"));

	// 类的初始化
	imgChangeClass = new ImgChange;
	geomClass = new Geom;
	grayClass = new Gray; 
	enhanceClass = new Enhance;  
	morpClass = new Morp;
	adjustClass = new Adjust;
	filterClass = new Filter;
	detectionClass = new Detection;

	// 输出窗口编辑框
	outputEdit = new QTextEdit();
	outputEdit->textCursor().movePosition(QTextCursor::End);		// 垂直滚动条自动跟踪到最底部
	outputEdit->setWordWrapMode(QTextOption::NoWrap);				// 水平滚动条
	outputEdit->setReadOnly(true);

	// 工具箱
	drawType = 0;

	// 界面设计 
	isSkin = false;

	QFile file("../qss/black.qss");
	file.open(QFile::ReadOnly);
	QTextStream filetext(&file);
	stylesheet = filetext.readAll();
	file.close();
}

// 显示界面
void ImageProcessing::Show()
{
	// 标题大小
	setWindowTitle("SlimPhotoshop");
	resize(QApplication::desktop()->width()*0.9, QApplication::desktop()->height()*0.9);
	move(QApplication::desktop()->width()*0.05, QApplication::desktop()->height()*0.01);

	// 菜单栏
	I_menubar = new Menu(this, this);
	this->setMenuBar(I_menubar);

	// 布局
	CreateLayout();

	// 图像窗口
	InitImage();	

	// 设置QSS页面
	this->setStyleSheet(stylesheet);
	preWin->setStyleSheet(stylesheet);
	I_menubar->setMenuQSS(true);
	isSkin = true;

	imgchangeClass = new ImgChange;
}
