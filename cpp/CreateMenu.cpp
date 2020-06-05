#pragma execution_character_set("utf-8")		// ���ı���


//#include <QMainWindow>
//#include "ui_ImageProcessing.h"
#include "header/MainWindow.h"
#include "header/CreateMenu.h"
#include "header/ImgChange.h" 
#include "header/CustomWindow.h"
#include "header/Filter.h"

using namespace cv;
using namespace std;

Menu::Menu(QWidget *parent, ImageProcessing *p)
	: QMenuBar(parent)
{
	I_MainWindow = (ImageProcessing*)p;
	
	I_ToolBar = new QToolBar ;
	I_ToolBar = I_MainWindow->addToolBar(tr("�ļ�������"));
	I_ToolBar->setMovable(false);
	
	Menu_File();
	Menu_Edit();
	Menu_Geom();
	Menu_Gray();
	Menu_Enhance();
	Menu_Morp();	
	Menu_Detection();
}

Menu::~Menu()
{
}

void Menu::Menu_File()
{
	QAction* Act_file_new = new QAction(QIcon("../Image/file/New.png"), tr("�½�"), this);
	Act_file_new->setShortcut(Qt::Key_Control & Qt::Key_N);
	//Act_file_new->setStatusTip(tr("�½�ͼ��"));
	connect(Act_file_new, SIGNAL(triggered()), this, SLOT(File_new()));

	QMenu* Act_file_new2 = new QMenu;
	Act_file_new2->setIcon(QIcon("../Image/file/New.png"));
	Act_file_new2->setTitle(tr("�½�"));
	QAction* New1 = new QAction(QIcon("../Image/file/New.png"), tr("500 * 500"), this);
	//connect(Act_geom_size, SIGNAL(triggered()), I_MainWindow, SLOT(autoSize()));
	QAction* New2 = new QAction(QIcon("../Image/file/New.png"), tr("800 * 600"), this);
	//connect(Act_geom_enlarge, SIGNAL(triggered()), I_MainWindow, SLOT(enlarge2()));
	QAction* New3 = new QAction(QIcon("../Image/file/New.png"), tr("1024 * 768"), this);
	//connect(Act_geom_reduce, SIGNAL(triggered()), I_MainWindow, SLOT(reduce2()));
	Act_file_new2->addAction(New1);
	Act_file_new2->addAction(New2);
	Act_file_new2->addAction(New3);
	connect(New1, SIGNAL(triggered()), this, SLOT(File_new1()));
	connect(New2, SIGNAL(triggered()), this, SLOT(File_new2()));
	connect(New3, SIGNAL(triggered()), this, SLOT(File_new3()));


	QAction* Act_file_open = new QAction(QIcon("../Image/file/Open.png"), tr("��"), this);
	Act_file_open->setShortcuts(QKeySequence::Open);
	//Act_file_open->setStatusTip(tr("��ͼ��"));
	connect(Act_file_open, SIGNAL(triggered()), this, SLOT(File_open()));

	QAction* Act_file_save = new QAction(QIcon("../Image/file/Save.png"), tr("����"), this);
	Act_file_save->setShortcuts(QKeySequence::Save);
	//Act_file_save->setStatusTip(tr("����ͼ��"));
	connect(Act_file_save, SIGNAL(triggered()), this, SLOT(File_save()));

	QAction* Act_file_saveas = new QAction(QIcon("../Image/file/SaveAs.png"), tr("���Ϊ"), this);
	Act_file_saveas->setShortcuts(QKeySequence::SaveAs);
	//Act_file_saveas->setStatusTip(tr("ͼ�����Ϊ"));
	connect(Act_file_saveas, SIGNAL(triggered()), this, SLOT(File_saveas()));

	QAction* Act_file_close = new QAction(QIcon("../Image/file/Close.png"), tr("�ر�"), this);
	Act_file_close->setShortcuts(QKeySequence::Close);
	//Act_file_close->setStatusTip(tr("�ر����"));
	connect(Act_file_close, SIGNAL(triggered()), I_MainWindow, SLOT(close()));

	QMenu* file = addMenu(tr("�ļ�"));
	file->addMenu(Act_file_new2);
	file->addAction(Act_file_open);
	file->addAction(Act_file_save);
	file->addAction(Act_file_saveas);
	file->addSeparator();						//���һ���ָ���
	file->addAction(Act_file_close);

	I_ToolBar->addAction(Act_file_new);
	I_ToolBar->addAction(Act_file_open);
	I_ToolBar->addAction(Act_file_save);
}

