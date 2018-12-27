#include "LinePaint.h"

LinePaint::LinePaint(const QLine &line, const QPen &pen)
	: QLine(line)
{
	_pen = pen;
}

LinePaint::LinePaint(const QLine &line)
	: QLine(line)
{

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

