#include "snappit.h"
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QScreen>
#include <QMouseEvent>
#include <QBitmap>

snappit::snappit(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	logInit();
	_file_path = QStringLiteral(".");

	connect(ui.pushButton_open, SIGNAL(clicked()), this, SLOT(openImage()));
	connect(ui.pushButton_shortcut, SIGNAL(clicked()), this, SLOT(screenShotCut()));
	LOG(INFO) << "snappit::snappit()";
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


void snappit::closeEvent(QCloseEvent *event)
{
	//to do
	QWidget::closeEvent(event);
}
