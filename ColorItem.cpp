#include "ColorItem.h"
#include <QPainter>

ColorItem::ColorItem(const QColor &color, int item_length /*= ITEM_LENGTH*/, QWidget *parent /*= 0*/)
	: QLabel(parent), _item_length(item_length)
{
	_bMouseIn = false;
	setFixedSize(_item_length, _item_length);
	setColor(color);
}

ColorItem::~ColorItem()
{

}

void ColorItem::setColor(const QColor &color)
{
	_color = color;
	QImage itemImg = QImage(_item_length, _item_length, QImage::Format_RGB32);
	QPainter painter(&itemImg);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.drawImage(0, 0, itemImg);
	painter.setPen(QPen(Qt::NoPen));
	painter.setBrush(color);
	painter.drawRect(0, 0, _item_length, _item_length);
	setPixmap(QPixmap::fromImage(itemImg));
	painter.setPen(QPen(Qt::black, 2));
	painter.drawRect(1, 1, _item_length - 2, _item_length - 2);
	update();
}

QColor ColorItem::getColor() const
{
	return _color;
}

void ColorItem::mousePressEvent(QMouseEvent *event)
{
	emit itemClicked(_color);
	emit clicked();
}

void ColorItem::enterEvent(QEvent *event)
{
	_bMouseIn = true;
	update();
}

void ColorItem::leaveEvent(QEvent *event)
{
	_bMouseIn = false;
	update();
}

void ColorItem::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);
	if (_bMouseIn)
	{
		QPainter painter(this);
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.setPen(QPen(Qt::white, 2));
		painter.drawRect(1, 1, _item_length - 2, _item_length - 2);
	}
}
