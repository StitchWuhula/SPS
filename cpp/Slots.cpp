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

// 撤销
void ImageProcessing::undo()
{
	if (iter != imgVector[0].begin())			// 前面还有对象
	{
		iter--;
		ShowImage(*iter, false);
		I_menubar->Act_edit_redo->setEnabled(true);

		if (iter == imgVector[0].begin())		// 判断自减后是否为初始图像
		{
			I_menubar->Act_edit_undo->setEnabled(false);
		}
	}
}

// 重做															
void ImageProcessing::redo()
{
	if (iter != (imgVector[0].end() - 1))		// 后面还有对象
	{
		iter++;
		ShowImage(*iter, false);
		I_menubar->Act_edit_undo->setEnabled(true);

		if (iter == (imgVector[0].end() - 1))
		{
			I_menubar->Act_edit_redo->setEnabled(false);
		}
	}
}

// 退出全屏
void ImageProcessing::maxScreen()
{
	showMaximized();
}

// 全屏显示
void ImageProcessing::fullScreen()
{
	showFullScreen();
}

// 白色布局
void ImageProcessing::whiteLayout()
{
	this->setStyleSheet(stylesheet);
	preWin->setStyleSheet(stylesheet);
	I_menubar->setMenuQSS(true);
	isSkin = true;
}


