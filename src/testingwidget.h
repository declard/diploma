#ifndef TESTINGPARAMSWIDGET_H
#define TESTINGPARAMSWIDGET_H

#include <QtGui/QWidget>
#include "neocognitron.h"

namespace Ui {
	class TestingWidget;
}

class TestingWidget : public QWidget {
	Q_OBJECT
	QVector<BasicLayer::Data> data;
	int layer;
	Neocognitron*neo;
public:
	TestingWidget(QWidget *parent = 0);
	~TestingWidget();

	void setData(Neocognitron*);
protected:
	void changeEvent(QEvent *e);

private:
	Ui::TestingWidget *m_ui;

private slots:
	void on_buttonSave_clicked();
 void on_spinBox_valueChanged(int );
	void on_spinBox_3_valueChanged(int );
	void on_spinBox_2_valueChanged(int );
	void on_pushButton_clicked();
	void brushSizeChanged(int sz);
	void on_buttonLoad_clicked();
};

#endif // TESTINGPARAMSWIDGET_H
