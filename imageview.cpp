#include "imageview.h"
#include <QApplication>
#include <QWheelEvent>
#include <QMenu>
#include <QScrollBar>
#include "QPainter"
#define EPS 1.0e-6

ImageView::ImageView(QWidget *parent)
	: QWidget(parent)
{
	initParam();
	initUI();
}
ImageView::~ImageView()
{

}

void ImageView::setImage(const QString& imagepath)
{
	m_pix = QPixmap(imagepath);
	resize(m_pix.width(), m_pix.height());
}

void ImageView::initParam()
{
	m_zoomLevel = 1.0;
	m_current_scale = 10;
}

void ImageView::initUI()
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setStyleSheet("border-width:0;border-style:outset");
}


void ImageView::wheelEvent(QWheelEvent *event)
{
	if (!this->underMouse())
		return;
	qreal rate = event->delta();
	int new_scale = m_current_scale;
	if(rate > 0)
		new_scale += 1;
	else
		new_scale = new_scale - 1 < 1 ? 1: new_scale-1;
		
	m_zoomLevel = double(new_scale)/double(m_current_scale);
	updateZoomLevel();
	m_current_scale = new_scale;
}

void ImageView::updateZoomLevel()
{
	if(m_current_scale < 1)
		return;

	resize(width()*m_zoomLevel, height()*m_zoomLevel);
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
	setFocus();
	if(Qt::LeftButton == event->button() && 0 == (Qt::WindowMaximized & windowState()))
	{
		QPoint pt = event->globalPos();
		ptLast = pt - frameGeometry().topLeft();
	}
	else if(Qt::RightButton == event->button() && 0 == (Qt::WindowMaximized & windowState()))
	{
		QMenu *menu = new QMenu;
		QAction *actExit = new QAction(menu);
		actExit->setText(QStringLiteral("关闭"));
		connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
		menu->addAction(actExit);
		menu->exec(QCursor::pos());
	}
	event->ignore();
	m_bPressed =true;
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
	if(isMaximized())
		return;

	if((event->buttons() & Qt::LeftButton) && m_bPressed)
	{
		QPoint pt = event->globalPos();
		pt = pt - ptLast;
		move(pt);
	}
}

void ImageView::mouseReleaseEvent(QMouseEvent *event) 
{
	QApplication::restoreOverrideCursor();//恢复鼠标指针形状
	event->ignore();

	m_bPressed = false;
}

void ImageView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QRect rect = this->rect();
	painter.drawPixmap(rect, m_pix);
}
