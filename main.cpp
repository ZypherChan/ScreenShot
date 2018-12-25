#include "snappit.h"
#include <QtWidgets/QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	snappit w;
	QTranslator *pQTranslator = new QTranslator(&w);
	pQTranslator->load(":/translation/qt_zh_CN.qm");
	QCoreApplication::installTranslator(pQTranslator);
	w.show();

	return a.exec();
}
