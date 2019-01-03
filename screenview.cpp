#include "screenview.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <Qpainter>
#include <Qboxlayout>
#include <Qpushbutton>
#include <assert.h>
#include <Qscreen>
#include <QClipboard>
#include <QDateTime>
#include <QFileDialog>
#include <QFontMetrics>
#include <QLabel>
#include <QColorDialog>
#include <QSplitter>

static const QString s_normalStyle = QStringLiteral("QPushButton:hover{background-color: rgb(204, 206, 219);border:none;color:rgb(255, 255, 255);}");
static const QString s_pressStyle  = QStringLiteral("border:none;background-color:rgb(0, 122, 204);");
const QPen penScreenShotBound = QPen(Qt::cyan, 5, Qt::SolidLine);

ScreenView::ScreenView(QWidget *parent)
	: QWidget(parent)
{
	init();	
	initLabel();
	initColorBar();
	initToolBar();
	setMouseTracking(true);       //鼠标移动捕捉
	raise();                      //软置顶
	showFullScreen();             //画布全屏显示
}

ScreenView::~ScreenView()
{

}



void ScreenView::setBackGroundPixmap(QPixmap &px)
{
	_fullPixmap = px;
}

QRectF ScreenView::getRectF(QPointF p1, QPointF p2)
{
	qreal x1, y1, x2, y2;
	if (p1.x() < p2.x())
	{
		x1 = p1.x();
		x2 = p2.x();
	}
	else
	{
		x1 = p2.x();
		x2 = p1.x();
	}

	if (p1.y() < p2.y())
	{
		y1 = p1.y();
		y2 = p2.y();
	}
	else
	{
		y1 = p2.y();
		y2 = p1.y();
	}

	QPointF ps(x1, y1);
	QPointF pe(x2, y2);
	QRectF rect(ps, pe);
	return rect;
}

void ScreenView::setTopLeftAndButtomRight(QPointF &ptTL, QPointF &ptBR)
{
	qreal x1, y1, x2, y2;
	if (ptTL.x() < ptBR.x())
	{
		x1 = ptTL.x();
		x2 = ptBR.x();
	}
	else
	{
		x1 = ptBR.x();
		x2 = ptTL.x();
	}

	if (ptTL.y() < ptBR.y())
	{
		y1 = ptTL.y();
		y2 = ptBR.y();
	}
	else
	{
		y1 = ptBR.y();
		y2 = ptTL.y();
	}

	ptTL.setX(x1);
	ptTL.setY(y1);
	ptBR.setX(x2);
	ptBR.setY(y2);
}

CursorLocation ScreenView::caputerCursor(QRectF rect, qreal x, qreal y)
{
	QPointF ptTL = rect.topLeft();
	QPointF ptBR = rect.bottomRight();
	QPointF ptTR = rect.topRight();
	QPointF ptBL = rect.bottomLeft();

	if (x < ptTL.x() + MARGIN)
	{//left
		if (y < ptTL.y() + MARGIN)
		{
			return CursorLocation::TOPLEFT;
		}
		else if (y > ptBL.y() - MARGIN)
		{
			return CursorLocation::BUTTOMLEFT;
		}
		else
		{
			return CursorLocation::LEFT;
		}
	}
	else if (x > ptTR.x() - MARGIN)
	{//right
		if (y < ptTR.y() + MARGIN)
		{
			return CursorLocation::TOPRIGHT;
		}
		else if (y > ptBR.y() - MARGIN)
		{
			return CursorLocation::BUTTOMRIGHT;
		}
		else
		{
			return CursorLocation::RIGHT;
		}
	}
	else
	{//mid
		if (y < ptTL.y() + MARGIN)
		{
			return CursorLocation::TOP;
		}
		else if (y > ptBL.y() - MARGIN)
		{
			return CursorLocation::BUTTOM;
		}
		else
		{
			return CursorLocation::MIDDLE;
		}
	}
}

void ScreenView::copyImage()
{
	screenCapture(_shortArea);
	_clipboard->setPixmap(_shotPixmap);
	this->close();
}

