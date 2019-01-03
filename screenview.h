#ifndef SCREENVIEW_H
#define SCREENVIEW_H
#include <QtWidgets/QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "struct.h"
#include "LinePaint.h"
#include "RectPaint.h"
#include "TextPaint.h"
#include "ColorItem.h"
#include "PointSizeWidget.h"

typedef RectPaint EllipsePaint;


class ScreenView : public QWidget
{
	Q_OBJECT

public:
	ScreenView(QWidget *parent = NULL);	
	~ScreenView();

	/**
	 * @brief: ���ñ���ͼƬ
	 * @input: 
	 * @output: 
	 * @return: 
	 * @author: ZypherChan
	 * @date: 2018/12/13 22:07
	 **/
	void setBackGroundPixmap(QPixmap &px);

	/**
	 * @brief: ������Ĵ�С,��֤����С������
	 * @input: 
	 * @output: 
	 * @author: ZypherChan
	 * @date: 2018/12/13 22:01
	 **/
	void setTopLeftAndButtomRight(QPointF &ptTL, QPointF &ptBR);	

	/**
	 * @brief:���ݴ���㹹����� 
	 * @input: 
	 * @output: 
	 * @author: ZypherChan
	 * @date: 2018/12/13 22:02
	 **/
	QRectF getRectF(QPointF p1, QPointF p2);	

	/**
	 * @brief:��ǰ����ھ��������λ�� 
	 * @input: 
	 * @output: 
	 * @author: ZypherChan
	 * @date: 2018/12/13 22:02
	 **/
	CursorLocation caputerCursor(QRectF rect, qreal x, qreal y);

public slots:
	void copyImage();
	void saveImage();
	void drawLine();
	void drawRect();
	void drawEllipse();
	void drawTextStatus();

	void colorItemChanged(const QColor &color);
	void colorSelection();

	void pointSizeChanged(int point_size);

private:
	void init();

	/**
	* @brief: ��ʼ����ɫ������
	* @input:
	* @output:
	* @return:
	* @author: ZypherChan
	* @date: 2018/12/20 21:49
	**/
	void initColorBar();

	void showColorBar();

	void hideColorBar();

	/**
	 * @brief: ��ʼ��������С����ʾ��ǩ
	 * @input: 
	 * @output: 
	 * @return: 
	 * @author: ZypherChan
	 * @date: 2018/12/18 21:18
	 **/
	void initLabel();

	void showLabel();

	void hideLabel();

	/**
	 * @brief: ��ʼ������������
	 * @input: 
	 * @output: 
	 * @return: 
	 * @author: ZypherChan
	 * @date: 2018/12/18 21:20
	 **/
	void initToolBar();	
	
	void showToolBar();	
	
	void hideToolBar();

	/**
	 * @brief: ��ȡ��Ļ��������rect���ֵ�ͼ��
	 * @input: 
	 * @output: 
	 * @author: ZypherChan
	 * @date: 2018/12/13 21:59
	 **/
	void screenCapture(const QRectF &rect);

	/**
	 * @brief: ������ȡ��Χ�Ĵ�С
	 * @input: 
	 * @output: 
	 * @author: ZypherChan
	 * @date: 2018/12/13 21:59
	 **/
	void adjustShotScreen(QMouseEvent *event);

	/**
	 * @brief: �����ı���������
	 * @input: 
	 * @output: 
	 * @return: 
	 * @author: ZypherChan
	 * @date: 2018/12/15 17:38
	 **/
	void drawText(QMouseEvent *event);

protected:
	void mousePressEvent(QMouseEvent *event)   Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event)    Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event)       Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event)        Q_DECL_OVERRIDE;

private:
	QPixmap _fullPixmap;
	QPixmap _shotPixmap;
	double _screen_width;
	double _screen_height;
	
	QPointF _ptS;
	QPointF _ptE;
	QPointF _ptDrag;
	QRectF _shortArea;
	DrawStatus _draw_flag;
	DrawEditFlag _draw_edit_flag;
	QClipboard *_clipboard;
	CursorLocation _curlocation;    //��ǰ���λ��

	QWidget *_toolbar;              //������
	QWidget *_colorbar;             //��ɫ������
	QPushButton *_btn_copy;
	QPushButton *_btn_save;
	QPushButton *_btn_drawLine;
	QPushButton *_btn_drawRect;
	QPushButton *_btn_drawEllipse;
	QPushButton *_btn_drawText;
	QLabel *_label;

	QList<LinePaint> _line_list;
	QList<RectPaint> _rect_list;
	QList<EllipsePaint> _ellipse_list;
	QList<TextPaint*> _text_list;

	ColorItem *_cur_coloritem;
	QColor _pen_color;
	int _point_size;

	bool _bIsDrawLineEnd;//�жϵ�ǰ�����߶��Ƿ����
};

#endif // SCREENVIEW_H
