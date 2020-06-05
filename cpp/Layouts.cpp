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
using namespace cv;
using namespace std;

// 创建布局
void ImageProcessing::CreateLayout()
{
	QWidget* p = takeCentralWidget();	//删除中央窗体
	if (p)
		delete p;
	setDockNestingEnabled(true);		//允许嵌套dock

	WinMain();		// 主体窗口
	WinTool();		// 工具箱
	WinPreview();	// 预览窗口
	WinGeom();		// 几何变换窗口
	WinGray();		// 灰度变换窗口
	WinEnhance();	// 图像增强窗口
	WinMorp();		// 形态学处理窗口	
	WinAdjust();	// 图像调整窗口
	WinProp();		// 属性窗口	
	ColorToolBar();	// 绘图工具栏
	WinText();
	// 窗口大小
	dock_Tool->setFixedWidth(100);
	dock_Geom->setFixedWidth(450);
	dock_Gray->setFixedWidth(450);
	dock_Enhance->setFixedWidth(450);
	dock_Morp->setFixedWidth(450);
	dock_Adjust->setFixedWidth(450);
	dock_Prop->setMaximumHeight(240);
	dock_Text->setFixedWidth(450);

	// 排序
	setCentralWidget(dock_Image);
	addDockWidget(Qt::LeftDockWidgetArea, dock_Tool);
	addDockWidget(Qt::BottomDockWidgetArea, dock_Output);
	addDockWidget(Qt::RightDockWidgetArea, dock_Geom);
	addDockWidget(Qt::RightDockWidgetArea, dock_Gray);
	addDockWidget(Qt::RightDockWidgetArea, dock_Enhance);
	addDockWidget(Qt::RightDockWidgetArea, dock_Morp);
	addDockWidget(Qt::RightDockWidgetArea, dock_Adjust);
	addDockWidget(Qt::RightDockWidgetArea, dock_Prop);
	addDockWidget(Qt::RightDockWidgetArea, dock_Text);

	splitDockWidget(dock_Tool, dock_Image, Qt::Horizontal);		// 水平或垂直
	splitDockWidget(dock_Geom, dock_Prop, Qt::Vertical);
	splitDockWidget(dock_Gray, dock_Prop, Qt::Vertical);
	splitDockWidget(dock_Enhance, dock_Prop, Qt::Vertical);
	splitDockWidget(dock_Morp, dock_Prop, Qt::Vertical);
	splitDockWidget(dock_Adjust, dock_Prop, Qt::Vertical);
	splitDockWidget(dock_Text, dock_Prop, Qt::Vertical);

	splitDockWidget(dock_Geom, dock_Output, Qt::Vertical);
	splitDockWidget(dock_Gray, dock_Output, Qt::Vertical);
	splitDockWidget(dock_Enhance, dock_Output, Qt::Vertical);
	splitDockWidget(dock_Morp, dock_Output, Qt::Vertical);
	splitDockWidget(dock_Adjust, dock_Output, Qt::Vertical);
	splitDockWidget(dock_Text, dock_Output, Qt::Vertical);

	tabifyDockWidget(dock_Geom, dock_Gray);		// 合并
	tabifyDockWidget(dock_Gray, dock_Enhance);
	tabifyDockWidget(dock_Enhance, dock_Morp);
	tabifyDockWidget(dock_Morp, dock_Adjust);
	tabifyDockWidget(dock_Adjust, dock_Text);
	tabifyDockWidget(dock_Output, dock_Prop);
	dock_Geom->raise();
	dock_Output->raise();


}

