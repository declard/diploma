#include "mainwindow.h"
#include <QtGui>
#include <QHash>
#include "patternswidget.h"
#include "debugwidget.h"
#include "createdialog.h"
#include "progresswidget.h"
#include "patternsdialog.h"

MainWindow::MainWindow():QMainWindow() {
	ui.setupUi(this);
	container=new ParametersContainer(this);
	ui.parametersWidget->setData(container);
	neo=new Neocognitron(container);
	thread=new NetworkTrainer(neo,container,this);
	haveValidNet=false;
	setControlsEnabled(false);
	setActionsEnabled(true);
	ui.parametersWidget->setReadOnly();
	nneo=0; ncontainer=0; nthread=0;
	nparametersWidget=0;
}

void MainWindow::closeEvent(QCloseEvent*) {
	//trainingCanceled();
	qApp->quit();
}

void MainWindow::on_actionDebug_triggered() {
	DebugWidget*dw=new DebugWidget();
	connect(dw,SIGNAL(closed()),this,SLOT(debugWidgetClosed()),Qt::QueuedConnection);
	dw->show();
	ui.actionDebug->setEnabled(false);
}

void MainWindow::on_actionCreate_triggered() {
	CreateDialog cd(this);
	cd.exec();
	if (cd.result()==QDialog::Rejected) return;
	nparametersWidget=cd.takeParameters(0);
	ncontainer=nparametersWidget->container();
	nneo=new Neocognitron(ncontainer);
	PatternsDialog pd(this);
	pd.patterns()->setData(nneo);
	pd.exec();
	if (pd.result()==QDialog::Rejected ||
		nneo->patterns.size()==0) {
		delete nneo; nneo=0;
		ncontainer=0;
		delete nparametersWidget;
		nparametersWidget=0;
		return;
	}
	setActionsEnabled(false);
	nthread=new NetworkTrainer(nneo,ncontainer,this);
	ProgressWidget*pw=new ProgressWidget;
	connect(pw,SIGNAL(cancel()),this,SLOT(trainingCanceled()));
	connect(nthread,SIGNAL(progress(int)),pw,SLOT(setProgress(int)),Qt::QueuedConnection);
	connect(nthread,SIGNAL(state(QString)),pw,SLOT(setState(QString)),Qt::QueuedConnection);
	connect(nthread,SIGNAL(finished()),this,SLOT(trainingComplete()));
	connect(nthread,SIGNAL(finished()),pw,SLOT(close()));
	pw->show();
	nthread->start();
}

void MainWindow::on_actionLoad_triggered() {
	QString fileName=QFileDialog::getOpenFileName(this,"Загрузить");
	if (fileName.isEmpty()) return;
	QFile loadFile(fileName);
	loadFile.open(QIODevice::ReadOnly);
	QDataStream lstream(&loadFile);
	container->load(lstream);
	delete neo;
	neo=new Neocognitron(container);
	neo->load(lstream);
	haveValidNet=true;
	setControlsEnabled(true);
	setActionsEnabled(true);

	container->reset();
	ui.patternsWidget->setData(neo);
	ui.interactiveWidget->setData(neo);
	ui.batchWidget->setData(neo);
	ui.weightsWidget->setData(neo);
}

void MainWindow::on_actionSave_triggered() {
	QString fileName=QFileDialog::getSaveFileName(this,"Сохранить как");
	if (fileName.isEmpty()) return;
	QFile saveFile(fileName);
	saveFile.open(QIODevice::WriteOnly|QIODevice::Truncate);
	QDataStream sstream(&saveFile);
	container->save(sstream);
	neo->save(sstream);
}

void MainWindow::debugWidgetClosed() {
	ui.actionDebug->setEnabled(true);
}

void MainWindow::trainingComplete() {
	haveValidNet=true;
	setControlsEnabled(true);
	setActionsEnabled(true);

	nparametersWidget->setParent(ui.parametersTab);

	if (neo!=nneo) delete neo;
	if (thread!=nthread) delete thread;
	if (container!=ncontainer) delete container;
	delete ui.parametersWidget;
	ui.parametersWidget=nparametersWidget;
	ui.parametersLayout->addWidget(ui.parametersWidget);
	neo=nneo; container=ncontainer; thread=nthread;

	ui.patternsWidget->setData(neo);
	ui.interactiveWidget->setData(neo);
	ui.batchWidget->setData(neo);
	ui.weightsWidget->setData(neo);
}

void MainWindow::trainingCanceled() {
	delete nparametersWidget;
	nparametersWidget=0;
	if (nthread) nthread->terminate();
	setActionsEnabled(true);

	if (nneo!=neo) delete nneo;
	if (nthread!=thread) delete nthread;
	nneo=0; nthread=0;
}

void MainWindow::setControlsEnabled(bool enabled) {
	ui.weightsTab->setEnabled(enabled);
	ui.interactiveTab->setEnabled(enabled);
	ui.batchTab->setEnabled(enabled);
	ui.patternsTab->setEnabled(enabled);
}

void MainWindow::setActionsEnabled(bool enabled) {
	ui.actionCreate->setEnabled(enabled);
	ui.actionLoad->setEnabled(enabled);
	ui.actionSave->setEnabled(enabled && haveValidNet);
}
