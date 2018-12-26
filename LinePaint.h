#ifndef LINEPAINT_H
#define LINEPAINT_H

#include <QObject>
#include <QLine>
#include <QPen>

class LinePaint : public QObject
{
	Q_OBJECT

public:
	LinePaint(const QLine &line, const QPen &pen, QObject *parent = NULL);
	LinePaint(const LinePaint &linepaint);
	~LinePaint();

	void setPen(const QPen &pen);
	const QPen& getPen() const;

	void setP2(const QPoint &ap2);

	const QLine& getLine() const;

private:
	QLine _line;
	QPen _pen;
};

#endif // LINEPAINT_H