void ScreenView::saveImage()
{
	screenCapture(_shortArea);
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMdd_hhmmss");
	QString save_file = "ScreenShot_" + current_date + ".jpg";

	QString file_name = QFileDialog::getSaveFileName(this, 
													 QStringLiteral("保存图片"), 
													 "./" + save_file, 
													 "JPEG File (*.jpg);;BMP File (*.bmp);;PNG File (*.png)");
	if (file_name.isEmpty())
		return;

	_shotPixmap.save(file_name);

	this->close();
}

void ScreenView::drawLine()
{
	if (DrawEditFlag::DRAWLINE != _draw_edit_flag)
	{
		_draw_edit_flag = DrawEditFlag::DRAWLINE;
		_btn_drawLine->setStyleSheet(s_pressStyle);
		_btn_drawRect->setStyleSheet(s_normalStyle);
		_btn_drawEllipse->setStyleSheet(s_normalStyle);
		_btn_drawText->setStyleSheet(s_normalStyle);
		showColorBar();
	}
	else
	{
		_draw_edit_flag = DrawEditFlag::DRAWDRAG;
		_btn_drawLine->setStyleSheet(s_normalStyle);
		hideColorBar();
	}
}

void ScreenView::drawRect()
{
	if (DrawEditFlag::DRAWRECT != _draw_edit_flag)
	{
		_draw_edit_flag = DrawEditFlag::DRAWRECT;
		_btn_drawRect->setStyleSheet(s_pressStyle);
		_btn_drawLine->setStyleSheet(s_normalStyle);
		_btn_drawEllipse->setStyleSheet(s_normalStyle);
		_btn_drawText->setStyleSheet(s_normalStyle);
		showColorBar();
	}
	else
	{
		_draw_edit_flag = DrawEditFlag::DRAWDRAG;
		_btn_drawRect->setStyleSheet(s_normalStyle);
		hideColorBar();
	}
}

void ScreenView::drawEllipse()
{
	if (DrawEditFlag::DRAWCIRCLE != _draw_edit_flag)
	{
		_draw_edit_flag = DrawEditFlag::DRAWCIRCLE;
		_btn_drawEllipse->setStyleSheet(s_pressStyle);
		_btn_drawLine->setStyleSheet(s_normalStyle);
		_btn_drawRect->setStyleSheet(s_normalStyle);
		_btn_drawText->setStyleSheet(s_normalStyle);
		showColorBar();
	}
	else
	{
		_draw_edit_flag = DrawEditFlag::DRAWDRAG;
		_btn_drawEllipse->setStyleSheet(s_normalStyle);
		hideColorBar();
	}
}

void ScreenView::drawTextStatus()
{
	if (DrawEditFlag::DRAWTEXT != _draw_edit_flag)
	{
		_draw_edit_flag = DrawEditFlag::DRAWTEXT;
		_btn_drawText->setStyleSheet(s_pressStyle);
		_btn_drawLine->setStyleSheet(s_normalStyle);
		_btn_drawRect->setStyleSheet(s_normalStyle);
		_btn_drawEllipse->setStyleSheet(s_normalStyle);
		showColorBar();
	}
	else
	{
		_draw_edit_flag = DrawEditFlag::DRAWDRAG;
		_btn_drawText->setStyleSheet(s_normalStyle);
		hideColorBar();
	}
}

void ScreenView::colorItemChanged(const QColor &color)
{
	assert(_cur_coloritem);
	_cur_coloritem->setColor(color);
	_pen_color = color;
}

void ScreenView::colorSelection()
{
	QColorDialog dia(this);
	dia.setWindowTitle(QStringLiteral("选择颜色"));
	dia.setCurrentColor(_cur_coloritem->getColor());
	if (dia.exec() == QColorDialog::Accepted)
	{
		QColor color = dia.selectedColor();
		_cur_coloritem->setColor(color);
		_pen_color = color;
	}
}

void ScreenView::pointSizeChanged(int point_size)
{
	_point_size = point_size;
}

void ScreenView::init()
{
	_ptS.rx() = -10;
	_ptS.ry() = -10;
	_ptE.rx() = -10;
	_ptE.ry() = -10;
	_bIsDrawLineEnd = false;

	_line_list.clear();
	_rect_list.clear();
	_ellipse_list.clear();
	_text_list.clear();

	setCursor(Qt::ArrowCursor);
	_draw_flag = DrawStatus::WAITDRAW;
	_curlocation = CursorLocation::INVALID;
	_draw_edit_flag = DrawEditFlag::DRAWDRAG;
	_pen_color = QColor(Qt::black);
	_point_size = 2;

	QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
	_screen_width = screenGeometry.width();
	_screen_height = screenGeometry.height();
	_clipboard = QApplication::clipboard();   //获取系统剪贴板指针
}

