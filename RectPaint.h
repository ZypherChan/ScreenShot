#ifndef RECTPAINT_H
#define RECTPAINT_H

#include <QRectF>
#include <QPen>

class RectPaint : public QRectF
{
public:
	RectPaint();
	RectPaint(const QPointF &topLeft, const QPointF &bottomRight, const QPen &pen);
	RectPaint(const QRectF &rect);
	~RectPaint();

	void setPen(const QPen &pen);
	const QPen& getPen() const;

private:
	QPen _pen;
};

#endif // RECTPAINT_H