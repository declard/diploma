#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QtGui/QWidget>
#include "messagehandler.h"

namespace Ui {
	class DebugWidget;
}

class DebugWidget : public QWidget,public MessageHandler {
	Q_OBJECT
public:
	DebugWidget(QWidget *parent = 0);
	~DebugWidget();
	void installAsHandler();
protected:
	void changeEvent(QEvent *e);
	void handleMessage(QtMsgType type,const char*msg);
	void closeEvent(QCloseEvent*);
private:
	Ui::DebugWidget *m_ui;
private slots:
	void addHandledMessage(QString);
signals:
	void newMessage(QString);
	void closed();
};

#endif // DEBUGWIDGET_H
