#ifndef SNAPPIT_H
#define SNAPPIT_H

#include <QWidget>
#include "ui_snappit.h"
#include "imageview.h"
#include "screenview.h"
#include "logging.h"

class snappit : public QWidget
{
	Q_OBJECT

public:
	snappit(QWidget *parent = 0);
	~snappit();

public slots:
	void openImage();
	void screenShotCut();

private:
	void logInit();

protected:
	void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private:
	Ui::snappit ui;
	std::vector<ImageView *> _arr_imgView;
	QString _file_path;
};

#endif // SNAPPIT_H
