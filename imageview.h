#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget >
#include "QImage"
QT_BEGIN_NAMESPACE
class QWheelEvent;
QT_END_NAMESPACE


class ImageView : public QWidget  
{
	Q_OBJECT

public:
	ImageView(QWidget *parent = NULL);
	~ImageView();

	void setImage(const QString& imagepath);
	void setImage(const QPixmap& pixmap);
	void setImageGeometry(const QRect& rect);

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
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

	
private:
	QPixmap m_pix;
};

#endif // IMAGEVIEW_H