void Menu::Menu_Detection()
{
	QAction* Act_face_detection = new QAction(QIcon("../Image/file/New.png"), tr("�沿ʶ��"), this);
	QAction* Act_person_detection = new QAction(QIcon("../Image/file/New.png"), tr("����ʶ��"), this);
	QAction* Act_beautiful_detection = new QAction(QIcon("../Image/file/New.png"), tr("һ������"), this);
	QAction* Act_edge_detection = new QAction(QIcon("../Image/file/New.png"), tr("�������"), this);

	QMenu* detection = addMenu(tr("�߼�����"));

	detection->addAction(Act_face_detection);
	detection->addAction(Act_person_detection);
	detection->addAction(Act_beautiful_detection);
	detection->addAction(Act_edge_detection);

	connect(Act_face_detection, SIGNAL(triggered()), I_MainWindow, SLOT(faceDetection()));
	connect(Act_person_detection, SIGNAL(triggered()), I_MainWindow, SLOT(personDetection()));
	connect(Act_beautiful_detection, SIGNAL(triggered()), I_MainWindow, SLOT(beautifulDetection()));
	connect(Act_edge_detection, SIGNAL(triggered()), I_MainWindow, SLOT(edgeDetection()));

	//Act_edit_undo = new QAction(QIcon("../Image/edit/Edit_Cancel.png"), tr("����"), this);
	//Act_edit_undo->setShortcuts(QKeySequence::Undo);
	//connect(Act_edit_undo, SIGNAL(triggered()), I_MainWindow, SLOT(undo()));

	//Act_edit_redo = new QAction(QIcon("../Image/edit/Edit_Resume.png"), tr("����"), this);
	//Act_edit_redo->setShortcuts(QKeySequence::Redo);
	//connect(Act_edit_redo, SIGNAL(triggered()), I_MainWindow, SLOT(redo()));

	//QAction* Act_edit_full = new QAction(QIcon("../Image/edit/Edit_Full.png"), tr("ȫ����ʾ"), this);
	//Act_edit_full->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
	//connect(Act_edit_full, SIGNAL(triggered()), I_MainWindow, SLOT(fullScreen()));

	//QAction* Act_edit_back = new QAction(QIcon("../Image/edit/Edit_Max.png"), tr("�˳�ȫ��"), this);
	//Act_edit_back->setShortcut(QKeySequence(Qt::Key_Escape));
	//connect(Act_edit_back, SIGNAL(triggered()), I_MainWindow, SLOT(maxScreen()));

	//I_ToolBar->addAction(Act_edit_undo);
	//I_ToolBar->addAction(Act_edit_redo);
}

void Menu::Menu_Edit()
{
	Act_edit_undo = new QAction(QIcon("../Image/edit/Edit_Cancel.png"), tr("����"), this);
	Act_edit_undo->setShortcuts(QKeySequence::Undo);
	connect(Act_edit_undo, SIGNAL(triggered()), I_MainWindow, SLOT(undo()));
	
	Act_edit_redo = new QAction(QIcon("../Image/edit/Edit_Resume.png"), tr("����"), this);
	Act_edit_redo->setShortcuts(QKeySequence::Redo);
	connect(Act_edit_redo, SIGNAL(triggered()), I_MainWindow, SLOT(redo()));

	QAction *Act_edit_full = new QAction(QIcon("../Image/edit/Edit_Full.png"), tr("ȫ����ʾ"), this);
	Act_edit_full->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
	connect(Act_edit_full, SIGNAL(triggered()), I_MainWindow, SLOT(fullScreen()));

	QAction *Act_edit_back = new QAction(QIcon("../Image/edit/Edit_Max.png"), tr("�˳�ȫ��"), this);
	Act_edit_back->setShortcut(QKeySequence(Qt::Key_Escape));
	connect(Act_edit_back, SIGNAL(triggered()), I_MainWindow, SLOT(maxScreen()));

	I_ToolBar->addAction(Act_edit_undo);
	I_ToolBar->addAction(Act_edit_redo);
}

