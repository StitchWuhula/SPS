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

// ����
void ImageProcessing::undo()
{
	if (iter != imgVector[0].begin())			// ǰ�滹�ж���
	{
		iter--;
		ShowImage(*iter, false);
		I_menubar->Act_edit_redo->setEnabled(true);

		if (iter == imgVector[0].begin())		// �ж��Լ����Ƿ�Ϊ��ʼͼ��
		{
			I_menubar->Act_edit_undo->setEnabled(false);
		}
	}
}

// ����															
void ImageProcessing::redo()
{
	if (iter != (imgVector[0].end() - 1))		// ���滹�ж���
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

// �˳�ȫ��
void ImageProcessing::maxScreen()
{
	showMaximized();
}

// ȫ����ʾ
void ImageProcessing::fullScreen()
{
	showFullScreen();
}

// ��ɫ����
void ImageProcessing::whiteLayout()
{
	this->setStyleSheet(stylesheet);
	preWin->setStyleSheet(stylesheet);
	I_menubar->setMenuQSS(true);
	isSkin = true;
}


// ͼ����������
void ImageProcessing::fullSize()
{
	QImage Img = imgLabel->getImage().scaled(ImgscrollArea->width() - 2, ImgscrollArea->height() - 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ShowImage(Img, true);
}

// ͼ������Ӧ���ڴ�С
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

// �ı��С
void ImageProcessing::changeImageSize()
{
	QImage Img = geomClass->Resize(imgLabel->getImage(), line_L->text().toInt(), line_W->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// Ĭ�ϴ�С
void ImageProcessing::defaultImageSize()
{
	QImage Img = geomClass->Resize(imgLabel->getImage(), 512, 512);
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ȷ������
void ImageProcessing::enlargeReduceOK()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), line_times->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ͼ��Ŵ�2��
void ImageProcessing::enlarge2()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), 1);
	ShowImage(Img, true);	// ͼ�񱣴�			
}

// ͼ����С2��
void ImageProcessing::reduce2()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), -1);
	ShowImage(Img, true);	// ͼ�񱣴�				
}

// ȷ����ת
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

	ShowImage(Img, true);	// ͼ�񱣴�	
}

// ͼ����ת90
void ImageProcessing::rotateImage_90()
{
	QImage Img = geomClass->Rotate_fixed(imgLabel->getImage(), 90);
	ShowImage(Img, true);	// ͼ�񱣴�	
}

// ͼ����ת180
void ImageProcessing::rotateImage_180()
{
	QImage Img = geomClass->Rotate_fixed(imgLabel->getImage(), 180);
	ShowImage(Img, true);	// ͼ�񱣴�	
	spinbox_rotate->setValue(0);
}

// ͼ����ת270
void ImageProcessing::rotateImage_270()
{
	QImage Img = geomClass->Rotate_fixed(imgLabel->getImage(), 270);
	ShowImage(Img, true);	// ͼ�񱣴�	
	spinbox_rotate->setValue(0);
}

// ˮƽ����
void ImageProcessing::flipImage_H()
{
	QImage Img = geomClass->Flip(imgLabel->getImage(), 1);
	ShowImage(Img, true);	// ͼ�񱣴�	
}

// ��ֱ����
void ImageProcessing::flipImage_V()
{
	QImage Img = geomClass->Flip(imgLabel->getImage(), 0);
	ShowImage(Img, true);	// ͼ�񱣴�	
}

// ͼ����б
void ImageProcessing::LeanImageOK()
{
	QImage Img = geomClass->Lean(imgLabel->getImage(), line_leanX->text().toInt(), line_leanY->text().toInt());
	ShowImage(Img, true);
}

