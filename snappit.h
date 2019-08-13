#ifndef SNAPPIT_H
#define SNAPPIT_H

#include <QWidget>
#include "ui_snappit.h"
#include "imageview.h"
#include "screenview.h"
#include "logging.h"
#include <QSystemTrayIcon>
#include "QMenu"

class snappit : public QWidget
{
	Q_OBJECT

public:
	snappit(QWidget *parent = 0);
	~snappit();

public:
	void openImage();
	void screenShotCut();
	void trayMenuTrigged(QAction* action);
	void trayActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void logInit();

protected:
	void hideEvent(QHideEvent *event)     Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *event)	  Q_DECL_OVERRIDE;

private:
	Ui::snappit ui;
	std::vector<ImageView *> _arr_imgView;
	QString _file_path;
	QSystemTrayIcon *m_tray;
	QMenu *m_trayMenu;
};

#endif // SNAPPIT_H