void Menu::Menu_Geom()
{
	// �����˵�
	QAction *Act_geom_size = new QAction(QIcon("../Image/geom/Geom_Default.png"), tr("����Ӧ����"), this);
	connect(Act_geom_size, SIGNAL(triggered()), I_MainWindow, SLOT(autoSize()));
	QAction *Act_geom_enlarge = new QAction(QIcon("../Image/geom/Geom_Enlarge.png"), tr("�Ŵ�һ��"), this);
	connect(Act_geom_enlarge, SIGNAL(triggered()), I_MainWindow, SLOT(enlarge2()));
	QAction *Act_geom_reduce = new QAction(QIcon("../Image/geom/Geom_Reduce.png"), tr("��Сһ��"), this);
	connect(Act_geom_reduce, SIGNAL(triggered()), I_MainWindow, SLOT(reduce2()));

	menu_resize = new QMenu();
	menu_resize->setIcon(QIcon("../Image/geom/Geom_EnlargeReduce.png"));
	menu_resize->setTitle(tr("ͼ������"));
	menu_resize->addAction(Act_geom_size);
	menu_resize->addAction(Act_geom_enlarge);
	menu_resize->addAction(Act_geom_reduce);

	QAction *Act_geom_left = new QAction(QIcon("../Image/geom/Geom_RotateRight.png"), tr("˳ʱ����ת"), this);
	connect(Act_geom_left, SIGNAL(triggered()), I_MainWindow, SLOT(rotateImage_270()));
	QAction *Act_geom_right = new QAction(QIcon("../Image/geom/Geom_RotateLeft.png"), tr("��ʱ����ת"), this);
	connect(Act_geom_right, SIGNAL(triggered()), I_MainWindow, SLOT(rotateImage_90()));
	QAction *Act_geom_180 = new QAction(QIcon("../Image/geom/Geom_RotateCenter.png"), tr("������ת"), this);
	connect(Act_geom_180, SIGNAL(triggered()), I_MainWindow, SLOT(rotateImage_180()));

	menu_rotate = new QMenu();
	menu_rotate->setIcon(QIcon("../Image/geom/Geom_Rotate.png"));
	menu_rotate->setTitle(tr("ͼ����ת"));
	menu_rotate->addAction(Act_geom_left);
	menu_rotate->addAction(Act_geom_right);
	menu_rotate->addAction(Act_geom_180);

	QAction *Act_geom_H = new QAction(QIcon("../Image/geom/Geom_FlipX.png"), tr("ˮƽ��ת"), this);
	connect(Act_geom_H, SIGNAL(triggered()), I_MainWindow, SLOT(flipImage_H()));
	QAction *Act_geom_V = new QAction(QIcon("../Image/geom/Geom_FlipY.png"), tr("��ֱ��ת"), this);
	connect(Act_geom_V, SIGNAL(triggered()), I_MainWindow, SLOT(flipImage_V()));

	menu_flip = new QMenu();
	menu_flip->setIcon(QIcon("../Image/geom/Geom_Flip.png"));
	menu_flip->setTitle(tr("ͼ��ת"));
	menu_flip->addAction(Act_geom_H);
	menu_flip->addAction(Act_geom_V);
	
	QMenu *geom = addMenu(tr("���α任"));
	geom->addMenu(menu_resize);
	geom->addMenu(menu_rotate);
	geom->addMenu(menu_flip);
}