void ScreenView::initColorBar()
{
	_colorbar = new QWidget(this);
	ColorItem *coloritem1_1 = new ColorItem(QColor(0, 0, 0));
	ColorItem *coloritem1_2 = new ColorItem(QColor(127, 127, 127));
	ColorItem *coloritem1_3 = new ColorItem(QColor(136, 0, 21));
	ColorItem *coloritem1_4 = new ColorItem(QColor(237, 28, 36));
	ColorItem *coloritem1_5 = new ColorItem(QColor(255, 127, 39));
	ColorItem *coloritem1_6 = new ColorItem(QColor(255, 242, 0));
	ColorItem *coloritem1_7 = new ColorItem(QColor(34, 177, 76));
	ColorItem *coloritem1_8 = new ColorItem(QColor(0, 162, 232));
	ColorItem *coloritem1_9 = new ColorItem(QColor(63, 72, 204));
	ColorItem *coloritem1_10 = new ColorItem(QColor(163, 73, 164));
	QHBoxLayout *hBoxLayout1 = new QHBoxLayout();
	hBoxLayout1->addWidget(coloritem1_1);
	hBoxLayout1->addWidget(coloritem1_2);
	hBoxLayout1->addWidget(coloritem1_3);
	hBoxLayout1->addWidget(coloritem1_4);
	hBoxLayout1->addWidget(coloritem1_5);
	hBoxLayout1->addWidget(coloritem1_6);
	hBoxLayout1->addWidget(coloritem1_7);
	hBoxLayout1->addWidget(coloritem1_8);
	hBoxLayout1->addWidget(coloritem1_9);
	hBoxLayout1->addWidget(coloritem1_10);
	hBoxLayout1->setMargin(2);
	hBoxLayout1->setSpacing(2);

	connect(coloritem1_1,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_2,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_3,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_4,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_5,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_6,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_7,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_8,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_9,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem1_10, SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));

	ColorItem *coloritem2_1 = new ColorItem(QColor(255, 255, 255));
	ColorItem *coloritem2_2 = new ColorItem(QColor(195, 195, 195));
	ColorItem *coloritem2_3 = new ColorItem(QColor(185, 122, 87));
	ColorItem *coloritem2_4 = new ColorItem(QColor(255, 174, 201));
	ColorItem *coloritem2_5 = new ColorItem(QColor(255, 201, 14));
	ColorItem *coloritem2_6 = new ColorItem(QColor(239, 228, 176));
	ColorItem *coloritem2_7 = new ColorItem(QColor(181, 230, 29));
	ColorItem *coloritem2_8 = new ColorItem(QColor(153, 217, 234));
	ColorItem *coloritem2_9 = new ColorItem(QColor(112, 146, 190));
	ColorItem *coloritem2_10 = new ColorItem(QColor(200, 191, 231));
	QHBoxLayout *hBoxLayout2 = new QHBoxLayout();
	hBoxLayout2->addWidget(coloritem2_1);
	hBoxLayout2->addWidget(coloritem2_2);
	hBoxLayout2->addWidget(coloritem2_3);
	hBoxLayout2->addWidget(coloritem2_4);
	hBoxLayout2->addWidget(coloritem2_5);
	hBoxLayout2->addWidget(coloritem2_6);
	hBoxLayout2->addWidget(coloritem2_7);
	hBoxLayout2->addWidget(coloritem2_8);
	hBoxLayout2->addWidget(coloritem2_9);
	hBoxLayout2->addWidget(coloritem2_10);
	hBoxLayout2->setMargin(2);
	hBoxLayout2->setSpacing(2);

	connect(coloritem2_1,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_2,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_3,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_4,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_5,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_6,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_7,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_8,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_9,  SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));
	connect(coloritem2_10, SIGNAL(itemClicked(const QColor &)), this, SLOT(colorItemChanged(const QColor &)));

	QVBoxLayout *vBoxLayout = new QVBoxLayout();
	vBoxLayout->setMargin(1);
	vBoxLayout->setSpacing(0);
	vBoxLayout->addLayout(hBoxLayout1);
	vBoxLayout->addLayout(hBoxLayout2);

	_cur_coloritem = new ColorItem(QColor(0, 0, 0), 2 * ITEM_LENGTH + 2);
	PointSizeWidget *pointSizeWidget = new PointSizeWidget();
	QHBoxLayout *hBoxLayout = new QHBoxLayout();
	hBoxLayout->addWidget(pointSizeWidget);
	hBoxLayout->addWidget(_cur_coloritem);
	hBoxLayout->setMargin(1);
	hBoxLayout->setSpacing(2);

	connect(_cur_coloritem, SIGNAL(clicked()), this, SLOT(colorSelection()));
	connect(pointSizeWidget, SIGNAL(wheelscrolled(int)), this, SLOT(pointSizeChanged(int)));

	QHBoxLayout *mainBoxLayout = new QHBoxLayout();
	mainBoxLayout->addLayout(hBoxLayout);
	mainBoxLayout->addLayout(vBoxLayout);
	mainBoxLayout->setMargin(2);
	mainBoxLayout->setSpacing(2);

	_colorbar->setStyleSheet("background-color: rgb(34, 34, 37);");
	_colorbar->setCursor(Qt::ArrowCursor);
	_colorbar->setLayout(mainBoxLayout);
	_colorbar->setVisible(true);
	_colorbar->setVisible(false);
}

