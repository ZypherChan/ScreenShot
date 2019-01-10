#include "RectPaint.h"

RectPaint::RectPaint()
{

}

RectPaint::RectPaint(const QPointF &topLeft, const QPointF &bottomRight, const QPen &pen, const QBrush &brush)
	:QRectF(topLeft, bottomRight), _pen(pen),_brush(brush)
{

}

RectPaint::RectPaint(const QRectF &rect)
	: QRectF(rect)
{
	
}

RectPaint::~RectPaint()
{

}

void RectPaint::setPen(const QPen &pen)
{
	_pen = pen;
}

const QPen& RectPaint::getPen() const
{
	return _pen;
}

void RectPaint::setBrush(const QBrush &brush)
{
	_brush = brush;
}

const QBrush& RectPaint::getBrush() const
{
	return _brush;
}

