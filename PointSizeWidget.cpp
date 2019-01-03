#include "PointSizeWidget.h"
#include <QPainter>
#include <QWheelEvent>

PointSizeWidget::PointSizeWidget(int point_size/* = 2*/, QWidget *parent/* = 0*/)
	: QLabel(parent), _point_size(point_size)
{
	setFixedSize(LABELSIZE, LABELSIZE);
	setStyleSheet("background-color:rgb(0, 122, 204);");

	_ptCenter.setX(width()*0.5);
	_ptCenter.setY(height()*0.5);
}

PointSizeWidget::~PointSizeWidget()
{

}

void PointSizeWidget::setPointSize(int point_size)
{
	_point_size = point_size;
	update();
}

int PointSizeWidget::getPointSize() const
{
	return _point_size;
}

void PointSizeWidget::wheelEvent(QWheelEvent *event)
{
	qreal rate = event->delta();
	if (rate > 0)
	{
		_point_size >= 16 ? _point_size=16 : _point_size++;
	}
	else
	{
		_point_size <= 2 ? _point_size = 2 : _point_size--;
	}
	emit wheelscrolled(_point_size);
	update();
}

void PointSizeWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBrush(QBrush(Qt::white));
	painter.setPen(QPen(Qt::white));
	painter.setRenderHint(QPainter::Antialiasing, true); //设置渲染提示为消除锯齿
	int radius = _point_size * 0.7;
	radius < 2 ? radius = 2 : radius;
	painter.drawEllipse(_ptCenter, radius, radius);
	QString tip = QStringLiteral("%1").arg(_point_size);
	setToolTip(tip);
}
