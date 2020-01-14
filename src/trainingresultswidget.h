#ifndef TRAININGRESULTSWIDGET_H
#define TRAININGRESULTSWIDGET_H

#include <QtGui/QWidget>
#include "neocognitron.h"

namespace Ui {
	class TrainingResultsWidget;
}

class TrainingResultsWidget : public QWidget {
	Q_OBJECT
	Neocognitron*neo;
	int l;
	int kin;
	int kout;
	void showPlane();
public:
	TrainingResultsWidget(QWidget *parent = 0);
	~TrainingResultsWidget();

	void setData(Neocognitron*c);
protected:
	void changeEvent(QEvent *e);

private:
	Ui::TrainingResultsWidget *m_ui;

private slots:
	void on_spinBox_3_valueChanged(int );
	void on_spinBox_valueChanged(int );
	void on_spinBox_2_valueChanged(int );
};

#endif // TRAININGRESULTSWIDGET_H
