#include "LinePaint.h"

LinePaint::LinePaint(const QLine &line, const QPen &pen, QObject *parent/* = NULL*/)
	: QObject(parent)
{
	_line = line;
	_pen = pen;
}

LinePaint::LinePaint(const LinePaint &linepaint)
{
	_line = linepaint.getLine();
	_pen = linepaint.getPen();
}

LinePaint::~LinePaint()
{

}

void LinePaint::setPen(const QPen &pen)
{
	_pen = pen;
}

const QPen& LinePaint::getPen() const
{
	return _pen;
}

void LinePaint::setP2(const QPoint &ap2)
{
	_line.setP2(ap2);
}

const QLine& LinePaint::getLine() const
{
	return _line;
}