// 图像铺满窗口
void ImageProcessing::fullSize()
{
	QImage Img = imgLabel->getImage().scaled(ImgscrollArea->width() - 2, ImgscrollArea->height() - 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ShowImage(Img, true);
}

// 图像自适应窗口大小
void ImageProcessing::autoSize()
{
	QImage Img;
	double ImgRatio = 1.0 * imgLabel->getImage().width() / imgLabel->getImage().height();
	double WinRatio = 1.0 * (ImgscrollArea->width() - 2) / (ImgscrollArea->height() - 2);
	if (ImgRatio > WinRatio)
	{
		Img = imgLabel->getImage().scaled((ImgscrollArea->width() - 2), (ImgscrollArea->width() - 2) / ImgRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	else
	{
		Img = imgLabel->getImage().scaled((ImgscrollArea->height() - 2) * ImgRatio, (ImgscrollArea->height() - 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	//QImage Img = imgLabel->getImage().scaled(ImgscrollArea->width() - 2, ImgscrollArea->height() - 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ShowImage(Img, true);
}

// 改变大小
void ImageProcessing::changeImageSize()
{
	QImage Img = geomClass->Resize(imgLabel->getImage(), line_L->text().toInt(), line_W->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 默认大小
void ImageProcessing::defaultImageSize()
{
	QImage Img = geomClass->Resize(imgLabel->getImage(), 512, 512);
	ShowImage(Img, true);	// 图像保存
}

// 确认缩放
void ImageProcessing::enlargeReduceOK()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), line_times->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 图像放大2倍
void ImageProcessing::enlarge2()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), 1);
	ShowImage(Img, true);	// 图像保存			
}

// 图像缩小2倍
void ImageProcessing::reduce2()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), -1);
	ShowImage(Img, true);	// 图像保存				
}

// 确认旋转
void ImageProcessing::rotateImageOK()
{
	QImage Img;
	if (line_degree->text().toInt() == 90 || line_degree->text().toInt() == -270)
		Img = geomClass->Rotate_fixed(imgLabel->getImage(), 90);
	else if (line_degree->text().toInt() == 180 || line_degree->text().toInt() == -180)
		Img = geomClass->Rotate_fixed(imgLabel->getImage(), 180);
	else if (line_degree->text().toInt() == 270 || line_degree->text().toInt() == -90)
		Img = geomClass->Rotate_fixed(imgLabel->getImage(), 270);
	else
		Img = geomClass->Rotate(imgLabel->getImage(), 0 - line_degree->text().toInt());

	ShowImage(Img, true);	// 图像保存	
}

// 图像旋转90
void ImageProcessing::rotateImage_90()
{
	QImage Img = geomClass->Rotate_fixed(imgLabel->getImage(), 90);
	ShowImage(Img, true);	// 图像保存	
}

// 图像旋转180
void ImageProcessing::rotateImage_180()
{
	QImage Img = geomClass->Rotate_fixed(imgLabel->getImage(), 180);
	ShowImage(Img, true);	// 图像保存	
	spinbox_rotate->setValue(0);
}

// 图像旋转270
void ImageProcessing::rotateImage_270()
{
	QImage Img = geomClass->Rotate_fixed(imgLabel->getImage(), 270);
	ShowImage(Img, true);	// 图像保存	
	spinbox_rotate->setValue(0);
}

// 水平镜像
void ImageProcessing::flipImage_H()
{
	QImage Img = geomClass->Flip(imgLabel->getImage(), 1);
	ShowImage(Img, true);	// 图像保存	
}

// 垂直镜像
void ImageProcessing::flipImage_V()
{
	QImage Img = geomClass->Flip(imgLabel->getImage(), 0);
	ShowImage(Img, true);	// 图像保存	
}

// 图像倾斜
void ImageProcessing::LeanImageOK()
{
	QImage Img = geomClass->Lean(imgLabel->getImage(), line_leanX->text().toInt(), line_leanY->text().toInt());
	ShowImage(Img, true);
}

// 确认二值图像
void ImageProcessing::binImageOK()
{
	QImage Img = grayClass->Bin(imgLabel->getImage(), line_bin->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 灰度图像
void ImageProcessing::grayImage()
{
	QImage Img = grayClass->Graylevel(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}

// 图像反转
void ImageProcessing::reverseImage()
{
	QImage Img = grayClass->Reverse(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}

// 确认线性变换
void ImageProcessing::linearImageOK()
{
	QImage Img = grayClass->Linear(imgLabel->getImage(), line_alpha->text().toInt(), line_beta->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 确认对数变换
void ImageProcessing::logImageOK()
{
	QImage Img = grayClass->Log(imgLabel->getImage(), line_c->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 确认伽马变换
void ImageProcessing::gammaImageOK()
{
	QImage Img = grayClass->Gamma(imgLabel->getImage(), line_gamma->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 直方图均衡化
void ImageProcessing::histeqImage()
{
	QImage Img = grayClass->Histeq(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}

// 简单滤波
void ImageProcessing::normalizedImage()
{
	QImage Img = enhanceClass->Normalized(imgLabel->getImage(), 2 * (combo_smooth->currentIndex()) + 3);
	ShowImage(Img, true);	// 图像保存
}

// 高斯滤波
void ImageProcessing::gaussianImage()
{
	QImage Img = enhanceClass->Gaussian(imgLabel->getImage(), 2 * (combo_smooth->currentIndex()) + 3);
	ShowImage(Img, true);	// 图像保存
}

// 中值滤波
void ImageProcessing::medianImage()
{
	QImage Img = enhanceClass->Median(imgLabel->getImage(), 2 * (combo_smooth->currentIndex()) + 3);
	ShowImage(Img, true);	// 图像保存
}

// sobel边缘检测
void ImageProcessing::sobelImage()
{
	QImage Img = enhanceClass->Sobel(imgLabel->getImage(), 2 * (combo_sharpen->currentIndex()) + 3);
	ShowImage(Img, true);	// 图像保存
}

// laplacian边缘检测
void ImageProcessing::laplacianImage()
{
	QImage Img = enhanceClass->Laplacian(imgLabel->getImage(), 2 * (combo_sharpen->currentIndex()) + 3);
	ShowImage(Img, true);	// 图像保存
}

// canny边缘检测
void ImageProcessing::cannyImage()
{
	QImage Img = enhanceClass->Canny(imgLabel->getImage(), 2 * (combo_sharpen->currentIndex()) + 3, spinbox_canny->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// line检测
void ImageProcessing::lineImage()
{
	QImage Img = enhanceClass->HoughLine(imgLabel->getImage(), spinbox_lineThreshold->text().toInt(), spinbox_minLineLength->text().toInt(), spinbox_maxLineGap->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// circle检测
void ImageProcessing::circleImage()
{
	if (spinbox_minRadius->text().toInt() > spinbox_maxRadius->text().toInt())
	{
		QMessageBox::information(this, tr("输入错误"), tr("最大半径应大于等于最小半径!"));
	}
	else
	{
		QImage Img = enhanceClass->HoughCircle(imgLabel->getImage(), spinbox_minRadius->text().toInt(), spinbox_maxRadius->text().toInt());
		ShowImage(Img, true);	// 图像保存
	}
}

// 腐蚀
void ImageProcessing::erodeImage()
{
	QImage Img = morpClass->Erode(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_erode->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 膨胀
void ImageProcessing::dilateImage()
{
	QImage Img = morpClass->Dilate(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_dilate->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 开运算
void ImageProcessing::openImage()
{
	QImage Img = morpClass->Open(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_open->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 闭运算
void ImageProcessing::closeImage()
{
	QImage Img = morpClass->Close(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_close->text().toInt());
	ShowImage(Img, true);	// 图像保存
}

// 形态学梯度
void ImageProcessing::gradImage()
{
	QImage Img = morpClass->Grad(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex());
	ShowImage(Img, true);	// 图像保存
}

// 顶帽操作
void ImageProcessing::tophatImage()
{
	QImage Img = morpClass->Tophat(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex());
	ShowImage(Img, true);	// 图像保存
}

// 黑帽操作
void ImageProcessing::blackhatImage()
{
	QImage Img = morpClass->Blackhat(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex());
	ShowImage(Img, true);	// 图像保存
}

void ImageProcessing::frostedglassImage()
{
	QImage Img = filterClass->frostedglass(imgLabel->getMat());
	ShowImage(Img, true);
}

void ImageProcessing::sketchImage()
{
	QImage Img = filterClass->sketch(imgLabel->getMat());
	ShowImage(Img, true);
}

void ImageProcessing::embossImage()
{
	QImage Img = filterClass->emboss(imgLabel->getMat());
	ShowImage(Img, true);
}

void ImageProcessing::glareImage()
{
	QImage Img = filterClass->glare(imgLabel->getMat());
	ShowImage(Img, true);
}

void ImageProcessing::adjustContrastImage(int rate)
{
	float alpha = 1.0 + rate / 200.0;
	QImage Img = adjustClass->contrastAjust(imgLabel->getMat(), alpha);
	ShowImage(Img, true);
}

void ImageProcessing::adjustBrightnessImage(int rate)
{
	float beta = rate;
	QImage Img = adjustClass->lightAdjust(imgLabel->getMat(), beta);
	ShowImage(Img, true);
}

// 绘图槽函数ButtonGroup
void ImageProcessing::toolButtonClicked(int id)
{
	QList<QAbstractButton*> buttons = toolButtonGroup->buttons();
	foreach(QAbstractButton * button, buttons)
	{
		if (toolButtonGroup->button(id) != button)
		{
			button->setChecked(false);
			button->setStyleSheet("{background-color: transparent}");	// 透明
		}
		else if (drawType == id)
		{
			drawType = 0;
			button->setStyleSheet("{background-color: transparent}");	// 透明
		}
		else
		{
			drawType = id;
			button->setStyleSheet("background-color: rgb(76,235,255)");// 青蓝色
		}
	}

	switch (drawType)
	{
	case 0:
		imgLabel->setShape(PaintWidget::Null);
		ShowImage(imgLabel->getImage(), true);
		break;
	case 1:								// 首个图标按钮
		imgLabel->setShape(PaintWidget::Pen);
		break;
	case 2:
		imgLabel->setShape(PaintWidget::Line);
		break;
	case 3:
		imgLabel->setShape(PaintWidget::Ellipse);
		break;
	case 4:
		imgLabel->setShape(PaintWidget::Circle);
		break;
	case 5:
		imgLabel->setShape(PaintWidget::Triangle);
		break;
	case 6:
		imgLabel->setShape(PaintWidget::Rhombus);
		break;
	case 7:
		imgLabel->setShape(PaintWidget::Rect);
		break;
	case 8:
		imgLabel->setShape(PaintWidget::Square);
		break;
	case 9:
		imgLabel->setShape(PaintWidget::Hexagon);
		break;
	case 10:
		imgLabel->setShape(PaintWidget::Text);
		break;
	default:
		break;
	}
}

// 预览窗口槽函数
void ImageProcessing::preButtonClicked(int id)
{
	// 重新赋值
	preLabel->setImage(imgLabel->getImage());
	spinbox_size->setValue(line_times->text().toInt());
	spinbox_rotate->setValue(line_degree->text().toInt());
	spinbox_leanX->setValue(line_leanX->text().toInt());
	spinbox_leanY->setValue(line_leanY->text().toInt());
	spinbox_bin->setValue(line_bin->text().toInt());
	spinbox_alpha->setValue(line_alpha->text().toInt());
	spinbox_beta->setValue(line_beta->text().toInt());
	spinbox_c->setValue(line_c->text().toInt());
	spinbox_gamma->setValue(line_gamma->text().toInt());

	for (int i = 0; i < 7; i++)
	{
		if (i != id)
			widget_pre[i]->hide();
		else
			widget_pre[i]->show();
	}
	preWin->exec();
}

// 设置画笔线宽
void ImageProcessing::penWidth()
{
	imgLabel->setPenWidth(spinbox_penWidth->text().toInt());
}

// 设置画笔颜色
void ImageProcessing::penColor()
{
	QColor c = QColorDialog::getColor(Qt::blue);
	if (c.isValid())
	{
		frame_color->setPalette(QPalette(c));
		imgLabel->setPenColor(c);
		frame_color->setStyleSheet("QFrame{background-color: rgba(" + QString::number(c.red()) + "," + QString::number(c.green()) + "," + QString::number(c.blue()) + ",1);border:none}");
	}
}

// ----------------------------------------------------------------------预览窗口槽函数-------------------------------------------------------------------------------------

// 图像缩放（预览）
void ImageProcessing::enlargeReduce()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), spinbox_size->text().toInt());
	PreviewImage(Img);
}

// 图像旋转（预览）
void ImageProcessing::rotateImage()
{
	QImage Img = geomClass->Rotate(imgLabel->getImage(), 0 - spinbox_rotate->text().toInt());
	PreviewImage(Img);
}

// 图像倾斜（预览）
void ImageProcessing::LeanImage()
{
	QImage Img = geomClass->Lean(imgLabel->getImage(), spinbox_leanX->text().toInt(), spinbox_leanY->text().toInt());
	PreviewImage(Img);
}

// 二值图像（预览）
void ImageProcessing::binImage()
{
	QImage Img = grayClass->Bin(imgLabel->getImage(), spinbox_bin->text().toInt());
	PreviewImage(Img);
}

// 线性变换（预览）
void ImageProcessing::linearImage()
{
	QImage Img = grayClass->Linear(imgLabel->getImage(), spinbox_alpha->text().toInt(), spinbox_beta->text().toInt());
	PreviewImage(Img);
}

// 对数变换（预览）
void ImageProcessing::logImage()
{
	QImage Img = grayClass->Log(imgLabel->getImage(), spinbox_c->text().toInt());
	PreviewImage(Img);
}

// 伽马变换（预览）
void ImageProcessing::gammaImage()
{
	QImage Img = grayClass->Gamma(imgLabel->getImage(), spinbox_gamma->text().toInt());
	PreviewImage(Img);
}

void ImageProcessing::faceDetection()
{
	QImage Img = detectionClass->FaceDetection(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}

void ImageProcessing::personDetection()
{
	QImage Img = detectionClass->PersonDetection(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}

void ImageProcessing::beautifulDetection()
{
	QImage Img = detectionClass->BeautifulDetection(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}

void ImageProcessing::edgeDetection()
{
	QImage Img = detectionClass->EdgeDetection(imgLabel->getImage());
	ShowImage(Img, true);	// 图像保存
}