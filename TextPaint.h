#ifndef TEXTPAINT_H
#define TEXTPAINT_H

#include <QLineEdit>

class TextPaint : public QLineEdit
{
	Q_OBJECT

public:
	TextPaint(QWidget *parent = Q_NULLPTR);
	~TextPaint();

public slots:
	void slt_autoResize();
	void slt_editEnd();
	
};

#endif // TEXTPAINT_H
