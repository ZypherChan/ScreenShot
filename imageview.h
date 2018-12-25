#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsview >
QT_BEGIN_NAMESPACE
class QWheelEvent;
QT_END_NAMESPACE


class ImageView : public QGraphicsView  
{
	Q_OBJECT

public:
	ImageView(QWidget *parent = NULL);
	~ImageView();

private:
	int m_current_scale;
	qreal m_zoomLevel;
	QPoint ptLast;
	bool m_bPressed;

private:
	void initParam();
	void initUI();

protected:
	void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
	void updateZoomLevel();
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	
};

#endif // IMAGEVIEW_H