void ScreenView::showColorBar()
{
	assert(_colorbar);
	qreal x = _shortArea.topLeft().x();
	qreal y = _shortArea.bottomRight().y() + _toolbar->height() + 6;
	_colorbar->move(x, y);
	_colorbar->setVisible(true);
}

void ScreenView::hideColorBar()
{
	_colorbar->setVisible(false);
}

void ScreenView::initLabel()
{
	_label = new QLabel(this);
	_label->setStyleSheet("background-color: rgba(0, 0, 0, 50%);color: white;");
	_label->setAlignment(Qt::AlignHCenter);

	QFont font;
	font.setFamily(QStringLiteral("Microsoft YaHei"));
	_label->setFont(font);
}

void ScreenView::showLabel()
{
	assert(_label);
	qreal x = _shortArea.topLeft().x();
	qreal y = _shortArea.topLeft().y() - _label->height() - 2;
	qreal w = _shortArea.width();
	qreal h = _shortArea.height();
	QString tip = QStringLiteral("%1 × %2").arg(w).arg(h);
	_label->setText(tip);
	_label->adjustSize();
	_label->move(x, y);
	_label->setVisible(true);
}

void ScreenView::hideLabel()
{
	_label->setVisible(false);
}

void ScreenView::initToolBar()
{
	_toolbar = new QWidget(this);

	QHBoxLayout *mainToolBarLayout = new QHBoxLayout();
	_btn_copy = new QPushButton();
	_btn_save = new QPushButton();
	_btn_drawLine = new QPushButton();
	_btn_drawRect = new QPushButton();
	_btn_drawEllipse = new QPushButton();
	_btn_drawText = new QPushButton();
	
	_btn_copy->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	_btn_save->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	_btn_drawLine->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	_btn_drawRect->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	_btn_drawEllipse->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	_btn_drawText->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);

	_btn_copy->setIcon(QPixmap(":/image/copy.png"));
	_btn_save->setIcon(QPixmap(":/image/save.png"));
	_btn_drawLine->setIcon(QPixmap(":/image/drawline.png"));
	_btn_drawRect->setIcon(QPixmap(":/image/drawrect.png"));
	_btn_drawEllipse->setIcon(QPixmap(":/image/drawellipse.png"));
	_btn_drawText->setIcon(QPixmap(":/image/drawtext.png"));
	
	_btn_copy->setToolTip(QStringLiteral("复制到剪贴板"));
	_btn_save->setToolTip(QStringLiteral("保存到文件"));
	_btn_drawLine->setToolTip(QStringLiteral("绘制线段"));
	_btn_drawRect->setToolTip(QStringLiteral("绘制矩形"));
	_btn_drawEllipse->setToolTip(QStringLiteral("绘制椭圆"));
	_btn_drawText->setToolTip(QStringLiteral("添加文本"));

	_toolbar->setStyleSheet("border:none;background-color: rgb(255, 255, 255);");
	_btn_copy->setStyleSheet(s_normalStyle);
	_btn_save->setStyleSheet(s_normalStyle);
	_btn_drawLine->setStyleSheet(s_normalStyle);
	_btn_drawRect->setStyleSheet(s_normalStyle);
	_btn_drawEllipse->setStyleSheet(s_normalStyle);
	_btn_drawText->setStyleSheet(s_normalStyle);
	
	mainToolBarLayout->addWidget(_btn_drawLine);
	mainToolBarLayout->addWidget(_btn_drawRect);
	mainToolBarLayout->addWidget(_btn_drawEllipse);
	mainToolBarLayout->addWidget(_btn_drawText);
	mainToolBarLayout->addWidget(_btn_save);
	mainToolBarLayout->addWidget(_btn_copy);

	mainToolBarLayout->setContentsMargins(0, 0, 0, 0);
	mainToolBarLayout->setSpacing(0);
	
	connect(_btn_copy, SIGNAL(clicked()), this, SLOT(copyImage()));
	connect(_btn_save, SIGNAL(clicked()), this, SLOT(saveImage()));
	connect(_btn_drawLine, SIGNAL(clicked()), this, SLOT(drawLine()));
	connect(_btn_drawRect, SIGNAL(clicked()), this, SLOT(drawRect()));
	connect(_btn_drawEllipse, SIGNAL(clicked()), this, SLOT(drawEllipse()));
	connect(_btn_drawText, SIGNAL(clicked()), this, SLOT(drawTextStatus()));

	_toolbar->setCursor(Qt::ArrowCursor);
	_toolbar->setLayout(mainToolBarLayout);
	_toolbar->setVisible(true);
	_toolbar->setVisible(false);
}

