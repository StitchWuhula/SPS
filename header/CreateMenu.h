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
	void setMenuQSS(bool);		// 设置二级菜单的QSS

private slots :
	// 界面目录
	void File_new();		// 新建
	void File_new1();
	void File_new2();
	void File_new3();
	void File_open();		// 打开
	void File_save();		// 保存
	void File_saveas();		// 另存为

private:
	ImageProcessing *I_MainWindow;
	QToolBar *I_ToolBar;
	QString currentPath;	// 当前图像路径
	void setSubMenu(QMenu *menu, bool flag);		// 设置qss

	void Menu_File();		// 文件菜单
	void Menu_Edit();		// 编辑菜单
	void Menu_Geom();		// 几何变换
	void Menu_Gray();		// 灰度变换
	void Menu_Enhance();	// 图像增强
	void Menu_Morp();		// 形态学
	void Menu_Detection();	// 识别模型

	// 二级菜单
	QMenu *menu_resize, *menu_rotate, *menu_flip, *menu_nolinear, *menu_detect, *menu_smooth, *menu_sharpen;
	QMenu *menu_RGB, *menu_HSV, *menu_YUV, *menu_HLS;
	QMenu *menu_frostedglass,*menu_sketch;

};

#endif
