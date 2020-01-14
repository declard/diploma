#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
	class ProgressWidget;
}

class ProgressWidget : public QWidget {
	Q_OBJECT
public:
	ProgressWidget(QWidget *parent = 0,bool noCancel=false);
	~ProgressWidget();
protected:
	void changeEvent(QEvent *e);
private:
	Ui::ProgressWidget *m_ui;
signals:
	void cancel();
public slots:
	void setProgress(int);
	void setState(QString);
};

#endif // PROGRESSWIDGET_H