void ScreenView::showToolBar()
{
	assert(_toolbar);
	qreal x = _shortArea.topLeft().x();
	qreal y = _shortArea.bottomRight().y()+4;
	_toolbar->move(x, y);
	_toolbar->setVisible(true);

}

void ScreenView::hideToolBar()
{
	assert(_toolbar);
	_toolbar->setVisible(false);
}

void ScreenView::screenCapture(const QRectF &rect)
{
	QScreen *screen = QGuiApplication::primaryScreen();
	_shotPixmap = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
}

void ScreenView::adjustShotScreen(QMouseEvent *event)
{
	QPointF pt_tl = _shortArea.topLeft();
	QPointF pt_br = _shortArea.bottomRight();
	qreal coord_x = event->x();
	qreal coord_y = event->y();
	qreal dx = coord_x - _ptDrag.x();
	qreal dy = coord_y - _ptDrag.y();

	CursorLocation curLocation = _curlocation;

	switch (curLocation)
	{
	case TOPLEFT:
		setCursor(Qt::SizeFDiagCursor);
		pt_tl.setX(coord_x);
		pt_tl.setY(coord_y);
		break;
	case TOP:
		setCursor(Qt::SizeVerCursor);
		pt_tl.setY(coord_y);
		break;
	case TOPRIGHT:
		setCursor(Qt::SizeBDiagCursor);
		pt_br.setX(coord_x);
		pt_tl.setY(coord_y);
		break;
	case LEFT:
		setCursor(Qt::SizeHorCursor);
		pt_tl.setX(coord_x);
		break;
	case MIDDLE:
		switch (_draw_edit_flag)
		{
		case DRAWLINE:
			_ptE.setX(coord_x);
			_ptE.setY(coord_y);
			_line_list.last().setP2(_ptE.toPoint());
			break;
		case DRAWRECT:
			_ptE.setX(coord_x);
			_ptE.setY(coord_y);
			_rect_list.last().setBottomRight(_ptE.toPoint());
			break;
		case DRAWCIRCLE:
			_ptE.setX(coord_x);
			_ptE.setY(coord_y);
			_ellipse_list.last().setBottomRight(_ptE.toPoint());
			break;
		case DRAWDRAG:
			setCursor(Qt::SizeAllCursor);
			_ptDrag.rx() = coord_x;
			_ptDrag.ry() = coord_y;
			if ((pt_tl.x() + dx > 0) && (pt_br.x() + dx < _screen_width))
			{
				pt_tl.rx() += dx;
				pt_br.rx() += dx;
			}
			if ((pt_tl.y() + dy > 0) && (pt_br.y() + dy < _screen_height))
			{
				pt_tl.ry() += dy;
				pt_br.ry() += dy;
			}
			break;
		default:
			break;
		}
		break;
	case RIGHT:
		setCursor(Qt::SizeHorCursor);
		pt_br.setX(coord_x);
		break;
	case BUTTOMLEFT:
		setCursor(Qt::SizeBDiagCursor);
		pt_tl.setX(coord_x);
		pt_br.setY(coord_y);
		break;
	case BUTTOM:
		setCursor(Qt::SizeVerCursor);
		pt_br.setY(coord_y);
		break;
	case BUTTOMRIGHT:
		setCursor(Qt::SizeFDiagCursor);
		pt_br.setX(coord_x);
		pt_br.setY(coord_y);
		break;
	default:
		break;
	}

	if (pt_tl.x() > pt_br.x() || pt_tl.y() > pt_br.y())
	{//如果矩形范围左上大于右下则进行调整
		if (pt_tl.x() > pt_br.x() && pt_tl.y() > pt_br.y())
		{
			switch (_curlocation)
			{
			case TOPLEFT:
				_curlocation = BUTTOMRIGHT;
				break;
			case TOPRIGHT:
				_curlocation = BUTTOMLEFT;
				break;
			case BUTTOMLEFT:
				_curlocation = TOPRIGHT;
				break;
			case BUTTOMRIGHT:
				_curlocation = TOPLEFT;
				break;
			default:
				break;
			}
		}
		else if (pt_tl.x() > pt_br.x())
		{
			switch (_curlocation)
			{
			case TOPLEFT:
				_curlocation = TOPRIGHT;
				break;
			case TOPRIGHT:
				_curlocation = TOPLEFT;
				break;
			case LEFT:
				_curlocation = RIGHT;
				break;
			case RIGHT:
				_curlocation = LEFT;
				break;
			case BUTTOMLEFT:
				_curlocation = BUTTOMRIGHT;
				break;
			case BUTTOMRIGHT:
				_curlocation = BUTTOMLEFT;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (_curlocation)
			{
			case TOPLEFT:
				_curlocation = BUTTOMLEFT;
				break;
			case TOP:
				_curlocation = BUTTOM;
				break;
			case TOPRIGHT:
				_curlocation = BUTTOMRIGHT;
				break;
			case BUTTOMLEFT:
				_curlocation = TOPLEFT;
				break;
			case BUTTOM:
				_curlocation = TOP;
				break;
			case BUTTOMRIGHT:
				_curlocation = TOPRIGHT;
				break;
			default:
				break;
			}
		}
		setTopLeftAndButtomRight(pt_tl, pt_br);
	}

	_shortArea.setTopLeft(pt_tl);
	_shortArea.setBottomRight(pt_br);
}

void ScreenView::drawText(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();
	TextPaint *curText = new TextPaint(this);
	int size = _text_list.length();
	for (int i = 0; i < size; i++)
	{
		_text_list[i]->clearFocus();
	}

	QPalette palette;
	palette.setColor(QPalette::Text, _pen_color);
	curText->setPalette(palette);
	curText->setStyleSheet("background:transparent;border-width:0;border-style:outset");

	curText->setGeometry(x, y, 30, 30);
	curText->setVisible(true);
	curText->setFocus();
	_text_list.append(curText);
	QWidget::mousePressEvent(event);
	update();
}

void ScreenView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton)
	{
		if (DrawStatus::WAITDRAW == _draw_flag)
		{
			_ptS.rx() = event->x();
			_ptS.ry() = event->y();
			_ptE.rx() = event->x();
			_ptE.ry() = event->y();
			_shortArea = getRectF(_ptS, _ptE);
			_draw_flag = DrawStatus::DRAWINIG;
		}
		else if (DrawStatus::DRAWEND == _draw_flag)
		{
			_ptDrag.rx() = event->x();
			_ptDrag.ry() = event->y();

			QPointF pt_tl = _shortArea.topLeft();
			QPointF pt_br = _shortArea.bottomRight();
			
			qreal coord_x = event->x();
			qreal coord_y = event->y();
			qreal dx = coord_x - _ptDrag.x();
			qreal dy = coord_y - _ptDrag.y();

			CursorLocation curLocation = _curlocation;
			Qt::CursorShape curshape = cursor().shape();
			if (Qt::ForbiddenCursor == curshape)
				return;

			switch (curLocation)
			{
			case TOPLEFT:
				setCursor(Qt::SizeFDiagCursor);
				pt_tl.setX(coord_x);
				pt_tl.setY(coord_y);
				break;
			case TOP:
				setCursor(Qt::SizeVerCursor);
				pt_tl.setY(coord_y);
				break;
			case TOPRIGHT:
				setCursor(Qt::SizeBDiagCursor);
				pt_br.setX(coord_x);
				pt_tl.setY(coord_y);
				break;
			case LEFT:
				setCursor(Qt::SizeHorCursor);
				pt_tl.setX(coord_x);
				break;
			case MIDDLE:
				switch (_draw_edit_flag)
				{
				case DRAWLINE:
					_bIsDrawLineEnd = false;
					_ptS.rx() = event->x();
					_ptS.ry() = event->y();
					_ptE.rx() = event->x();
					_ptE.ry() = event->y();
					_line_list.append(LinePaint(QLine(_ptS.toPoint(), _ptE.toPoint()), QPen(_pen_color, _point_size, Qt::SolidLine)));
					break;
				case DRAWRECT:
					_ptS.rx() = event->x();
					_ptS.ry() = event->y();
					_ptE.rx() = event->x();
					_ptE.ry() = event->y();
					_rect_list.append(RectPaint(_ptS.toPoint(), _ptE.toPoint(), QPen(_pen_color, _point_size, Qt::SolidLine)));
					break;
				case DRAWCIRCLE:
					_ptS.rx() = event->x();
					_ptS.ry() = event->y();
					_ptE.rx() = event->x();
					_ptE.ry() = event->y();
					_ellipse_list.append(EllipsePaint(_ptS.toPoint(), _ptE.toPoint(), QPen(_pen_color, _point_size, Qt::SolidLine)));
					break;
				case DRAWTEXT:
					drawText(event);
					break;
				case DRAWDRAG:
					setCursor(Qt::SizeAllCursor);
					_ptDrag.rx() = coord_x;
					_ptDrag.ry() = coord_y;
					if ((pt_tl.x() + dx > 0) && (pt_br.x() + dx < _screen_width))
					{
						pt_tl.rx() += dx;
						pt_br.rx() += dx;
					}
					if ((pt_tl.y() + dy > 0) && (pt_br.y() + dy < _screen_height))
					{
						pt_tl.ry() += dy;
						pt_br.ry() += dy;
					}
					break;
				default:
					break;
				}
				break;
			case RIGHT:
				setCursor(Qt::SizeHorCursor);
				pt_br.setX(coord_x);
				break;
			case BUTTOMLEFT:
				setCursor(Qt::SizeBDiagCursor);
				pt_tl.setX(coord_x);
				pt_br.setY(coord_y);
				break;
			case BUTTOM:
				setCursor(Qt::SizeVerCursor);
				pt_br.setY(coord_y);
				break;
			case BUTTOMRIGHT:
				setCursor(Qt::SizeFDiagCursor);
				pt_br.setX(coord_x);
				pt_br.setY(coord_y);
				break;
			default:
				break;
			}

			_shortArea.setTopLeft(pt_tl);
			_shortArea.setBottomRight(pt_br);
		}
	}
	else if (event->button() & Qt::RightButton)
	{
		if (DrawEditFlag::DRAWLINE == _draw_edit_flag)
		{
			if (_line_list.size() != 0 && !_bIsDrawLineEnd)
			{
				_line_list.removeLast();
				_ptS.setX(-10);
				_ptS.setY(-10);
				_bIsDrawLineEnd = true;
			}
		}
		else
		{
// 			init();
// 			hideToolBar();
// 			hideColorBar();
// 			hideLabel();
// 			update();
		}
	}
}

