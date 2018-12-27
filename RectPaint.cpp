#include "RectPaint.h"

RectPaint::RectPaint()
{

}

RectPaint::RectPaint(const QPointF &topLeft, const QPointF &bottomRight, const QPen &pen)
	:QRectF(topLeft, bottomRight)
{
	_pen = pen;
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

