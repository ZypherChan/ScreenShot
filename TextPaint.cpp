#include "TextPaint.h"
#include <QFontMetrics>

TextPaint::TextPaint(QWidget *parent)
	: QLineEdit(parent)
{
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(slt_autoResize()));
	connect(this, SIGNAL(editingFinished()), this, SLOT(slt_editEnd()));
}

TextPaint::~TextPaint()
{

}

void TextPaint::slt_autoResize()
{
	QFontMetrics fm(this->font());
	this->setFixedWidth(fm.width(this->text()) + 6);
	this->setFixedHeight(fm.height() + 2);
}

void TextPaint::slt_editEnd()
{
	this->setDisabled(true);
}