void ScreenView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		if (DrawStatus::DRAWINIG == _draw_flag)
		{
			_ptE.setX(event->x());
			_ptE.setY(event->y());
			_shortArea = getRectF(_ptS, _ptE);
		}
		else if (DrawStatus::DRAWEND == _draw_flag)
		{
			if (Qt::ForbiddenCursor == cursor().shape())
				return;
			adjustShotScreen(event);
		}

		update();
	}
	else
	{
		if (DrawStatus::DRAWEND == _draw_flag)
		{
			CursorLocation curLocation = caputerCursor(_shortArea, event->x(), event->y());
			_curlocation = curLocation;

			switch (curLocation)
			{
			case TOPLEFT:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeFDiagCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case TOP:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeVerCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case TOPRIGHT:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeBDiagCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case LEFT:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeHorCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case MIDDLE:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeAllCursor) : setCursor(Qt::ArrowCursor);
				if (DrawEditFlag::DRAWLINE == _draw_edit_flag)
				{
					_ptE.setX(event->x());
					_ptE.setY(event->y());
					if (!_bIsDrawLineEnd && _ptS.x() > -1 && _ptS.y() > -1 && _line_list.size()>0)
						_line_list.last().setP2(_ptE.toPoint());
				}
				update();
				break;
			case RIGHT:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeHorCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case BUTTOMLEFT:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeBDiagCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case BUTTOM:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeVerCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			case BUTTOMRIGHT:
				DrawEditFlag::DRAWDRAG == _draw_edit_flag ? setCursor(Qt::SizeFDiagCursor) : setCursor(Qt::ForbiddenCursor);
				break;
			default:
				break;
			}
		}
	}
}

