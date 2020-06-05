#ifndef CREATEMENU_H
#define CREATEMENU_H

#include <QtGui>
#include <QtWidgets>
#include <QMenuBar>
#include <QMainWindow>

#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/opencv.hpp"    
#include <opencv2/core/core.hpp> 

#include "header/MainWindow.h"
#include "header/CreateMenu.h"
#include "header/ImgChange.h" 
#include "header/CustomWindow.h"
class ImageProcessing;

using namespace cv;


class Menu : public QMenuBar
{
	Q_OBJECT
public:
	Menu(QWidget *parent, ImageProcessing *p);
	~Menu();

	CustomWindow *helpWin;
	QAction *Act_edit_undo, *Act_edit_redo;
	void setMenuQSS(bool);		// ���ö����˵���QSS

private slots :
	// ����Ŀ¼
	void File_new();		// �½�
	void File_new1();
	void File_new2();
	void File_new3();
	void File_open();		// ��
	void File_save();		// ����
	void File_saveas();		// ���Ϊ

private:
	ImageProcessing *I_MainWindow;
	QToolBar *I_ToolBar;
	QString currentPath;	// ��ǰͼ��·��
	void setSubMenu(QMenu *menu, bool flag);		// ����qss

	void Menu_File();		// �ļ��˵�
	void Menu_Edit();		// �༭�˵�
	void Menu_Geom();		// ���α任
	void Menu_Gray();		// �Ҷȱ任
	void Menu_Enhance();	// ͼ����ǿ
	void Menu_Morp();		// ��̬ѧ
	void Menu_Detection();	// ʶ��ģ��

	// �����˵�
	QMenu *menu_resize, *menu_rotate, *menu_flip, *menu_nolinear, *menu_detect, *menu_smooth, *menu_sharpen;
	QMenu *menu_RGB, *menu_HSV, *menu_YUV, *menu_HLS;
	QMenu *menu_frostedglass,*menu_sketch;

};

#endif
