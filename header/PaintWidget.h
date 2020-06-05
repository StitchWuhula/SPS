#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>

#include "opencv2/highgui/highgui.hpp"    
#include "opencv2//opencv.hpp"    
#include "opencv2//core//core.hpp"  

class PaintWidget :public QLabel
{
	Q_OBJECT
public:
	explicit PaintWidget(QWidget *parent = 0);
	enum shape {
		Pen = 1,Line,Ellipse,Circle, Triangle, Rhombus, 
		Rect, Square, Hexagon, Text,Null
	};
	void paint(QImage &theImage);
	void setImage(QImage img);
	QImage getImage(); // �ⲿ��ȡͼ��
	void setMat(cv::Mat &raw);
	cv::Mat getMat();

	void setShape(PaintWidget::shape ); 					// ���û�������
	void setPenWidth(int w);								// ���û��ʿ��
	void setPenColor(QColor c);								// ���û�����ɫ
	void textChange(QString font, QString text, int size);
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	PaintWidget::shape type;
	int penWidth; 
	QColor penColor;
	QImage image;
	QImage tempImage;
	QPoint lastPoint;
	QPoint endPoint;
	bool isDrawing;
	cv::Mat src;
	QString FONT;
	QString TEXT;
	int SIZE;
};

#endif// !PAINTWIDGET