void ScreenView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton)
	{
		if (DrawStatus::DRAWINIG == _draw_flag)
		{
			_ptE.setX(event->x());
			_ptE.setY(event->y());
			_shortArea = getRectF(_ptS, _ptE);

			_draw_flag = DrawStatus::DRAWEND;
			showToolBar();
		}
		else if (DrawStatus::DRAWEND == _draw_flag)
		{
			switch (_draw_edit_flag)
			{
			case DRAWLINE:
// 				if (qAbs(_ptS.x() - _ptE.x()) < 1.0e-3 && qAbs(_ptS.y() - _ptE.y()) < 1.0e-3)
// 					_line_list.removeLast();
				break;
			case DRAWRECT:
				if (qAbs(_ptS.x() - _ptE.x()) < 1.0e-3 && qAbs(_ptS.y() - _ptE.y()) < 1.0e-3)
					_rect_list.removeLast();
				break;
			case DRAWCIRCLE:
				if (qAbs(_ptS.x() - _ptE.x()) < 1.0e-3 && qAbs(_ptS.y() - _ptE.y()) < 1.0e-3)
					_ellipse_list.removeLast();
				break;
			case DRAWDRAG:
				break;
			default:
				break;
			}
		}
		update();
	}
}

void ScreenView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
		close();

	QWidget::keyPressEvent(event);
}