// ȷ�϶�ֵͼ��
void ImageProcessing::binImageOK()
{
	QImage Img = grayClass->Bin(imgLabel->getImage(), line_bin->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// �Ҷ�ͼ��
void ImageProcessing::grayImage()
{
	QImage Img = grayClass->Graylevel(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ͼ��ת
void ImageProcessing::reverseImage()
{
	QImage Img = grayClass->Reverse(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ȷ�����Ա任
void ImageProcessing::linearImageOK()
{
	QImage Img = grayClass->Linear(imgLabel->getImage(), line_alpha->text().toInt(), line_beta->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ȷ�϶����任
void ImageProcessing::logImageOK()
{
	QImage Img = grayClass->Log(imgLabel->getImage(), line_c->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ȷ��٤��任
void ImageProcessing::gammaImageOK()
{
	QImage Img = grayClass->Gamma(imgLabel->getImage(), line_gamma->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ֱ��ͼ���⻯
void ImageProcessing::histeqImage()
{
	QImage Img = grayClass->Histeq(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ���˲�
void ImageProcessing::normalizedImage()
{
	QImage Img = enhanceClass->Normalized(imgLabel->getImage(), 2 * (combo_smooth->currentIndex()) + 3);
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ��˹�˲�
void ImageProcessing::gaussianImage()
{
	QImage Img = enhanceClass->Gaussian(imgLabel->getImage(), 2 * (combo_smooth->currentIndex()) + 3);
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ��ֵ�˲�
void ImageProcessing::medianImage()
{
	QImage Img = enhanceClass->Median(imgLabel->getImage(), 2 * (combo_smooth->currentIndex()) + 3);
	ShowImage(Img, true);	// ͼ�񱣴�
}

// sobel��Ե���
void ImageProcessing::sobelImage()
{
	QImage Img = enhanceClass->Sobel(imgLabel->getImage(), 2 * (combo_sharpen->currentIndex()) + 3);
	ShowImage(Img, true);	// ͼ�񱣴�
}

// laplacian��Ե���
void ImageProcessing::laplacianImage()
{
	QImage Img = enhanceClass->Laplacian(imgLabel->getImage(), 2 * (combo_sharpen->currentIndex()) + 3);
	ShowImage(Img, true);	// ͼ�񱣴�
}

// canny��Ե���
void ImageProcessing::cannyImage()
{
	QImage Img = enhanceClass->Canny(imgLabel->getImage(), 2 * (combo_sharpen->currentIndex()) + 3, spinbox_canny->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// line���
void ImageProcessing::lineImage()
{
	QImage Img = enhanceClass->HoughLine(imgLabel->getImage(), spinbox_lineThreshold->text().toInt(), spinbox_minLineLength->text().toInt(), spinbox_maxLineGap->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// circle���
void ImageProcessing::circleImage()
{
	if (spinbox_minRadius->text().toInt() > spinbox_maxRadius->text().toInt())
	{
		QMessageBox::information(this, tr("�������"), tr("���뾶Ӧ���ڵ�����С�뾶!"));
	}
	else
	{
		QImage Img = enhanceClass->HoughCircle(imgLabel->getImage(), spinbox_minRadius->text().toInt(), spinbox_maxRadius->text().toInt());
		ShowImage(Img, true);	// ͼ�񱣴�
	}
}

// ��ʴ
void ImageProcessing::erodeImage()
{
	QImage Img = morpClass->Erode(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_erode->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ����
void ImageProcessing::dilateImage()
{
	QImage Img = morpClass->Dilate(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_dilate->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ������
void ImageProcessing::openImage()
{
	QImage Img = morpClass->Open(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_open->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ������
void ImageProcessing::closeImage()
{
	QImage Img = morpClass->Close(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex(), spinbox_close->text().toInt());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ��̬ѧ�ݶ�
void ImageProcessing::gradImage()
{
	QImage Img = morpClass->Grad(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ��ñ����
void ImageProcessing::tophatImage()
{
	QImage Img = morpClass->Tophat(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex());
	ShowImage(Img, true);	// ͼ�񱣴�
}

// ��ñ����
void ImageProcessing::blackhatImage()
{
	QImage Img = morpClass->Blackhat(imgLabel->getImage(), combo_elem->currentIndex(), combo_kernel->currentIndex());
	ShowImage(Img, true);	// ͼ�񱣴�
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

// ��ͼ�ۺ���ButtonGroup
void ImageProcessing::toolButtonClicked(int id)
{
	QList<QAbstractButton*> buttons = toolButtonGroup->buttons();
	foreach(QAbstractButton * button, buttons)
	{
		if (toolButtonGroup->button(id) != button)
		{
			button->setChecked(false);
			button->setStyleSheet("{background-color: transparent}");	// ͸��
		}
		else if (drawType == id)
		{
			drawType = 0;
			button->setStyleSheet("{background-color: transparent}");	// ͸��
		}
		else
		{
			drawType = id;
			button->setStyleSheet("background-color: rgb(76,235,255)");// ����ɫ
		}
	}

	switch (drawType)
	{
	case 0:
		imgLabel->setShape(PaintWidget::Null);
		ShowImage(imgLabel->getImage(), true);
		break;
	case 1:								// �׸�ͼ�갴ť
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

// Ԥ�����ڲۺ���
void ImageProcessing::preButtonClicked(int id)
{
	// ���¸�ֵ
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

// ���û����߿�
void ImageProcessing::penWidth()
{
	imgLabel->setPenWidth(spinbox_penWidth->text().toInt());
}

// ���û�����ɫ
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

// ----------------------------------------------------------------------Ԥ�����ڲۺ���-------------------------------------------------------------------------------------

// ͼ�����ţ�Ԥ����
void ImageProcessing::enlargeReduce()
{
	QImage Img = geomClass->Enlarge_Reduce(imgLabel->getImage(), spinbox_size->text().toInt());
	PreviewImage(Img);
}

// ͼ����ת��Ԥ����
void ImageProcessing::rotateImage()
{
	QImage Img = geomClass->Rotate(imgLabel->getImage(), 0 - spinbox_rotate->text().toInt());
	PreviewImage(Img);
}

// ͼ����б��Ԥ����
void ImageProcessing::LeanImage()
{
	QImage Img = geomClass->Lean(imgLabel->getImage(), spinbox_leanX->text().toInt(), spinbox_leanY->text().toInt());
	PreviewImage(Img);
}

// ��ֵͼ��Ԥ����
void ImageProcessing::binImage()
{
	QImage Img = grayClass->Bin(imgLabel->getImage(), spinbox_bin->text().toInt());
	PreviewImage(Img);
}

// ���Ա任��Ԥ����
void ImageProcessing::linearImage()
{
	QImage Img = grayClass->Linear(imgLabel->getImage(), spinbox_alpha->text().toInt(), spinbox_beta->text().toInt());
	PreviewImage(Img);
}

// �����任��Ԥ����
void ImageProcessing::logImage()
{
	QImage Img = grayClass->Log(imgLabel->getImage(), spinbox_c->text().toInt());
	PreviewImage(Img);
}

// ٤��任��Ԥ����
void ImageProcessing::gammaImage()
{
	QImage Img = grayClass->Gamma(imgLabel->getImage(), spinbox_gamma->text().toInt());
	PreviewImage(Img);
}

void ImageProcessing::faceDetection()
{
	QImage Img = detectionClass->FaceDetection(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}

void ImageProcessing::personDetection()
{
	QImage Img = detectionClass->PersonDetection(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}

void ImageProcessing::beautifulDetection()
{
	QImage Img = detectionClass->BeautifulDetection(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}

void ImageProcessing::edgeDetection()
{
	QImage Img = detectionClass->EdgeDetection(imgLabel->getImage());
	ShowImage(Img, true);	// ͼ�񱣴�
}