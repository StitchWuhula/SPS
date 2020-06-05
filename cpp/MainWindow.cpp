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

// ��ʼ��
void ImageProcessing::init()
{
	imgVector.resize(2);

	// ���ͼ��
	setWindowIcon(QIcon("../Image/icon/draw.png"));

	// ��ĳ�ʼ��
	imgChangeClass = new ImgChange;
	geomClass = new Geom;
	grayClass = new Gray; 
	enhanceClass = new Enhance;  
	morpClass = new Morp;
	adjustClass = new Adjust;
	filterClass = new Filter;
	detectionClass = new Detection;

	// ������ڱ༭��
	outputEdit = new QTextEdit();
	outputEdit->textCursor().movePosition(QTextCursor::End);		// ��ֱ�������Զ����ٵ���ײ�
	outputEdit->setWordWrapMode(QTextOption::NoWrap);				// ˮƽ������
	outputEdit->setReadOnly(true);

	// ������
	drawType = 0;

	// ������� 
	isSkin = false;

	QFile file("../qss/black.qss");
	file.open(QFile::ReadOnly);
	QTextStream filetext(&file);
	stylesheet = filetext.readAll();
	file.close();
}

// ��ʾ����
void ImageProcessing::Show()
{
	// �����С
	setWindowTitle("SlimPhotoshop");
	resize(QApplication::desktop()->width()*0.9, QApplication::desktop()->height()*0.9);
	move(QApplication::desktop()->width()*0.05, QApplication::desktop()->height()*0.01);

	// �˵���
	I_menubar = new Menu(this, this);
	this->setMenuBar(I_menubar);

	// ����
	CreateLayout();

	// ͼ�񴰿�
	InitImage();	

	// ����QSSҳ��
	this->setStyleSheet(stylesheet);
	preWin->setStyleSheet(stylesheet);
	I_menubar->setMenuQSS(true);
	isSkin = true;

	imgchangeClass = new ImgChange;
}
