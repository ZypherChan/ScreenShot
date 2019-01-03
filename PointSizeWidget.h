#ifndef POINTSIZEWIDGET_H
#define POINTSIZEWIDGET_H

#include "struct.h"
#include <QLabel>

#define  LABELSIZE  2 * ITEM_LENGTH + 2

class PointSizeWidget : public QLabel
{
	Q_OBJECT

public:
	PointSizeWidget(int point_size = 2, QWidget *parent = 0);
	~PointSizeWidget();

	void setPointSize(int point_size);
	int getPointSize() const;

signals:
	void wheelscrolled(int point_size);

protected:
	void wheelEvent(QWheelEvent *event)		Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event)		Q_DECL_OVERRIDE;

private:
	int _point_size;
	QPoint _ptCenter;
};

#endif // POINTSIZEWIDGET_H