void ScreenView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true); //设置渲染提示为消除锯齿

	QPixmap tempmask(_screen_width, _screen_width);
	tempmask.fill((QColor(0, 0, 0, 160)));
	painter.drawPixmap(0, 0, _fullPixmap);     //先绘制全屏原图背景
	painter.drawPixmap(0, 0, tempmask);        //然后绘制半透明背景，用来降低亮度

	switch (_draw_flag)
	{
	case WAITDRAW:
		break;
	case DRAWINIG:
	{
		painter.setPen(penScreenShotBound);
		painter.drawRect(_shortArea);
		painter.drawPixmap(_shortArea, _fullPixmap, _shortArea);
		showLabel();
		break;
	}
	case DRAWEND:
	{
		painter.setPen(penScreenShotBound);            //设置画笔形式
		painter.drawRect(_shortArea);                                //然后绘制矩形框
		painter.drawPixmap(_shortArea, _fullPixmap, _shortArea);     //然后将矩形框中的半透明图像替换成原图
		showToolBar();
		showLabel();
		break;
	}
	default:
		break;
	}

	int size = _line_list.length();
	for (int i = 0; i < size; i++)
	{
		painter.setPen(_line_list[i].getPen());
		painter.drawLine(_line_list[i]);
	}

	size = _rect_list.length();
	for (int i = 0; i < size; i++)
	{
		painter.setPen(_rect_list[i].getPen());
		painter.drawRect(_rect_list[i]);
	}

	size = _ellipse_list.length();
	for (int i = 0; i < size; i++)
	{
		painter.setPen(_ellipse_list[i].getPen());
		painter.drawEllipse(_ellipse_list[i]);
	}

	QWidget::paintEvent(event);
}
