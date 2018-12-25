#ifndef COLORITEM_H
#define COLORITEM_H

#include "struct.h"
#include <QLabel>

class ColorItem : public QLabel
{
	Q_OBJECT

public:
	ColorItem(const QColor &color ,int item_length = ITEM_LENGTH ,QWidget *parent = 0);
	~ColorItem();

	/**
	 * @brief: 设置控件颜色
	 * @input: 
	 * @output: 
	 * @return: 
	 * @author: ZypherChan
	 * @date: 2018/12/19 21:59
	 **/
	void setColor(const QColor &color);

	/**
	 * @brief: 获取控件颜色
	 * @input: 
	 * @output: 
	 * @return: 
	 * @author: ZypherChan
	 * @date: 2018/12/20 22:43
	 **/
	QColor getColor() const;

protected:
	void mousePressEvent(QMouseEvent *event)  Q_DECL_OVERRIDE;
	void enterEvent(QEvent *event)            Q_DECL_OVERRIDE;
	void leaveEvent(QEvent *event)			  Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event)		  Q_DECL_OVERRIDE;

signals:
	void itemClicked(const QColor &color);
	void clicked();

private:
	bool _bMouseIn;
	QColor _color;
	int _item_length;
};

#endif // COLORITEM_H