void Menu::Menu_Gray()		// �Ҷȱ任
{
	// �����˵�
	QAction *Act_gray_log = new QAction(QIcon("../Image/gray/Gray_Log.png"), tr("�����任"), this);
	connect(Act_gray_log, SIGNAL(triggered()), I_MainWindow, SLOT(logImageOK()));
	QAction *Act_gray_gamma = new QAction(QIcon("../Image/gray/Gray_Gamma.png"),tr("٤��任"), this);
	connect(Act_gray_gamma, SIGNAL(triggered()), I_MainWindow, SLOT(gammaImageOK()));
	QAction *Act_gray_histeq = new QAction(QIcon("../Image/gray/Gray_Histeq.png"), tr("ֱ��ͼ���⻯"), this);
	connect(Act_gray_histeq, SIGNAL(triggered()), I_MainWindow, SLOT(histeqImage()));

	menu_nolinear = new QMenu();
	menu_nolinear->addAction(Act_gray_log);
	menu_nolinear->addAction(Act_gray_gamma);
	menu_nolinear->addAction(Act_gray_histeq);
	
	// һ���˵�
	QAction *Act_gray_bin = new QAction(QIcon("../Image/gray/Gray_Bin.png"), tr("��ֵͼ"), this);
	connect(Act_gray_bin, SIGNAL(triggered()), I_MainWindow, SLOT(binImageOK()));
	QAction *Act_gray_gray = new QAction(QIcon("../Image/gray/Gray_Gray.png"), tr("�Ҷ�ͼ"), this);
	connect(Act_gray_gray, SIGNAL(triggered()), I_MainWindow, SLOT(grayImage()));
	QAction *Act_gray_reverse = new QAction(QIcon("../Image/gray/Gray_Reverse.png"), tr("��ת�任"), this);
	connect(Act_gray_reverse, SIGNAL(triggered()), I_MainWindow, SLOT(reverseImage()));
	QAction *Act_gray_linear = new QAction(QIcon("../Image/gray/Gray_Linear.png"), tr("���Ա任"), this);
	connect(Act_gray_linear, SIGNAL(triggered()), I_MainWindow, SLOT(linearImageOK()));
	QAction *Act_gray_nolinear = new QAction(QIcon("../Image/gray/Gray_Nolinear.png"), tr("�����Ա任"), this);
	Act_gray_nolinear->setMenu(menu_nolinear);
	
	QMenu *gray = addMenu(tr("�Ҷȱ任"));
	gray->addAction(Act_gray_bin);
	gray->addAction(Act_gray_gray);
	gray->addAction(Act_gray_reverse);
	gray->addAction(Act_gray_linear);
	gray->addAction(Act_gray_nolinear);
}

