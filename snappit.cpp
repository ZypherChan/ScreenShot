#include "snappit.h"
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QScreen>
#include <QMouseEvent>
#include <QBitmap>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

snappit::snappit(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	logInit();
	LOG(INFO) << "snappit::snappit()";
	_file_path = QStringLiteral(".");

	m_tray = new QSystemTrayIcon(QIcon(":/image/main.ico"), this);
	m_tray->setToolTip(tr("Snappit"));
	m_tray->show();
	m_tray->showMessage(tr(""), tr("Snappit"));

	m_trayMenu = new QMenu();
	m_trayMenu->addAction(tr("首选项"));
	m_trayMenu->addAction(tr("退出"));
	m_tray->setContextMenu(m_trayMenu);

	ui.pushButton_shortcut->setShortcut(QKeySequence(QLatin1String("F1")));

	connect(ui.pushButton_open,     &QPushButton::clicked, this, &snappit::openImage);
	connect(ui.pushButton_shortcut, &QPushButton::clicked, this, &snappit::screenShotCut);
	connect(m_trayMenu, &QMenu::triggered, this, &snappit::trayMenuTrigged);
	connect(m_tray, &QSystemTrayIcon::activated, this, &snappit::trayActivated);
}

void snappit::logInit()
{
	QString app_path = QCoreApplication::applicationDirPath();
	QString log_path = app_path + "\\snappit_log";
	
	QDir dir;
	if (!dir.exists(log_path))
		dir.mkdir(log_path);
	
	FLAGS_log_dir = log_path.toLocal8Bit();
	google::InitGoogleLogging("snappit");
	FLAGS_logbufsecs = 0;
	FLAGS_colorlogtostderr = true;
	FLAGS_max_log_size = 1024;
	FLAGS_stop_logging_if_full_disk = true;
}

snappit::~snappit()
{
	for(int i = 0; i < _arr_imgView.size(); i++)
	{
		ImageView *imgView = _arr_imgView[i];
		if(imgView != NULL)
		{
			imgView->close();
			delete imgView;
			imgView = NULL;
		}
	}
	LOG(INFO) << "snappit::~snappit()";
	google::ShutdownGoogleLogging();
}

void snappit::openImage()
{
	QString img_path = QFileDialog::getOpenFileName(this, QStringLiteral("选择要打开的图片"), _file_path, "Image files (*.bmp *.jpg *.png);;All files (*.*)");
	if(!img_path.isEmpty())
	{
		QFileInfo file(img_path);
		_file_path = file.absolutePath();
		LOG(INFO) << img_path.toLocal8Bit().constData();
		QImage img(img_path);
		QGraphicsScene *scene = new QGraphicsScene;
		scene->addPixmap(QPixmap::fromImage(img));
		ImageView *imgView = new ImageView();
		imgView->setScene(scene);
		imgView->resize(scene->width(), scene->height());
		imgView->show();
		_arr_imgView.push_back(imgView);
	}
}

void snappit::screenShotCut()
{
	QScreen *screen = QGuiApplication::primaryScreen();
	QPixmap fullPixmap = screen->grabWindow(0);
	ScreenView *screenView = new ScreenView();
	screenView->setBackGroundPixmap(fullPixmap);  //传递全屏背景图片
	screenView->show();
}


void snappit::trayMenuTrigged(QAction* action)
{
	QString text = action->text();
	if (text == tr("首选项"))
	{
		this->hide();
		this->setWindowFlags(Qt::WindowStaysOnTopHint);
		this->show();
	}
	else if (text == tr("退出"))
	{
		exit(0);
	}
}

void snappit::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		this->hide();
		this->setWindowFlags(Qt::WindowStaysOnTopHint);
		this->show();
		break;
	default:
		break;
	}
}

void snappit::hideEvent(QHideEvent *event)
{
	if (m_tray->isVisible())
	{
		this->hide();
		event->ignore();
	}
}

void snappit::closeEvent(QCloseEvent *event)
{
	if (m_tray->isVisible())
	{
		this->hide(); //隐藏窗口
		event->ignore(); //忽略事件
	}
}
