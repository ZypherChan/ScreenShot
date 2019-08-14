#include "snappit.h"
#include <QtWidgets/QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	snappit w;
	w.switchLanguage("zh");
	a.setQuitOnLastWindowClosed(false);

	return a.exec();
}
