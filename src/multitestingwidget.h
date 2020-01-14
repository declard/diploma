#ifndef MULTITESTINGWIDGET_H
#define MULTITESTINGWIDGET_H

#include <QtGui/QWidget>
#include "neocognitron.h"
#include <QRunnable>


namespace Ui {
	class MultitestingWidget;
}

class MultitestingWidget : public QWidget, public QRunnable {
	Q_OBJECT
	Neocognitron*neo;
	int rec,notrec,confused;
	bool testing;
	struct Fail {
		QString klass;
		QString badKlass;
		MatrixC data;
		Fail() {}
		Fail(QString k,QString b,MatrixC d):
				klass(k),badKlass(b),data(d) {}
	};
	QVector<Fail> fails;
	MatrixIndex translation;
	double noise;
public:
	MultitestingWidget(QWidget *parent = 0);
	~MultitestingWidget();

	void setData(Neocognitron*c);
protected:
	void closeEvent(QCloseEvent*);
	void changeEvent(QEvent *e);
	void run();
	void test();
private:
	Ui::MultitestingWidget *m_ui;

private slots:
	void on_spinBox_valueChanged(int );
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();

	void onBatchFinished();
signals:
	void batchFinished();
	void progress(int);
	void state(QString);
};

#endif // MULTITESTINGWIDGET_H
