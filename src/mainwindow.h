#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QMainWindow>
#include "imageeditorwidget.h"
#include "neocognitron.h"
#include "networktrainer.h"
#include "parameterscontainer.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
	Ui::MainWindow ui;
public:
	MainWindow();

protected:
	NetworkTrainer*thread,*nthread;
	Neocognitron*neo,*nneo;
	ParametersContainer*container,*ncontainer;
	Parameters*nparametersWidget;

	void setControlsEnabled(bool);
	void setActionsEnabled(bool);

	void closeEvent(QCloseEvent*);

	bool haveValidNet;
private slots:
	void on_actionDebug_triggered();
	void on_actionCreate_triggered();
	void on_actionLoad_triggered();
	void on_actionSave_triggered();

	void debugWidgetClosed();

	void trainingComplete();
	void trainingCanceled();
};

#endif // MAINWINDOW_H