// 初始化图像
void ImageProcessing::InitImage()
{
	imgLabel = new PaintWidget(dock_Image);
	imgLabel->setScaledContents(true);

	QImage image = QImage(500, 500, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	imgLabel->setPenWidth(2);
	imgLabel->setPenColor(Qt::black);
	imgLabel->setImage(image);
	imgLabel->setPixmap(QPixmap::fromImage(image));
	imgVector[0].push_back(image);
	UpdateProp(image);
	UpdateImgName("图像");


	iter = imgVector[0].end() - 1;						// 最后一个元素对应的迭代器指针
	I_menubar->Act_edit_undo->setEnabled(false);		// 不允许撤销
	I_menubar->Act_edit_redo->setEnabled(false);		// 不允许重做

	// 增加滚动条
	ImgscrollArea = new QScrollArea();
	ImgscrollArea->setBackgroundRole(QPalette::Dark);
	ImgscrollArea->setAlignment(Qt::AlignCenter);
	ImgscrollArea->setWidget(imgLabel);
	dock_Image->setWidget(ImgscrollArea);
}

// 显示图像
void ImageProcessing::ShowImage(QImage Img, bool isSave)
{
	imgLabel->setImage(Img);
	imgLabel->setPixmap(QPixmap::fromImage(Img));
	imgLabel->resize(Img.width(), Img.height());
	UpdateProp(Img);
	if (isSave == true)
	{
		if (iter != (imgVector[0].end() - 1))
		{
			imgVector[0].erase(++iter, imgVector[0].end());
		}

		imgVector[0].push_back(Img);
		iter = imgVector[0].end() - 1;
		I_menubar->Act_edit_undo->setEnabled(true);
		I_menubar->Act_edit_redo->setEnabled(false);
	}
}

// 预览图像
void ImageProcessing::PreviewImage(QImage Img)
{
	preLabel->setImage(Img);
	preLabel->setPixmap(QPixmap::fromImage(Img));
	preLabel->resize(Img.width(), Img.height());
}

// 输出
void ImageProcessing::output(QString information)
{
	outputEdit->append(information);
}

void ImageProcessing::saveRaw(Mat src)
{
	imgLabel->setMat(src);
}

// --------------------------------------各个窗口-----------------------------------------------------------------

// 主体窗口
void ImageProcessing::WinMain()
{
	dock_Image = new QDockWidget(this);				//	图像
	dock_Image->setWindowTitle(QString("图像"));
	dock_Image->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	dock_Output = new QDockWidget(tr("输出窗口"), this);				// 输出窗口
	dock_Output->setFeatures(QDockWidget::AllDockWidgetFeatures);
	dock_Output->setWidget(outputEdit);
	dock_Output->setHidden(true);
}

// 预览窗口
void ImageProcessing::WinPreview()
{
	preWin = new CustomWindow;
	preWin->resize(700, 820);

	preButtonGroup = new QButtonGroup();
	connect(preButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(preButtonClicked(int)));

	// 主体框架
	preLabel = new PaintWidget(preWin);
	preLabel->setScaledContents(true);

	QScrollArea* scrollArea = new QScrollArea(preWin);// 增加滚动条
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setAlignment(Qt::AlignCenter);
	scrollArea->setWidget(preLabel);
	scrollArea->resize(600, 600);

	QImage image = QImage(550, 550, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	preLabel->setImage(image);
	preLabel->setPixmap(QPixmap::fromImage(image));
	preLabel->resize(image.width(), image.height());

	QPushButton* button_cancel = new QPushButton(tr("退出预览/Esc"), preWin);
	connect(button_cancel, SIGNAL(clicked()), preWin, SLOT(close()));

	scrollArea->move(50, 50);
	button_cancel->move(500, 770);
	button_cancel->resize(120, 30);

	// 缩放预览
	QLabel* label_size = new QLabel(tr("缩放倍数:"));
	spinbox_size = new QSpinBox(preWin);
	spinbox_size->setAlignment(Qt::AlignCenter);
	spinbox_size->setRange(-40, 40);
	spinbox_size->setValue(0);

	QSlider* slider_size = new QSlider(Qt::Horizontal, preWin);
	slider_size->setRange(-40, 40);
	slider_size->setValue(0);

	connect(spinbox_size, SIGNAL(valueChanged(int)), slider_size, SLOT(setValue(int)));
	connect(slider_size, SIGNAL(valueChanged(int)), spinbox_size, SLOT(setValue(int)));
	connect(spinbox_size, SIGNAL(valueChanged(int)), this, SLOT(enlargeReduce()));

	QHBoxLayout* sizeLayout = new QHBoxLayout();
	sizeLayout->addWidget(label_size);
	sizeLayout->setStretchFactor(label_size, 1);
	sizeLayout->addWidget(slider_size);
	sizeLayout->setStretchFactor(slider_size, 5);
	sizeLayout->addWidget(spinbox_size);
	sizeLayout->setStretchFactor(spinbox_size, 2);

	widget_pre[0] = new QWidget(preWin);
	widget_pre[0]->setLayout(sizeLayout);
	widget_pre[0]->move(50, 660);
	widget_pre[0]->resize(600, 100);

	// 旋转预览
	QLabel* label_rotate = new QLabel(tr("旋转角度:"));
	spinbox_rotate = new QSpinBox;
	spinbox_rotate->setAlignment(Qt::AlignCenter);
	spinbox_rotate->setRange(-360, 360);
	spinbox_rotate->setValue(0);

	QSlider* slider_rotate = new QSlider(Qt::Horizontal);
	slider_rotate->setRange(-360, 360);
	slider_rotate->setValue(0);

	connect(spinbox_rotate, SIGNAL(valueChanged(int)), slider_rotate, SLOT(setValue(int)));
	connect(slider_rotate, SIGNAL(valueChanged(int)), spinbox_rotate, SLOT(setValue(int)));
	connect(spinbox_rotate, SIGNAL(valueChanged(int)), this, SLOT(rotateImage()));

	QHBoxLayout* rotateLayout = new QHBoxLayout();
	rotateLayout->addWidget(label_rotate);
	rotateLayout->setStretchFactor(label_rotate, 1);
	rotateLayout->addWidget(slider_rotate);
	rotateLayout->setStretchFactor(slider_rotate, 5);
	rotateLayout->addWidget(spinbox_rotate);
	rotateLayout->setStretchFactor(spinbox_rotate, 2);

	widget_pre[1] = new QWidget(preWin);
	widget_pre[1]->setLayout(rotateLayout);
	widget_pre[1]->move(50, 660);
	widget_pre[1]->resize(600, 100);

	// 倾斜预览
	QLabel* label_leanX = new QLabel(tr("水平角度:"));
	spinbox_leanX = new QSpinBox();
	spinbox_leanX->setAlignment(Qt::AlignCenter);
	spinbox_leanX->setRange(0, 89);
	spinbox_leanX->setValue(0);
	QSlider* slider_leanX = new QSlider(Qt::Horizontal);
	slider_leanX->setRange(0, 89);
	slider_leanX->setValue(0);

	QLabel* label_leanY = new QLabel(tr("垂直角度:"));
	spinbox_leanY = new QSpinBox();
	spinbox_leanY->setAlignment(Qt::AlignCenter);
	spinbox_leanY->setRange(0, 89);
	spinbox_leanY->setValue(0);
	QSlider* slider_leanY = new QSlider(Qt::Horizontal);
	slider_leanY->setRange(0, 89);
	slider_leanY->setValue(0);

	connect(spinbox_leanX, SIGNAL(valueChanged(int)), slider_leanX, SLOT(setValue(int)));
	connect(slider_leanX, SIGNAL(valueChanged(int)), spinbox_leanX, SLOT(setValue(int)));
	connect(spinbox_leanY, SIGNAL(valueChanged(int)), slider_leanY, SLOT(setValue(int)));
	connect(slider_leanY, SIGNAL(valueChanged(int)), spinbox_leanY, SLOT(setValue(int)));
	connect(spinbox_leanX, SIGNAL(valueChanged(int)), this, SLOT(LeanImage()));
	connect(spinbox_leanY, SIGNAL(valueChanged(int)), this, SLOT(LeanImage()));

	QHBoxLayout* leanXLayout = new QHBoxLayout();
	leanXLayout->addWidget(label_leanX);
	leanXLayout->setStretchFactor(label_leanX, 1);
	leanXLayout->addWidget(slider_leanX);
	leanXLayout->setStretchFactor(slider_leanX, 5);
	leanXLayout->addWidget(spinbox_leanX);
	leanXLayout->setStretchFactor(spinbox_leanX, 2);
	QWidget* leanXwidget = new QWidget();
	leanXwidget->setLayout(leanXLayout);
	QHBoxLayout* leanYLayout = new QHBoxLayout();
	leanYLayout->addWidget(label_leanY);
	leanYLayout->setStretchFactor(label_leanY, 1);
	leanYLayout->addWidget(slider_leanY);
	leanYLayout->setStretchFactor(slider_leanY, 5);
	leanYLayout->addWidget(spinbox_leanY);
	leanYLayout->setStretchFactor(spinbox_leanY, 2);
	QWidget* leanYwidget = new QWidget();
	leanYwidget->setLayout(leanYLayout);
	QVBoxLayout* leanLayout = new QVBoxLayout();
	leanLayout->addWidget(leanXwidget);
	leanLayout->addWidget(leanYwidget);

	widget_pre[2] = new QWidget(preWin);
	widget_pre[2]->setLayout(leanLayout);
	widget_pre[2]->move(50, 650);
	widget_pre[2]->resize(600, 120);

	// 二值化预览
	QLabel* label_bin = new QLabel(tr("二值化阈值:"));
	spinbox_bin = new QSpinBox;
	spinbox_bin->setAlignment(Qt::AlignCenter);
	spinbox_bin->setRange(0, 255);
	spinbox_bin->setValue(100);
	QSlider* slider_bin = new QSlider(Qt::Horizontal);
	slider_bin->setRange(0, 255);
	slider_bin->setValue(100);

	connect(spinbox_bin, SIGNAL(valueChanged(int)), slider_bin, SLOT(setValue(int)));
	connect(slider_bin, SIGNAL(valueChanged(int)), spinbox_bin, SLOT(setValue(int)));
	connect(spinbox_bin, SIGNAL(valueChanged(int)), this, SLOT(binImage()));

	QHBoxLayout* binLayout = new QHBoxLayout();
	binLayout->addWidget(label_bin);
	binLayout->setStretchFactor(label_bin, 1);
	binLayout->addWidget(slider_bin);
	binLayout->setStretchFactor(slider_bin, 5);
	binLayout->addWidget(spinbox_bin);
	binLayout->setStretchFactor(spinbox_bin, 2);

	widget_pre[3] = new QWidget(preWin);
	widget_pre[3]->setLayout(binLayout);
	widget_pre[3]->move(50, 660);
	widget_pre[3]->resize(600, 100);

	// 线性变换
	QLabel* label_alpha = new QLabel(tr("Alpha:"));
	spinbox_alpha = new QSpinBox;
	spinbox_alpha->setAlignment(Qt::AlignCenter);
	spinbox_alpha->setRange(0, 500);
	spinbox_alpha->setValue(100);
	QSlider* slider_alpha = new QSlider(Qt::Horizontal);
	slider_alpha->setRange(0, 500);
	slider_alpha->setValue(100);

	QLabel* label_beta = new QLabel(tr("Beta:"));
	spinbox_beta = new QSpinBox;
	spinbox_beta->setAlignment(Qt::AlignCenter);
	spinbox_beta->setRange(0, 200);
	spinbox_beta->setValue(100);
	QSlider* slider_beta = new QSlider(Qt::Horizontal);
	slider_beta->setRange(0, 200);
	slider_beta->setValue(100);

	connect(spinbox_alpha, SIGNAL(valueChanged(int)), slider_alpha, SLOT(setValue(int)));
	connect(slider_alpha, SIGNAL(valueChanged(int)), spinbox_alpha, SLOT(setValue(int)));
	connect(spinbox_beta, SIGNAL(valueChanged(int)), slider_beta, SLOT(setValue(int)));
	connect(slider_beta, SIGNAL(valueChanged(int)), spinbox_beta, SLOT(setValue(int)));
	connect(spinbox_alpha, SIGNAL(valueChanged(int)), this, SLOT(linearImage()));
	connect(spinbox_beta, SIGNAL(valueChanged(int)), this, SLOT(linearImage()));

	QHBoxLayout* alphaLayout = new QHBoxLayout();
	alphaLayout->addWidget(label_alpha);
	alphaLayout->setStretchFactor(label_alpha, 1);
	alphaLayout->addWidget(slider_alpha);
	alphaLayout->setStretchFactor(slider_alpha, 5);
	alphaLayout->addWidget(spinbox_alpha);
	alphaLayout->setStretchFactor(spinbox_alpha, 2);
	QWidget* alphawidget = new QWidget();
	alphawidget->setLayout(alphaLayout);
	QHBoxLayout* betaLayout = new QHBoxLayout();
	betaLayout->addWidget(label_beta);
	betaLayout->setStretchFactor(label_beta, 1);
	betaLayout->addWidget(slider_beta);
	betaLayout->setStretchFactor(slider_beta, 5);
	betaLayout->addWidget(spinbox_beta);
	betaLayout->setStretchFactor(spinbox_beta, 2);
	QWidget* betawidget = new QWidget();
	betawidget->setLayout(betaLayout);
	QVBoxLayout* linearLayout = new QVBoxLayout();
	linearLayout->addWidget(alphawidget);
	linearLayout->addWidget(betawidget);

	widget_pre[4] = new QWidget(preWin);
	widget_pre[4]->setLayout(linearLayout);
	widget_pre[4]->move(50, 650);
	widget_pre[4]->resize(600, 120);

	// 伽马变换
	QLabel* label_gamma = new QLabel(tr("Gamma:"));
	spinbox_gamma = new QSpinBox;
	spinbox_gamma->setRange(0, 200);
	spinbox_gamma->setValue(100);
	QSlider* slider_gamma = new QSlider(Qt::Horizontal);
	slider_gamma->setRange(0, 200);
	slider_gamma->setValue(100);

	connect(spinbox_gamma, SIGNAL(valueChanged(int)), slider_gamma, SLOT(setValue(int)));
	connect(slider_gamma, SIGNAL(valueChanged(int)), spinbox_gamma, SLOT(setValue(int)));
	connect(spinbox_gamma, SIGNAL(valueChanged(int)), this, SLOT(gammaImage()));

	QHBoxLayout* gammaLayout = new QHBoxLayout();
	gammaLayout->addWidget(label_gamma);
	gammaLayout->setStretchFactor(label_gamma, 1);
	gammaLayout->addWidget(slider_gamma);
	gammaLayout->setStretchFactor(slider_gamma, 5);
	gammaLayout->addWidget(spinbox_gamma);
	gammaLayout->setStretchFactor(spinbox_gamma, 2);

	widget_pre[5] = new QWidget(preWin);
	widget_pre[5]->setLayout(gammaLayout);
	widget_pre[5]->move(50, 660);
	widget_pre[5]->resize(600, 100);

	// 对数变换
	QLabel* label_c = new QLabel(tr("C:"));
	spinbox_c = new QSpinBox;
	spinbox_c->setAlignment(Qt::AlignCenter);
	spinbox_c->setRange(0, 200);
	spinbox_c->setValue(100);
	QSlider* slider_c = new QSlider(Qt::Horizontal);
	slider_c->setRange(0, 200);
	slider_c->setValue(100);

	connect(spinbox_c, SIGNAL(valueChanged(int)), slider_c, SLOT(setValue(int)));
	connect(slider_c, SIGNAL(valueChanged(int)), spinbox_c, SLOT(setValue(int)));
	connect(spinbox_c, SIGNAL(valueChanged(int)), this, SLOT(logImage()));

	QHBoxLayout* logLayout = new QHBoxLayout();
	logLayout->addWidget(label_c);
	logLayout->setStretchFactor(label_c, 1);
	logLayout->addWidget(slider_c);
	logLayout->setStretchFactor(slider_c, 5);
	logLayout->addWidget(spinbox_c);
	logLayout->setStretchFactor(spinbox_c, 2);

	widget_pre[6] = new QWidget(preWin);
	widget_pre[6]->setLayout(logLayout);
	widget_pre[6]->move(50, 660);
	widget_pre[6]->resize(600, 100);
}

// 工具箱
void ImageProcessing::WinTool()
{
	dock_Tool = new QDockWidget(tr("工具箱"), this);				// 工具栏
	dock_Tool->setFeatures(QDockWidget::DockWidgetClosable);

	QPushButton* button_pen = new QPushButton(QIcon("../Image/toolbox/Painbrush.png"), tr(""), this);
	button_pen->setFixedSize(35, 35);
	button_pen->setToolTip(tr("钢笔"));
	button_pen->setObjectName("customButton");
	QPushButton* button_line = new QPushButton(QIcon("../Image/toolbox/Line.png"), tr(""), this);
	button_line->setFixedSize(35, 35);
	button_line->setToolTip(tr("线条"));
	button_line->setObjectName("customButton");
	QPushButton* button_circle = new QPushButton(QIcon("../Image/toolbox/Circle.png"), tr(""), this);
	button_circle->setFixedSize(35, 35);
	button_circle->setToolTip(tr("圆形"));
	button_circle->setObjectName("customButton");
	QPushButton* button_ellipse = new QPushButton(QIcon("../Image/toolbox/Elipse.png"), tr(""), this);
	button_ellipse->setFixedSize(35, 35);
	button_ellipse->setToolTip(tr("椭圆"));
	button_ellipse->setObjectName("customButton");
	QPushButton* button_triangle = new QPushButton(QIcon("../Image/toolbox/Triangle.png"), tr(""), this);
	button_triangle->setFixedSize(35, 35);
	button_triangle->setToolTip(tr("三角形"));
	button_triangle->setObjectName("customButton");
	QPushButton* button_rhombus = new QPushButton(QIcon("../Image/toolbox/Rhombus.png"), tr(""), this);
	button_rhombus->setFixedSize(35, 35);
	button_rhombus->setToolTip(tr("菱形"));
	button_rhombus->setObjectName("customButton");
	QPushButton* button_rect = new QPushButton(QIcon("../Image/toolbox/Rectangle.png"), tr(""), this);
	button_rect->setFixedSize(35, 35);
	button_rect->setToolTip(tr("长方形"));
	button_rect->setObjectName("customButton");
	QPushButton* button_square = new QPushButton(QIcon("../Image/toolbox/Square.png"), tr(""), this);
	button_square->setFixedSize(35, 35);
	button_square->setToolTip(tr("正方形"));
	button_square->setObjectName("customButton");
	QPushButton* button_hexagon = new QPushButton(QIcon("../Image/toolbox/Hexagon.png"), tr(""), this);
	button_hexagon->setFixedSize(35, 35);
	button_hexagon->setToolTip(tr("六边形"));
	button_hexagon->setObjectName("customButton");
	QPushButton* button_text = new QPushButton(QIcon("../Image/toolbox/Text.png"), tr(""), this);
	button_text->setFixedSize(35, 35);
	button_text->setToolTip(tr("文本框"));
	button_text->setObjectName("customButton");

	toolButtonGroup = new QButtonGroup();
	connect(toolButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(toolButtonClicked(int)));
	toolButtonGroup->addButton(button_pen, 1);
	toolButtonGroup->addButton(button_line, 2);
	toolButtonGroup->addButton(button_ellipse, 3);
	toolButtonGroup->addButton(button_circle, 4);
	toolButtonGroup->addButton(button_triangle, 5);
	toolButtonGroup->addButton(button_rhombus, 6);
	toolButtonGroup->addButton(button_rect, 7);
	toolButtonGroup->addButton(button_square, 8);
	toolButtonGroup->addButton(button_hexagon, 9);
	toolButtonGroup->addButton(button_text, 10);

	QGridLayout* toolLayout = new QGridLayout();
	toolLayout->setAlignment(Qt::AlignTop);
	toolLayout->addWidget(button_pen, 0, 0);
	toolLayout->addWidget(button_line, 0, 1);
	toolLayout->addWidget(button_ellipse, 1, 1);
	toolLayout->addWidget(button_circle, 1, 0);
	toolLayout->addWidget(button_triangle, 2, 0);
	toolLayout->addWidget(button_rhombus, 2, 1);
	toolLayout->addWidget(button_rect, 3, 0);
	toolLayout->addWidget(button_square, 3, 1);
	toolLayout->addWidget(button_hexagon, 4, 0);
	toolLayout->addWidget(button_text, 4, 1);

	QWidget* toolWidget = new QWidget(dock_Tool);
	toolWidget->setLayout(toolLayout);
	dock_Tool->setWidget(toolWidget);
}

// 绘图工具栏
void ImageProcessing::ColorToolBar()
{
	I_ColorBar = new QToolBar();
	I_ColorBar = addToolBar(tr("画笔工具栏"));
	I_ColorBar->setMovable(true);

	QLabel* label_penWidth = new QLabel(tr("线宽"));
	spinbox_penWidth = new QSpinBox;
	spinbox_penWidth->setRange(1, 50);
	spinbox_penWidth->setValue(2);

	QPushButton* button_color = new QPushButton(tr("画笔颜色"));

	frame_color = new QFrame;
	frame_color->setObjectName("customFrame");
	frame_color->setFrameShape(QFrame::Box);
	frame_color->setPalette(QPalette(Qt::black));
	frame_color->setAutoFillBackground(true);
	frame_color->setFixedSize(25, 25);
	frame_color->setStyleSheet("QFrame{background-color: rgba(" + QString::number(0) + "," + QString::number(0) + "," + QString::number(0) + ",1);border:none}");

	QPushButton* button_full = new QPushButton(tr("铺满窗口"));
	QPushButton* button_auto = new QPushButton(tr("自适应窗口"));

	connect(spinbox_penWidth, SIGNAL(valueChanged(int)), this, SLOT(penWidth()));
	connect(button_color, SIGNAL(clicked()), this, SLOT(penColor()));
	connect(button_full, SIGNAL(clicked()), this, SLOT(fullSize()));
	connect(button_auto, SIGNAL(clicked()), this, SLOT(autoSize()));

	QPushButton* button_larger = new QPushButton(tr("放大一倍"));
	QPushButton* button_smaller = new QPushButton(tr("缩小一倍"));

	connect(button_larger, SIGNAL(clicked()), this, SLOT(enlarge2()));
	connect(button_smaller, SIGNAL(clicked()), this, SLOT(reduce2()));

	QHBoxLayout* colorLayout = new QHBoxLayout();
	colorLayout->setAlignment(Qt::AlignLeft);

	colorLayout->addWidget(label_penWidth);
	colorLayout->addWidget(spinbox_penWidth);
	colorLayout->addWidget(button_color);
	colorLayout->addWidget(frame_color);
	colorLayout->addWidget(button_full);
	colorLayout->addWidget(button_auto);
	colorLayout->addWidget(button_larger);
	colorLayout->addWidget(button_smaller);

	QWidget* colorWidget = new QWidget();
	colorWidget->setLayout(colorLayout);
	I_ColorBar->addWidget(colorWidget);
}

// 属性窗口
void ImageProcessing::WinProp()
{
	dock_Prop = new QDockWidget(tr("属性"), this);				// 属性窗口
	dock_Prop->setFeatures(QDockWidget::AllDockWidgetFeatures);
	dock_Prop->setHidden(true);

	line_img = new QLineEdit();
	line_length = new QLineEdit();
	line_width = new QLineEdit();
	line_depth = new QLineEdit();
	line_isGray = new QLineEdit();

	line_img->setReadOnly(true);
	line_length->setReadOnly(true);
	line_width->setReadOnly(true);
	line_depth->setReadOnly(true);
	line_isGray->setReadOnly(true);

	QFormLayout* propLayout = new QFormLayout();
	propLayout->addRow(tr("图像"), line_img);
	propLayout->addRow(tr("长度"), line_length);
	propLayout->addRow(tr("宽度"), line_width);
	propLayout->addRow(tr("深度"), line_depth);
	propLayout->addRow(tr("是否灰度"), line_isGray);
	propLayout->setSpacing(20);
	propLayout->setMargin(20);

	QWidget* propWidget = new QWidget(dock_Prop);
	propWidget->setLayout(propLayout);
	dock_Prop->setWidget(propWidget);
}

// 更新图像地址
void ImageProcessing::UpdateImgName(QString name)
{
	line_img->setText(name);
}

// 更新属性窗口数据
void ImageProcessing::UpdateProp(QImage Img)
{
	line_length->setText(QString::number(Img.width()));
	line_width->setText(QString::number(Img.height()));
	line_depth->setText(QString::number(Img.depth()));
	if (Img.depth() == 8 || Img.depth() == 1)
		line_isGray->setText("是");
	else
		line_isGray->setText("否");
}

// 几何变换窗口
void ImageProcessing::WinGeom()
{
	dock_Geom = new QDockWidget(tr("几何变换"), this);				// 几何变换
	dock_Geom->setFeatures(QDockWidget::AllDockWidgetFeatures);


	// 图像大小
	QLabel* null = new QLabel(tr("	"));
	QLabel* label_area = new QLabel(tr("图像大小"));
	QLabel* labelL = new QLabel(tr("图像长度:"));
	QLabel* labelW = new QLabel(tr("图像宽度:"));
	QPushButton* buttonDefault = new QPushButton(tr("默认"));
	QPushButton* buttonOK = new QPushButton(tr("确认"));
	line_L = new QLineEdit("512");
	line_L->setValidator(new QIntValidator(1, 10000));		// 只能输入数字
	line_L->setAlignment(Qt::AlignCenter);
	line_L->setStyleSheet("min-width: 50px;");
	line_W = new QLineEdit("512");
	line_W->setValidator(new QIntValidator(1, 10000));
	line_W->setAlignment(Qt::AlignCenter);

	connect(buttonDefault, SIGNAL(clicked()), this, SLOT(autoSize()));
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(changeImageSize()));

	// 图像缩放
	QLabel* label_size = new QLabel(tr("图像缩放"));
	QLabel* label_times = new QLabel(tr("缩放倍数:"));
	QPushButton* button_size = new QPushButton(tr("确认"));
	QPushButton* buttonPre_size = new QPushButton(tr("预览"));

	line_times = new QSpinBox();
	line_times->setAlignment(Qt::AlignCenter);
	line_times->setRange(-10, 10);
	line_times->setValue(0);
	connect(button_size, SIGNAL(clicked()), this, SLOT(enlargeReduceOK()));

	// 图像旋转
	QLabel* label_rotate = new QLabel(tr("图像旋转"));
	QLabel* label_degree = new QLabel(tr("旋转角度:"));
	QPushButton* button_rotate = new QPushButton(tr("确认"));
	QPushButton* buttonPre_rotate = new QPushButton(tr("预览"));
	QPushButton* button_left = new QPushButton(tr("顺时针90"));
	QPushButton* button_right = new QPushButton(tr("逆时针90"));
	QPushButton* button_180 = new QPushButton(tr("旋转180"));
	//line_degree = new QLineEdit("0");
	//line_degree->setValidator(new QIntValidator(-360, 360));		// 只能输入数字
	//line_degree->setAlignment(Qt::AlignCenter);
	line_degree = new QSpinBox();
	line_degree->setAlignment(Qt::AlignCenter);
	line_degree->setRange(-360, 360);
	line_degree->setValue(0);
	connect(button_rotate, SIGNAL(clicked()), this, SLOT(rotateImageOK()));
	connect(button_left, SIGNAL(clicked()), this, SLOT(rotateImage_270()));
	connect(button_right, SIGNAL(clicked()), this, SLOT(rotateImage_90()));
	connect(button_180, SIGNAL(clicked()), this, SLOT(rotateImage_180()));

	// 图像倾斜
	QLabel* label_lean = new QLabel(tr("图像倾斜"));
	QLabel* label_leanX = new QLabel(tr("水平角度:"));
	QLabel* label_leanY = new QLabel(tr("垂直角度:"));
	QPushButton* button_lean = new QPushButton(tr("确认"));
	QPushButton* buttonPre_lean = new QPushButton(tr("预览"));

	line_leanX = new QSpinBox();
	line_leanX->setAlignment(Qt::AlignCenter);
	line_leanX->setRange(0, 89);
	line_leanX->setValue(0);
	line_leanY = new QSpinBox();
	line_leanY->setAlignment(Qt::AlignCenter);
	line_leanY->setRange(0, 89);
	line_leanY->setValue(0);
	connect(button_lean, SIGNAL(clicked()), this, SLOT(LeanImageOK()));

	// 图像翻转
	QLabel* label_reverse = new QLabel(tr("图像翻转"));
	QLabel* label_H = new QLabel(tr("水平翻转:"));
	QLabel* label_V = new QLabel(tr("垂直翻转:"));
	QPushButton* button_H = new QPushButton(tr("水平翻转"));
	QPushButton* button_V = new QPushButton(tr("垂直翻转"));
	connect(button_H, SIGNAL(clicked()), this, SLOT(flipImage_H()));
	connect(button_V, SIGNAL(clicked()), this, SLOT(flipImage_V()));

	// 预览按钮组
	preButtonGroup->addButton(buttonPre_size, 0);
	preButtonGroup->addButton(buttonPre_rotate, 1);
	preButtonGroup->addButton(buttonPre_lean, 2);

	// 格栅布局
	QGridLayout* geomLayout = new QGridLayout();
	geomLayout->setAlignment(Qt::AlignTop);
	geomLayout->setMargin(30);

	geomLayout->addWidget(label_area, 0, 0, 1, 1);// 图像大小
	geomLayout->addWidget(labelL, 1, 1, 1, 1);
	geomLayout->addWidget(line_L, 1, 2, 1, 1);
	geomLayout->addWidget(labelW, 2, 1, 1, 1);
	geomLayout->addWidget(line_W, 2, 2, 1, 1);
	geomLayout->addWidget(buttonOK, 1, 3, 1, 1);
	geomLayout->addWidget(buttonDefault, 1, 4, 1, 1);

	geomLayout->addWidget(null, 10, 0, 1, 1);// 图像缩放
	geomLayout->addWidget(label_size, 11, 0, 1, 1);
	geomLayout->addWidget(label_times, 12, 1, 1, 1);
	geomLayout->addWidget(line_times, 12, 2, 1, 1);
	geomLayout->addWidget(button_size, 12, 3, 1, 1);
	geomLayout->addWidget(buttonPre_size, 12, 4, 1, 1);
	geomLayout->addWidget(null, 14, 0, 1, 1);

	geomLayout->addWidget(null, 20, 0, 1, 1);// 图像旋转
	geomLayout->addWidget(label_rotate, 21, 0, 1, 1);
	geomLayout->addWidget(label_degree, 22, 1, 1, 1);
	geomLayout->addWidget(line_degree, 22, 2, 1, 1);
	geomLayout->addWidget(button_rotate, 22, 3, 1, 1);
	geomLayout->addWidget(buttonPre_rotate, 22, 4, 1, 1);
	geomLayout->addWidget(button_180, 23, 2, 1, 1);
	geomLayout->addWidget(button_left, 23, 3, 1, 1);
	geomLayout->addWidget(button_right, 23, 4, 1, 1);

	geomLayout->addWidget(null, 30, 0, 1, 1);// 图像倾斜
	geomLayout->addWidget(label_lean, 31, 0, 1, 1);
	geomLayout->addWidget(label_leanX, 32, 1, 1, 1);
	geomLayout->addWidget(line_leanX, 32, 2, 1, 1);
	geomLayout->addWidget(label_leanY, 33, 1, 1, 1);
	geomLayout->addWidget(line_leanY, 33, 2, 1, 1);
	geomLayout->addWidget(button_lean, 32, 3, 1, 1);
	geomLayout->addWidget(buttonPre_lean, 32, 4, 1, 1);

	geomLayout->addWidget(null, 40, 0, 1, 1);// 图像翻转
	geomLayout->addWidget(label_reverse, 41, 0, 1, 1);
	//	geomLayout->addWidget(label_H, 42, 1, 1, 1);
	geomLayout->addWidget(button_H, 42, 1, 1, 2);
	//	geomLayout->addWidget(label_V, 43, 1, 1, 1);
	geomLayout->addWidget(button_V, 42, 3, 1, 2);

	QWidget* geomWidget = new QWidget(dock_Geom);
	geomWidget->setFixedWidth(420);
	geomWidget->setLayout(geomLayout);
	dock_Geom->setWidget(geomWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Geom);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(geomWidget);
	dock_Geom->setWidget(scrollArea);


}



void ImageProcessing::WinText()
{
	dock_Text = new QDockWidget(tr("文字处理"), this);
	dock_Text->setFeatures(QDockWidget::AllDockWidgetFeatures);

	/*QLabel* label_kernel = new QLabel();
	label_kernel->setText(tr("模板大小"));
	combo_kernel = new QComboBox();
	combo_kernel->addItem(tr("3×3")); combo_kernel->addItem(tr("5×5"));
	combo_kernel->addItem(tr("7×7")); combo_kernel->addItem(tr("9×9"));
	combo_kernel->addItem(tr("11×11")); combo_kernel->addItem(tr("13×13"));
	combo_kernel->addItem(tr("15×15")); combo_kernel->addItem(tr("17×17"));
	combo_kernel->addItem(tr("19×19")); combo_kernel->addItem(tr("21×21"));

	QLabel* label_elem = new QLabel();
	label_elem->setText(tr("结构元素"));
	combo_elem = new QComboBox();
	combo_elem->addItem(tr("矩形"));
	combo_elem->addItem(tr("十字形"));
	combo_elem->addItem(tr("椭圆形"));

	QLabel* label_erode = new QLabel();
	label_erode->setText(tr("腐蚀"));
	QLabel* label_erodeCount = new QLabel();
	label_erodeCount->setText(tr("次"));
	spinbox_erode = new QSpinBox;
	spinbox_erode->setRange(1, 50);
	spinbox_erode->setValue(1);
	QPushButton* button_erode = new QPushButton();			// 确定按钮
	button_erode->setText(tr("腐蚀"));

	QLabel* label_dilate = new QLabel();
	label_dilate->setText(tr("膨胀"));
	QLabel* label_dilateCount = new QLabel();
	label_dilateCount->setText(tr("次"));
	spinbox_dilate = new QSpinBox;
	spinbox_dilate->setRange(1, 50);
	spinbox_dilate->setValue(1);
	QPushButton* button_dilate = new QPushButton();			// 确定按钮
	button_dilate->setText(tr("膨胀"));

	QLabel* label_open = new QLabel();
	label_open->setText(tr("开运算"));
	QLabel* label_openCount = new QLabel();
	label_openCount->setText(tr("次"));
	spinbox_open = new QSpinBox;
	spinbox_open->setRange(1, 50);
	spinbox_open->setValue(1);
	QPushButton* button_open = new QPushButton();			// 确定按钮
	button_open->setText(tr("开运算"));

	QLabel* label_close = new QLabel();
	label_close->setText(tr("闭运算"));
	QLabel* label_closeCount = new QLabel();
	label_closeCount->setText(tr("次"));
	spinbox_close = new QSpinBox;
	spinbox_close->setRange(1, 50);
	spinbox_close->setValue(1);
	QPushButton* button_close = new QPushButton();			// 确定按钮
	button_close->setText(tr("闭运算"));

	QLabel* label_grad = new QLabel();
	label_grad->setText(tr("形态学梯度"));
	QPushButton* button_grad = new QPushButton();			// 确定按钮
	button_grad->setText(tr("形态学梯度"));

	QLabel* label_tophat = new QLabel();
	label_tophat->setText(tr("顶帽操作"));
	QPushButton* button_tophat = new QPushButton();			// 确定按钮
	button_tophat->setText(tr("顶帽操作"));

	QLabel* label_blackhat = new QLabel();
	label_blackhat->setText(tr("黑帽操作"));
	QPushButton* button_blackhat = new QPushButton();			// 确定按钮
	button_blackhat->setText(tr("黑帽操作"));

	connect(button_erode, SIGNAL(clicked()), this, SLOT(erodeImage()));
	connect(button_dilate, SIGNAL(clicked()), this, SLOT(dilateImage()));
	connect(button_open, SIGNAL(clicked()), this, SLOT(openImage()));
	connect(button_close, SIGNAL(clicked()), this, SLOT(closeImage()));
	connect(button_grad, SIGNAL(clicked()), this, SLOT(gradImage()));
	connect(button_tophat, SIGNAL(clicked()), this, SLOT(tophatImage()));
	connect(button_blackhat, SIGNAL(clicked()), this, SLOT(blackhatImage()));

	QGridLayout* morpLayout = new QGridLayout();
	morpLayout->setAlignment(Qt::AlignTop);
	morpLayout->setMargin(30);

	morpLayout->addWidget(label_kernel, 0, 1, 1, 1);
	morpLayout->addWidget(combo_kernel, 0, 2, 1, 1);
	morpLayout->addWidget(label_elem, 1, 1, 1, 1);
	morpLayout->addWidget(combo_elem, 1, 2, 1, 1);

	morpLayout->addWidget(label_erode, 2, 0, 1, 1);
	morpLayout->addWidget(button_erode, 3, 1, 1, 1);
	morpLayout->addWidget(spinbox_erode, 3, 2, 1, 1);
	morpLayout->addWidget(label_erodeCount, 3, 3, 1, 1);

	morpLayout->addWidget(label_dilate, 4, 0, 1, 1);
	morpLayout->addWidget(button_dilate, 5, 1, 1, 1);
	morpLayout->addWidget(spinbox_dilate, 5, 2, 1, 1);
	morpLayout->addWidget(label_dilateCount, 5, 3, 1, 1);

	morpLayout->addWidget(label_open, 6, 0, 1, 1);
	morpLayout->addWidget(button_open, 7, 1, 1, 1);
	morpLayout->addWidget(spinbox_open, 7, 2, 1, 1);
	morpLayout->addWidget(label_openCount, 7, 3, 1, 1);

	morpLayout->addWidget(label_close, 8, 0, 1, 1);
	morpLayout->addWidget(button_close, 9, 1, 1, 1);
	morpLayout->addWidget(spinbox_close, 9, 2, 1, 1);
	morpLayout->addWidget(label_closeCount, 9, 3, 1, 1);

	morpLayout->addWidget(label_tophat, 10, 0, 1, 1);
	morpLayout->addWidget(button_tophat, 11, 1, 1, 1);
	morpLayout->addWidget(label_blackhat, 12, 0, 1, 1);
	morpLayout->addWidget(button_blackhat, 13, 1, 1, 1);
	morpLayout->addWidget(label_grad, 14, 0, 1, 1);
	morpLayout->addWidget(button_grad, 15, 1, 1, 1);

	QWidget* morpWidget = new QWidget(dock_Morp);
	morpWidget->setFixedWidth(420);
	morpWidget->setLayout(morpLayout);
	dock_Morp->setWidget(morpWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Morp);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(morpWidget);
	dock_Morp->setWidget(scrollArea);*/

	/*I_ColorBar = new QToolBar();
	I_ColorBar = addToolBar(tr("画笔工具栏"));
	I_ColorBar->setMovable(true);

	QLabel* label_penWidth = new QLabel(tr("线宽"));
	label_penWidth->setMaximumWidth(40);
	spinbox_penWidth = new QSpinBox;
	spinbox_penWidth->setRange(1, 50);
	spinbox_penWidth->setValue(2);
	spinbox_penWidth->setMaximumWidth(40);

	QPushButton* button_color = new QPushButton(tr("画笔颜色"));
	button_color->setMaximumWidth(100);

	frame_color = new QFrame;
	frame_color->setObjectName("customFrame");
	frame_color->setFrameShape(QFrame::Box);
	frame_color->setPalette(QPalette(Qt::black));
	frame_color->setAutoFillBackground(true);
	frame_color->setFixedSize(25, 25);
	frame_color->setStyleSheet("QFrame{background-color: rgba(" + QString::number(0) + "," + QString::number(0) + "," + QString::number(0) + ",1);border:none}");

	QPushButton* button_full = new QPushButton(tr("铺满窗口"));
	button_full->setMaximumWidth(100);
	QPushButton* button_auto = new QPushButton(tr("自适应窗口"));
	button_auto->setMaximumWidth(100);
	QPushButton* button_skin = new QPushButton(tr("一键换肤"));
	button_skin->setMaximumWidth(100);

	connect(spinbox_penWidth, SIGNAL(valueChanged(int)), this, SLOT(penWidth()));
	connect(button_color, SIGNAL(clicked()), this, SLOT(penColor()));
	connect(button_full, SIGNAL(clicked()), this, SLOT(fullSize()));
	connect(button_auto, SIGNAL(clicked()), this, SLOT(autoSize()));
	connect(button_skin, SIGNAL(clicked()), this, SLOT(changeSkin()));*/


	QLabel* label_penWidth2 = new QLabel(tr("文本输入"));
	label_penWidth2->setMaximumWidth(60);
	/*spinbox_penWidth2 = new QSpinBox;
	spinbox_penWidth2->setRange(300, 400);*/
	Text = new QLineEdit;
	//Text->setMaximumWidth(10);
	Text->setText("文字输入");
	Text->setMaximumWidth(150);
	Size = new QLineEdit;
	//Text->setMaximumWidth(10);
	Size = new QLineEdit;
	//Text->setMaximumWidth(10);
	Size->setText("10");
	Size->setMaximumWidth(60);
	Font = new QComboBox();
	Font->setMaximumWidth(150);
	Font->addItem(tr("宋体"));
	Font->addItem(tr("黑体"));
	Font->addItem(tr("微软雅黑"));
	Font->addItem(tr("Times New Roman"));

	/*Font = new QLineEdit;
	//Text->setMaximumWidth(10);
	Font->setText("宋体");
	Font->setMaximumWidth(60);*/


	QLabel* label_penWidth4 = new QLabel(tr("大小"));
	label_penWidth4->setMaximumWidth(30);
	QLabel* label_penWidth5 = new QLabel(tr("字体"));
	label_penWidth5->setMaximumWidth(30);
	QString fontString = Font->currentText();
	QString textString = Text->text();
	QString sizeString = Size->text();

	//Text->editingFinished();

	connect(Text, SIGNAL(textChanged(const QString&)), this, SLOT(textChange()));
	connect(Font, SIGNAL(currentTextChanged(const QString&)), this, SLOT(textChange()));
	connect(Size, SIGNAL(textChanged(const QString&)), this, SLOT(textChange()));
	//	printf("%s.%s",fontString,textString);//////////
		//spinbox_penWidth2->setSpecialValueText(tr("OOP小组：许嘉诚，代艺博，叶江龙，刘长硕"));
		//spinbox_penWidth->setValue("OOP小组：许嘉诚，代艺博，叶江龙，刘长硕");

	QGridLayout* textLayout = new QGridLayout();
	textLayout->setAlignment(Qt::AlignLeft);
	textLayout->setMargin(30);

	textLayout->addWidget(label_penWidth2, 0, 0, 1, 1);
	textLayout->addWidget(Text, 1, 1, 1, 1);
	textLayout->addWidget(NULL, 7, 1, 1, 1);
	textLayout->addWidget(label_penWidth5, 8, 0, 1, 1);
	textLayout->addWidget(Font, 9, 1, 1, 1);
	textLayout->addWidget(NULL, 7, 1, 1, 1);
	textLayout->addWidget(label_penWidth4, 15, 0, 1, 1);
	textLayout->addWidget(Size, 16, 1, 1, 1);

	QWidget* textWidget = new QWidget(dock_Text);
	textWidget->setFixedWidth(420);
	textWidget->setLayout(textLayout);
	dock_Text->setWidget(textWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Text);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(textWidget);
	dock_Text->setWidget(scrollArea);

}

void ImageProcessing::textChange()
{
	imgLabel->textChange(Font->currentText(), Text->text(), Size->text().toInt());
}


// 灰度变换窗口
void ImageProcessing::WinGray()
{
	dock_Gray = new QDockWidget(tr("灰度变换"), this);
	dock_Gray->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QLabel* label_linear = new QLabel(tr("线性变换"));
	QLabel* label_alpha = new QLabel(tr("Alpha:"));
	QLabel* label_beta = new QLabel(tr(" Beta:"));
	QPushButton* button_linear = new QPushButton(tr("确认"));
	QPushButton* buttonPre_linear = new QPushButton(tr("预览"));

	line_alpha = new QSpinBox();
	line_alpha->setAlignment(Qt::AlignCenter);
	line_alpha->setRange(0, 500);
	line_alpha->setValue(100);
	line_beta = new QSpinBox();
	line_beta->setAlignment(Qt::AlignCenter);
	line_beta->setRange(0, 200);
	line_beta->setValue(100);

	QLabel* label_log = new QLabel(tr("对数变换"));
	QLabel* label_c = new QLabel(tr("  Log:"));
	QPushButton* button_log = new QPushButton(tr("确认"));
	QPushButton* buttonPre_log = new QPushButton(tr("预览"));

	line_c = new QSpinBox();
	line_c->setAlignment(Qt::AlignCenter);
	line_c->setRange(0, 200);
	line_c->setValue(100);

	QLabel* label_gamma = new QLabel(tr("伽马变换"));
	QLabel* label_gam = new QLabel(tr("Gamma:"));
	QPushButton* button_gamma = new QPushButton(tr("确认"));
	QPushButton* buttonPre_gamma = new QPushButton(tr("预览"));

	line_gamma = new QSpinBox();
	line_gamma->setAlignment(Qt::AlignCenter);
	line_gamma->setRange(0, 200);
	line_gamma->setValue(100);

	// 二值图像
	QLabel* null = new QLabel(tr("	"));
	QLabel* label_bin = new QLabel(tr("二值图像"));
	QLabel* label_threshold = new QLabel(tr(" 阈值:"));
	QPushButton* button_bin = new QPushButton(tr("确认"));
	QPushButton* buttonPre_bin = new QPushButton(tr("预览"));

	line_bin = new QSpinBox();
	line_bin->setAlignment(Qt::AlignCenter);
	line_bin->setRange(0, 255);
	line_bin->setValue(100);

	QLabel* label_graylevel = new QLabel(tr("灰度图像"));
	QPushButton* button_graylevel = new QPushButton(tr("灰度图像"));

	QLabel* label_reverse = new QLabel(tr("反转变换"));
	QPushButton* button_reverse = new QPushButton(tr("图像反转"));

	QLabel* label_histeq = new QLabel();
	label_histeq->setText(tr("直方图"));
	QPushButton* button_histeq = new QPushButton();
	button_histeq->setText(tr("灰度直方图均衡化"));

	preButtonGroup->addButton(buttonPre_bin, 3);
	preButtonGroup->addButton(buttonPre_linear, 4);
	preButtonGroup->addButton(buttonPre_gamma, 5);
	preButtonGroup->addButton(buttonPre_log, 6);

	connect(button_bin, SIGNAL(clicked()), this, SLOT(binImageOK()));
	connect(button_graylevel, SIGNAL(clicked()), this, SLOT(grayImage()));
	connect(button_reverse, SIGNAL(clicked()), this, SLOT(reverseImage()));
	connect(button_linear, SIGNAL(clicked()), this, SLOT(linearImageOK()));
	connect(button_gamma, SIGNAL(clicked()), this, SLOT(gammaImageOK()));
	connect(button_log, SIGNAL(clicked()), this, SLOT(logImageOK()));
	connect(button_histeq, SIGNAL(clicked()), this, SLOT(histeqImage()));

	// 格栅布局
	QGridLayout* grayLayout = new QGridLayout();
	grayLayout->setAlignment(Qt::AlignTop);
	grayLayout->setMargin(30);

	grayLayout->addWidget(label_linear, 16, 0, 1, 1);
	grayLayout->addWidget(label_alpha, 17, 1, 1, 1);
	grayLayout->addWidget(line_alpha, 17, 2, 1, 1);
	grayLayout->addWidget(label_beta, 18, 1, 1, 1);
	grayLayout->addWidget(line_beta, 18, 2, 1, 1);
	grayLayout->addWidget(button_linear, 17, 3, 1, 1);
	grayLayout->addWidget(buttonPre_linear, 17, 4, 1, 1);

	grayLayout->addWidget(null, 20, 0, 1, 1);
	grayLayout->addWidget(label_log, 21, 0, 1, 1);
	grayLayout->addWidget(label_c, 22, 1, 1, 1);
	grayLayout->addWidget(line_c, 22, 2, 1, 1);
	grayLayout->addWidget(button_log, 22, 3, 1, 1);
	grayLayout->addWidget(buttonPre_log, 22, 4, 1, 1);

	grayLayout->addWidget(null, 25, 0, 1, 1);
	grayLayout->addWidget(label_gamma, 26, 0, 1, 1);
	grayLayout->addWidget(label_gam, 27, 1, 1, 1);
	grayLayout->addWidget(line_gamma, 27, 2, 1, 1);
	grayLayout->addWidget(button_gamma, 27, 3, 1, 1);
	grayLayout->addWidget(buttonPre_gamma, 27, 4, 1, 1);

	grayLayout->addWidget(null, 30, 0, 1, 1);
	grayLayout->addWidget(label_bin, 31, 0, 1, 1);
	grayLayout->addWidget(label_threshold, 32, 1, 1, 1);
	grayLayout->addWidget(line_bin, 32, 2, 1, 1);
	grayLayout->addWidget(button_bin, 32, 3, 1, 1);
	grayLayout->addWidget(buttonPre_bin, 32, 4, 1, 1);

	grayLayout->addWidget(null, 35, 0, 1, 1);
	grayLayout->addWidget(label_graylevel, 36, 0, 1, 1);
	grayLayout->addWidget(button_graylevel, 37, 1, 1, 4);

	grayLayout->addWidget(null, 40, 0, 1, 1);
	grayLayout->addWidget(label_reverse, 41, 0, 1, 1);
	grayLayout->addWidget(button_reverse, 42, 1, 1, 4);

	grayLayout->addWidget(null, 45, 0, 1, 1);
	grayLayout->addWidget(label_histeq, 46, 0, 1, 1);
	grayLayout->addWidget(button_histeq, 47, 1, 1, 4);

	QWidget* grayWidget = new QWidget(dock_Gray);
	grayWidget->setFixedWidth(420);
	grayWidget->setLayout(grayLayout);
	dock_Gray->setWidget(grayWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Gray);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(grayWidget);
	dock_Gray->setWidget(scrollArea);
}

// 图像增强窗口
void ImageProcessing::WinEnhance()
{
	dock_Enhance = new QDockWidget(tr("图像增强"), this);
	dock_Enhance->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QLabel* null = new QLabel(tr("	"));
	QLabel* label_smooth = new QLabel(tr("图像平滑"));
	QLabel* label_coreSmooth = new QLabel(tr("模板大小"));
	QPushButton* button_Normalized = new QPushButton(tr("简单滤波"));
	QPushButton* button_Gaussian = new QPushButton(tr("高斯滤波"));
	QPushButton* button_Median = new QPushButton(tr("中值滤波"));
	combo_smooth = new QComboBox();
	combo_smooth->addItem(tr("3×3")); combo_smooth->addItem(tr("5×5"));
	combo_smooth->addItem(tr("7×7")); combo_smooth->addItem(tr("9×9"));
	combo_smooth->addItem(tr("11×11")); combo_smooth->addItem(tr("13×13"));
	combo_smooth->addItem(tr("15×15")); combo_smooth->addItem(tr("17×17"));
	combo_smooth->addItem(tr("19×19")); combo_smooth->addItem(tr("21×21"));

	connect(button_Normalized, SIGNAL(clicked()), this, SLOT(normalizedImage()));
	connect(button_Gaussian, SIGNAL(clicked()), this, SLOT(gaussianImage()));
	connect(button_Median, SIGNAL(clicked()), this, SLOT(medianImage()));
	connect(combo_smooth, SIGNAL(currentIndexChanged(int)), this, SLOT(kernelSmoothChange()));

	QLabel* label_sharpen = new QLabel(tr("图像锐化"));
	QLabel* label_coreSharpen = new QLabel(tr("模板大小"));
	QPushButton* button_sobel = new QPushButton(tr("Sobel边缘检测"));
	QPushButton* button_laplacian = new QPushButton(tr("Laplacian检测"));
	QPushButton* button_canny = new QPushButton(tr("Canny边缘检测"));
	QPushButton* button_line = new QPushButton(tr("Hough线检测"));
	QPushButton* button_circle = new QPushButton(tr("Hough圆检测"));
	combo_sharpen = new QComboBox();
	combo_sharpen->addItem(tr("3×3")); combo_sharpen->addItem(tr("5×5"));
	combo_sharpen->addItem(tr("7×7")); combo_sharpen->addItem(tr("9×9"));
	combo_sharpen->addItem(tr("11×11")); combo_sharpen->addItem(tr("13×13"));
	combo_sharpen->addItem(tr("15×15")); combo_sharpen->addItem(tr("17×17"));
	combo_sharpen->addItem(tr("19×19")); combo_sharpen->addItem(tr("21×21"));

	QLabel* label_canny = new QLabel(tr("Canny阈值"));
	spinbox_canny = new QSpinBox;							// 微调框
	spinbox_canny->setRange(0, 100);
	spinbox_canny->setValue(20);

	QSlider* slider_canny = new QSlider(Qt::Horizontal);		// 滑动条
	slider_canny->setRange(0, 100);
	slider_canny->setValue(20);

	connect(button_sobel, SIGNAL(clicked()), this, SLOT(sobelImage()));
	connect(button_laplacian, SIGNAL(clicked()), this, SLOT(laplacianImage()));
	connect(button_canny, SIGNAL(clicked()), this, SLOT(cannyImage()));
	connect(button_line, SIGNAL(clicked()), this, SLOT(lineImage()));
	connect(button_circle, SIGNAL(clicked()), this, SLOT(circleImage()));
	connect(combo_sharpen, SIGNAL(currentIndexChanged(int)), this, SLOT(kernelSharpenChange()));

	connect(spinbox_canny, SIGNAL(valueChanged(int)), slider_canny, SLOT(setValue(int)));
	connect(slider_canny, SIGNAL(valueChanged(int)), spinbox_canny, SLOT(setValue(int)));
	connect(spinbox_canny, SIGNAL(valueChanged(int)), this, SLOT(cannyValueChange()));

	QLabel* label_lineThreshold = new QLabel(tr("线检测阈值"));
	spinbox_lineThreshold = new QSpinBox;						// HoughLine
	spinbox_lineThreshold->setRange(0, 500);
	spinbox_lineThreshold->setValue(50);
	QSlider* slider_lineThreshold = new QSlider(Qt::Horizontal);
	slider_lineThreshold->setRange(0, 500);
	slider_lineThreshold->setValue(50);
	connect(spinbox_lineThreshold, SIGNAL(valueChanged(int)), slider_lineThreshold, SLOT(setValue(int)));
	connect(slider_lineThreshold, SIGNAL(valueChanged(int)), spinbox_lineThreshold, SLOT(setValue(int)));

	QLabel* label_minLineLength = new QLabel(tr("线段最小长度"));
	spinbox_minLineLength = new QSpinBox;
	spinbox_minLineLength->setRange(0, 500);
	spinbox_minLineLength->setValue(50);
	QSlider* slider_minLineLength = new QSlider(Qt::Horizontal);
	slider_minLineLength->setRange(0, 500);
	slider_minLineLength->setValue(50);
	connect(spinbox_minLineLength, SIGNAL(valueChanged(int)), slider_minLineLength, SLOT(setValue(int)));
	connect(slider_minLineLength, SIGNAL(valueChanged(int)), spinbox_minLineLength, SLOT(setValue(int)));

	QLabel* label_maxLineGap = new QLabel(tr("允许最大间隔"));
	spinbox_maxLineGap = new QSpinBox;
	spinbox_maxLineGap->setRange(0, 500);
	spinbox_maxLineGap->setValue(10);
	QSlider* slider_maxLineGap = new QSlider(Qt::Horizontal);
	slider_maxLineGap->setRange(0, 500);
	slider_maxLineGap->setValue(50);
	connect(spinbox_maxLineGap, SIGNAL(valueChanged(int)), slider_maxLineGap, SLOT(setValue(int)));
	connect(slider_maxLineGap, SIGNAL(valueChanged(int)), spinbox_maxLineGap, SLOT(setValue(int)));

	QLabel* label_minRadius = new QLabel(tr("最小半径"));
	spinbox_minRadius = new QSpinBox;
	spinbox_minRadius->setRange(0, 1000);
	spinbox_minRadius->setValue(1);
	QSlider* slider_minRadius = new QSlider(Qt::Horizontal);
	slider_minRadius->setRange(0, 1000);
	slider_minRadius->setValue(1);
	connect(spinbox_minRadius, SIGNAL(valueChanged(int)), slider_minRadius, SLOT(setValue(int)));
	connect(slider_minRadius, SIGNAL(valueChanged(int)), spinbox_minRadius, SLOT(setValue(int)));

	QLabel* label_maxRadius = new QLabel(tr("最大半径"));
	spinbox_maxRadius = new QSpinBox;
	spinbox_maxRadius->setRange(0, 1000);
	spinbox_maxRadius->setValue(100);
	QSlider* slider_maxRadius = new QSlider(Qt::Horizontal);
	slider_maxRadius->setRange(0, 1000);
	slider_maxRadius->setValue(100);
	connect(spinbox_maxRadius, SIGNAL(valueChanged(int)), slider_maxRadius, SLOT(setValue(int)));
	connect(slider_maxRadius, SIGNAL(valueChanged(int)), spinbox_maxRadius, SLOT(setValue(int)));

	// 格栅布局
	QGridLayout* enhanceLayout = new QGridLayout();
	enhanceLayout->setAlignment(Qt::AlignTop);
	enhanceLayout->setMargin(30);

	enhanceLayout->addWidget(label_smooth, 0, 0, 1, 1);
	enhanceLayout->addWidget(label_coreSmooth, 1, 1, 1, 1);
	enhanceLayout->addWidget(combo_smooth, 1, 2, 1, 1);
	enhanceLayout->addWidget(button_Normalized, 2, 2, 1, 1);
	enhanceLayout->addWidget(button_Gaussian, 3, 2, 1, 1);
	enhanceLayout->addWidget(button_Median, 4, 2, 1, 1);

	enhanceLayout->addWidget(label_sharpen, 5, 0, 1, 1);
	enhanceLayout->addWidget(label_coreSharpen, 6, 1, 1, 1);
	enhanceLayout->addWidget(combo_sharpen, 6, 2, 1, 1);
	enhanceLayout->addWidget(button_sobel, 7, 2, 1, 1);
	enhanceLayout->addWidget(button_laplacian, 8, 2, 1, 1);
	enhanceLayout->addWidget(button_canny, 9, 2, 1, 1);

	enhanceLayout->addWidget(label_canny, 10, 1, 1, 1);
	enhanceLayout->addWidget(slider_canny, 10, 2, 1, 1);
	enhanceLayout->addWidget(spinbox_canny, 10, 3, 1, 1);

	enhanceLayout->addWidget(button_line, 11, 2, 1, 1);
	enhanceLayout->addWidget(label_lineThreshold, 12, 1, 1, 1);
	enhanceLayout->addWidget(slider_lineThreshold, 12, 2, 1, 1);
	enhanceLayout->addWidget(spinbox_lineThreshold, 12, 3, 1, 1);

	enhanceLayout->addWidget(label_minLineLength, 13, 1, 1, 1);
	enhanceLayout->addWidget(slider_minLineLength, 13, 2, 1, 1);
	enhanceLayout->addWidget(spinbox_minLineLength, 13, 3, 1, 1);

	enhanceLayout->addWidget(label_maxLineGap, 14, 1, 1, 1);
	enhanceLayout->addWidget(slider_maxLineGap, 14, 2, 1, 1);
	enhanceLayout->addWidget(spinbox_maxLineGap, 14, 3, 1, 1);

	enhanceLayout->addWidget(button_circle, 15, 2, 1, 1);
	enhanceLayout->addWidget(label_minRadius, 16, 1, 1, 1);
	enhanceLayout->addWidget(slider_minRadius, 16, 2, 1, 1);
	enhanceLayout->addWidget(spinbox_minRadius, 16, 3, 1, 1);

	enhanceLayout->addWidget(label_maxRadius, 17, 1, 1, 1);
	enhanceLayout->addWidget(slider_maxRadius, 17, 2, 1, 1);
	enhanceLayout->addWidget(spinbox_maxRadius, 17, 3, 1, 1);

	QWidget* enhanceWidget = new QWidget(dock_Enhance);
	enhanceWidget->setFixedWidth(420);
	enhanceWidget->setLayout(enhanceLayout);
	dock_Enhance->setWidget(enhanceWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Enhance);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(enhanceWidget);
	dock_Enhance->setWidget(scrollArea);
}

// 形态学处理窗口
void ImageProcessing::WinMorp()
{
	dock_Morp = new QDockWidget(tr("形态学处理"), this);
	dock_Morp->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QLabel* label_kernel = new QLabel();
	label_kernel->setText(tr("模板大小"));
	combo_kernel = new QComboBox();
	combo_kernel->addItem(tr("3×3")); combo_kernel->addItem(tr("5×5"));
	combo_kernel->addItem(tr("7×7")); combo_kernel->addItem(tr("9×9"));
	combo_kernel->addItem(tr("11×11")); combo_kernel->addItem(tr("13×13"));
	combo_kernel->addItem(tr("15×15")); combo_kernel->addItem(tr("17×17"));
	combo_kernel->addItem(tr("19×19")); combo_kernel->addItem(tr("21×21"));

	QLabel* label_elem = new QLabel();
	label_elem->setText(tr("结构元素"));
	combo_elem = new QComboBox();
	combo_elem->addItem(tr("矩形"));
	combo_elem->addItem(tr("十字形"));
	combo_elem->addItem(tr("椭圆形"));

	QLabel* label_erode = new QLabel();
	label_erode->setText(tr("腐蚀"));
	QLabel* label_erodeCount = new QLabel();
	label_erodeCount->setText(tr("次"));
	spinbox_erode = new QSpinBox;
	spinbox_erode->setRange(1, 50);
	spinbox_erode->setValue(1);
	QPushButton* button_erode = new QPushButton();			// 确定按钮
	button_erode->setText(tr("腐蚀"));

	QLabel* label_dilate = new QLabel();
	label_dilate->setText(tr("膨胀"));
	QLabel* label_dilateCount = new QLabel();
	label_dilateCount->setText(tr("次"));
	spinbox_dilate = new QSpinBox;
	spinbox_dilate->setRange(1, 50);
	spinbox_dilate->setValue(1);
	QPushButton* button_dilate = new QPushButton();			// 确定按钮
	button_dilate->setText(tr("膨胀"));

	QLabel* label_open = new QLabel();
	label_open->setText(tr("开运算"));
	QLabel* label_openCount = new QLabel();
	label_openCount->setText(tr("次"));
	spinbox_open = new QSpinBox;
	spinbox_open->setRange(1, 50);
	spinbox_open->setValue(1);
	QPushButton* button_open = new QPushButton();			// 确定按钮
	button_open->setText(tr("开运算"));

	QLabel* label_close = new QLabel();
	label_close->setText(tr("闭运算"));
	QLabel* label_closeCount = new QLabel();
	label_closeCount->setText(tr("次"));
	spinbox_close = new QSpinBox;
	spinbox_close->setRange(1, 50);
	spinbox_close->setValue(1);
	QPushButton* button_close = new QPushButton();			// 确定按钮
	button_close->setText(tr("闭运算"));

	QLabel* label_grad = new QLabel();
	label_grad->setText(tr("形态学梯度"));
	QPushButton* button_grad = new QPushButton();			// 确定按钮
	button_grad->setText(tr("形态学梯度"));

	QLabel* label_tophat = new QLabel();
	label_tophat->setText(tr("顶帽操作"));
	QPushButton* button_tophat = new QPushButton();			// 确定按钮
	button_tophat->setText(tr("顶帽操作"));

	QLabel* label_blackhat = new QLabel();
	label_blackhat->setText(tr("黑帽操作"));
	QPushButton* button_blackhat = new QPushButton();			// 确定按钮
	button_blackhat->setText(tr("黑帽操作"));

	connect(button_erode, SIGNAL(clicked()), this, SLOT(erodeImage()));
	connect(button_dilate, SIGNAL(clicked()), this, SLOT(dilateImage()));
	connect(button_open, SIGNAL(clicked()), this, SLOT(openImage()));
	connect(button_close, SIGNAL(clicked()), this, SLOT(closeImage()));
	connect(button_grad, SIGNAL(clicked()), this, SLOT(gradImage()));
	connect(button_tophat, SIGNAL(clicked()), this, SLOT(tophatImage()));
	connect(button_blackhat, SIGNAL(clicked()), this, SLOT(blackhatImage()));

	QGridLayout* morpLayout = new QGridLayout();
	morpLayout->setAlignment(Qt::AlignTop);
	morpLayout->setMargin(30);

	morpLayout->addWidget(label_kernel, 0, 1, 1, 1);
	morpLayout->addWidget(combo_kernel, 0, 2, 1, 1);
	morpLayout->addWidget(label_elem, 1, 1, 1, 1);
	morpLayout->addWidget(combo_elem, 1, 2, 1, 1);

	morpLayout->addWidget(label_erode, 2, 0, 1, 1);
	morpLayout->addWidget(button_erode, 3, 1, 1, 1);
	morpLayout->addWidget(spinbox_erode, 3, 2, 1, 1);
	morpLayout->addWidget(label_erodeCount, 3, 3, 1, 1);

	morpLayout->addWidget(label_dilate, 4, 0, 1, 1);
	morpLayout->addWidget(button_dilate, 5, 1, 1, 1);
	morpLayout->addWidget(spinbox_dilate, 5, 2, 1, 1);
	morpLayout->addWidget(label_dilateCount, 5, 3, 1, 1);

	morpLayout->addWidget(label_open, 6, 0, 1, 1);
	morpLayout->addWidget(button_open, 7, 1, 1, 1);
	morpLayout->addWidget(spinbox_open, 7, 2, 1, 1);
	morpLayout->addWidget(label_openCount, 7, 3, 1, 1);

	morpLayout->addWidget(label_close, 8, 0, 1, 1);
	morpLayout->addWidget(button_close, 9, 1, 1, 1);
	morpLayout->addWidget(spinbox_close, 9, 2, 1, 1);
	morpLayout->addWidget(label_closeCount, 9, 3, 1, 1);

	morpLayout->addWidget(label_tophat, 10, 0, 1, 1);
	morpLayout->addWidget(button_tophat, 11, 1, 1, 1);
	morpLayout->addWidget(label_blackhat, 12, 0, 1, 1);
	morpLayout->addWidget(button_blackhat, 13, 1, 1, 1);
	morpLayout->addWidget(label_grad, 14, 0, 1, 1);
	morpLayout->addWidget(button_grad, 15, 1, 1, 1);

	QWidget* morpWidget = new QWidget(dock_Morp);
	morpWidget->setFixedWidth(420);
	morpWidget->setLayout(morpLayout);
	dock_Morp->setWidget(morpWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Morp);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(morpWidget);
	dock_Morp->setWidget(scrollArea);
}

//图像调整窗口
void ImageProcessing::WinAdjust()
{
	dock_Adjust = new QDockWidget(tr("图像调整"), this);
	dock_Adjust->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QLabel* null = new QLabel(tr("	"));

	QLabel* label_contrast = new QLabel(tr("对比度"));
	QSpinBox* SpinBox_contrast = new QSpinBox(this);
	SpinBox_contrast->setMinimum(-100);  // 最小值
	SpinBox_contrast->setMaximum(100);  // 最大值
	SpinBox_contrast->setSingleStep(1);  // 步长
	QSlider* Slider_contrast = new QSlider(this);
	Slider_contrast->setOrientation(Qt::Horizontal);  // 水平方向
	Slider_contrast->setMinimum(-100);  // 最小值
	Slider_contrast->setMaximum(100);  // 最大值
	Slider_contrast->setSingleStep(1);  // 步长
	connect(SpinBox_contrast, SIGNAL(valueChanged(int)), Slider_contrast, SLOT(setValue(int)));
	connect(SpinBox_contrast, SIGNAL(valueChanged(int)), this, SLOT(adjustContrastImage(int)));
	connect(Slider_contrast, SIGNAL(valueChanged(int)), SpinBox_contrast, SLOT(setValue(int)));
	SpinBox_contrast->setValue(0);

	QLabel* label_brightness = new QLabel(tr("亮度"));
	QSpinBox* SpinBox_brightness = new QSpinBox(this);
	SpinBox_brightness->setMinimum(-100);  // 最小值
	SpinBox_brightness->setMaximum(100);  // 最大值
	SpinBox_brightness->setSingleStep(1);  // 步长
	QSlider* Slider_brightness = new QSlider(this);
	Slider_brightness->setOrientation(Qt::Horizontal);  // 水平方向
	Slider_brightness->setMinimum(-100);  // 最小值
	Slider_brightness->setMaximum(100);  // 最大值
	Slider_brightness->setSingleStep(1);  // 步长
	connect(SpinBox_brightness, SIGNAL(valueChanged(int)), Slider_brightness, SLOT(setValue(int)));
	connect(SpinBox_brightness, SIGNAL(valueChanged(int)), this, SLOT(adjustBrightnessImage(int)));
	connect(Slider_brightness, SIGNAL(valueChanged(int)), SpinBox_brightness, SLOT(setValue(int)));
	SpinBox_brightness->setValue(0);


	QGridLayout* adjustLayout = new QGridLayout();
	adjustLayout->setAlignment(Qt::AlignTop);
	adjustLayout->setMargin(30);

	adjustLayout->addWidget(label_contrast, 0, 0, 1, 1);
	adjustLayout->addWidget(SpinBox_contrast, 1, 1, 1, 1);
	adjustLayout->addWidget(Slider_contrast, 1, 2, 1, 1);

	adjustLayout->addWidget(null, 5, 0, 1, 1);
	adjustLayout->addWidget(label_brightness, 6, 0, 1, 1);
	adjustLayout->addWidget(SpinBox_brightness, 7, 1, 1, 1);
	adjustLayout->addWidget(Slider_brightness, 7, 2, 1, 1);


	QWidget* adjustWidget = new QWidget(dock_Adjust);
	adjustWidget->setFixedWidth(420);
	adjustWidget->setLayout(adjustLayout);
	dock_Adjust->setWidget(adjustWidget);

	QScrollArea* scrollArea = new QScrollArea(dock_Adjust);
	scrollArea->setAlignment(Qt::AlignLeft);
	scrollArea->setWidget(adjustWidget);
	dock_Adjust->setWidget(scrollArea);
}