void Menu::Menu_Enhance()
{
	// �����˵�
	QAction *Act_enhance_circles = new QAction(tr("Բ���"), this);
	connect(Act_enhance_circles, SIGNAL(triggered()), I_MainWindow, SLOT(circleImage()));
	QAction *Act_enhance_lines = new QAction(tr("ֱ�߼��"), this);
	connect(Act_enhance_lines, SIGNAL(triggered()), I_MainWindow, SLOT(lineImage()));

	menu_detect = new QMenu();
	menu_detect->addAction(Act_enhance_circles);
	menu_detect->addAction(Act_enhance_lines);
	
	// �����˵�
	QAction *Act_enhance_normalize = new QAction(tr("���˲�"));
	connect(Act_enhance_normalize, SIGNAL(triggered()), I_MainWindow, SLOT(normalizedImage()));
	QAction *Act_enhance_gaussian = new QAction(tr("��˹�˲�"));
	connect(Act_enhance_gaussian, SIGNAL(triggered()), I_MainWindow, SLOT(gaussianImage()));
	QAction *Act_enhance_median = new QAction(tr("��ֵ�˲�"));
	connect(Act_enhance_median, SIGNAL(triggered()), I_MainWindow, SLOT(medianImage()));

	menu_smooth = new QMenu();
	menu_smooth->addAction(Act_enhance_normalize);
	menu_smooth->addAction(Act_enhance_gaussian);
	menu_smooth->addAction(Act_enhance_median);

	QAction *Act_enhance_sobel = new QAction(tr("Sobel����"));
	connect(Act_enhance_sobel, SIGNAL(triggered()), I_MainWindow, SLOT(sobelImage()));
	QAction *Act_enhance_laplacian = new QAction(tr("Laplacian����"));
	connect(Act_enhance_laplacian, SIGNAL(triggered()), I_MainWindow, SLOT(laplacianImage()));
	QAction *Act_enhance_canny = new QAction(tr("Canny����"));
	connect(Act_enhance_canny, SIGNAL(triggered()), I_MainWindow, SLOT(cannyImage()));
	QAction *Act_enhance_hough = new QAction(tr("Hough����"));
	Act_enhance_hough->setMenu(menu_detect);

	menu_sharpen = new QMenu();
	menu_sharpen->addAction(Act_enhance_sobel);
	menu_sharpen->addAction(Act_enhance_laplacian);
	menu_sharpen->addAction(Act_enhance_canny);
	menu_sharpen->addAction(Act_enhance_hough);
		
	// һ���˵�
	QAction *Act_enhance_smooth = new QAction(QIcon("../Image/filter/ƽ������.png"),tr("ƽ��"), this);
	Act_enhance_smooth->setMenu(menu_smooth);
	QAction *Act_enhance_sharpen = new QAction(QIcon("../Image/filter/��.png"), tr("��"), this);
	Act_enhance_sharpen->setMenu(menu_sharpen);
	QAction* Act_enhance_frostedglass = new QAction(QIcon("../Image/filter/ģ��.png"), tr("��ɢ"), this);
	QAction* Act_enhance_sketch = new QAction(QIcon("../Image/filter/����.png"), tr("����"), this);
	QAction* Act_enhance_emboss = new QAction(QIcon("../Image/filter/����.png"), tr("����"), this);
	QAction* Act_enhance_glare = new QAction(QIcon("../Image/filter/ǿ���Ӧ.png"), tr("ǿ��"), this);
	connect(Act_enhance_glare, SIGNAL(triggered()), I_MainWindow, SLOT(glareImage()));
	connect(Act_enhance_frostedglass, SIGNAL(triggered()), I_MainWindow, SLOT(frostedglassImage()));
	connect(Act_enhance_sketch, SIGNAL(triggered()), I_MainWindow, SLOT(sketchImage()));
	connect(Act_enhance_emboss, SIGNAL(triggered()), I_MainWindow, SLOT(embossImage()));

	QMenu *enhance = addMenu(tr("ͼ����ǿ"));
	enhance->addAction(Act_enhance_smooth);
	enhance->addAction(Act_enhance_sharpen);
	enhance->addAction(Act_enhance_frostedglass);
	enhance->addAction(Act_enhance_sketch);
	enhance->addAction(Act_enhance_emboss);
	enhance->addAction(Act_enhance_glare);
}

void Menu::Menu_Morp()
{
	QAction *Act_morp_erode = new QAction(tr("��ʴ"), this);
	connect(Act_morp_erode, SIGNAL(triggered()), I_MainWindow, SLOT(erodeImage()));
	QAction *Act_morp_dilate = new QAction(tr("����"), this);
	connect(Act_morp_dilate, SIGNAL(triggered()), I_MainWindow, SLOT(dilateImage()));
	QAction *Act_morp_open = new QAction(tr("������"), this);
	connect(Act_morp_open, SIGNAL(triggered()), I_MainWindow, SLOT(openImage()));
	QAction *Act_morp_close = new QAction(tr("������"), this);
	connect(Act_morp_close, SIGNAL(triggered()), I_MainWindow, SLOT(closeImage()));
	QAction *Act_morp_tophat = new QAction(tr("��ñ����"), this);
	connect(Act_morp_tophat, SIGNAL(triggered()), I_MainWindow, SLOT(tophatImage()));
	QAction *Act_morp_blackhat = new QAction(tr("��ñ����"), this);
	connect(Act_morp_blackhat, SIGNAL(triggered()), I_MainWindow, SLOT(blackhatImage()));
	QAction *Act_morp_grad = new QAction(tr("��̬ѧ�ݶ�"), this);
	connect(Act_morp_grad, SIGNAL(triggered()), I_MainWindow, SLOT(gradImage()));

	QMenu *morp = addMenu(tr("��̬ѧ����"));
	morp->addAction(Act_morp_erode);
	morp->addAction(Act_morp_dilate);
	morp->addAction(Act_morp_open);
	morp->addAction(Act_morp_close);
	morp->addAction(Act_morp_tophat);
	morp->addAction(Act_morp_blackhat);
	morp->addAction(Act_morp_grad);
}


// -------------------�ļ�-------------------------------------------
void Menu::File_new()		// �½��ļ�
{

	// ��������ʾ
	QImage image = QImage(500, 500, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	I_MainWindow->ShowImage(image, true);
	I_MainWindow->output("�½�ͼ��: ��500����500");
	I_MainWindow->UpdateImgName("��ͼ��");
	currentPath = "";
}
void Menu::File_new1()		// �½��ļ�
{

	// ��������ʾ
	QImage image = QImage(500, 500, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	I_MainWindow->ShowImage(image, true);
	I_MainWindow->output("�½�ͼ��: ��500����500");
	I_MainWindow->UpdateImgName("��ͼ��");
	currentPath = "";
}
void Menu::File_new2()		// �½��ļ�
{

	// ��������ʾ
	QImage image = QImage(800, 600, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	I_MainWindow->ShowImage(image, true);
	I_MainWindow->output("�½�ͼ��: ��800����600");
	I_MainWindow->UpdateImgName("��ͼ��");
	currentPath = "";
}
void Menu::File_new3()		// �½��ļ�
{

	// ��������ʾ
	QImage image = QImage(1024, 768, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	I_MainWindow->ShowImage(image, true);
	I_MainWindow->output("�½�ͼ��: ��1024����768");
	I_MainWindow->UpdateImgName("��ͼ��");
	currentPath = "";
}

void Menu::File_open()		// ���ļ�
{
	QString path = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		
		QImage* img = new QImage();
		if (!(img->load(path)))
		{
			QMessageBox::information(this, tr("����"), tr("��ͼ��ʧ�ܣ�"));
			delete img;
			return;
		}
		I_MainWindow->ShowImage(*img, true);
		I_MainWindow->saveRaw(imread(path.toStdString(), 1));
		I_MainWindow->UpdateImgName(path);
		currentPath = path;
	}
}

void Menu::File_save()			// �����ļ�
{
	ImgChange *imgchange = new ImgChange();

	if (currentPath.isEmpty())
	{
		QString path = QFileDialog::getSaveFileName(this, tr("����ͼ��"), ".", tr("Images(*.jpg *.png *.bmp)"));
		{
			if (!path.isEmpty())
			{
				QImage img = I_MainWindow->imgLabel->getImage();
				img.save(path);
				I_MainWindow->UpdateImgName(path);
				currentPath = path;
				I_MainWindow->ShowImage(img, true);
			}
		}
	}
	else
	{
		QImage img = I_MainWindow->imgLabel->getImage();
		img.save(currentPath);
		I_MainWindow->ShowImage(img, true);
	}
}

void Menu::File_saveas()			// ���Ϊ�ļ�
{
	QString path = QFileDialog::getSaveFileName(this, tr("ͼ�����Ϊ"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		ImgChange *imgchange = new ImgChange();
		QImage img = I_MainWindow->imgLabel->getImage();
		img.save(path);
		I_MainWindow->UpdateImgName(path);
		I_MainWindow->ShowImage(img, true);
	}
}

void Menu::setMenuQSS(bool flag)		// ���ö����˵�QSS
{
	setSubMenu(menu_resize,flag);
	setSubMenu(menu_rotate, flag);
	setSubMenu(menu_flip, flag);
	setSubMenu(menu_nolinear, flag);
	setSubMenu(menu_detect, flag);
	setSubMenu(menu_smooth, flag);
	setSubMenu(menu_sharpen, flag);
}

void Menu::setSubMenu(QMenu *menu, bool flag)		// ����qss
{
	if (flag == true)
		menu->setStyleSheet("background-color: rgba(74, 79, 81, 1); color: rgb(187, 187, 187);selection-background-color:#4b6eaf; ");
	else
		menu->setStyleSheet(0);
}