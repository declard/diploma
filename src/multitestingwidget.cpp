#include "multitestingwidget.h"
#include "ui_multitestingwidget.h"
#include <QThreadPool>
#include "progresswidget.h"

MultitestingWidget::MultitestingWidget(QWidget *parent) :
		QWidget(parent),m_ui(new Ui::MultitestingWidget) {
	m_ui->setupUi(this);
	neo=0;
	testing=true;
	m_ui->spinBox->setEnabled(false);
	setAutoDelete(false);
	noise=0.3;
	translation=MatrixIndex(15,15);
}

MultitestingWidget::~MultitestingWidget() {
	delete m_ui;
}

void MultitestingWidget::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

QHash<QString, QVector<BasicLayer::Data> > makeData(Neocognitron::Patterns patterns,bool learn);

void MultitestingWidget::run() {
	rec=0,notrec=0,confused=0;
	Neocognitron::Patterns patterns=neo->patterns;
	/*if (translation!=0) {
		MatrixIndex n,f,t(patterns.at(0).data.size());
		for(int i=0;i<patterns.size();i++) {
			MatrixC mci=patterns.at(i).data,mco(mci.size());
			for2DIncr(n,f,t)
				mco[n]=mci.at(n+translation,0.0);
			patterns[i].data=mco;
		}
	}
	if (noise>0.00001) {
		int thr=noise*RAND_MAX;
		MatrixIndex n,f,t(patterns.at(0).data.size());
		for(int i=0;i<patterns.size();i++) {
			MatrixC&mc=patterns[i].data;
			for2DIncr(n,f,t)
				if (qrand()<thr)
					mc[n]=~mc.at(n);
		}
	}*/

	for(int i=0;i<patterns.size();i++) {
		QImage im(Neocognitron::toImage(patterns.at(i).data));
		im=im.transformed(QTransform().scale(2,2));
		patterns[i].data=Neocognitron::fromImage(im);
	}

	QHash<QString, QVector<BasicLayer::Data> >data=makeData(patterns,!testing);
	QHashIterator<QString, QVector<BasicLayer::Data> > i(data);
	qDebug()<<neo->klassMap;
	int cs=0,ts=0;
	while(i.hasNext()) {
		i.next();
		ts+=i.value().size();
	}
	i.toFront();
	while(i.hasNext()) {
		i.next();
		QString klass=i.key();
		state("Checking class "+klass);
		QVector<BasicLayer::Data> dd=i.value();
		foreach(BasicLayer::Data d,dd) {
			progress(100.0/double(ts)*double(cs));
			QPair< QString,QVector<double> > res=neo->getClass(neo->activate(d));
			qDebug()<<cs<<"of"<<ts;
			qDebug()<<"Class"<<klass<<"similarity"<<res.second<<"chosen"<<res.first;
			if (res.first.isEmpty()) {
				qDebug()<<"Not recognized";
				fails<<Fail(klass,res.first,toMatrixC(d.at(0)));
				notrec++;
			}
			else if (res.first==klass) {
				qDebug()<<"Recognized";
				rec++;
			}
			else {
				qDebug()<<"Misclassified";
				fails<<Fail(klass,res.first,toMatrixC(d.at(0)));
				confused++;
			}
			cs++;
		}
	}
	qDebug()<<"rec"<<rec<<"not rec"<<notrec<<"conf"<<confused;
	batchFinished();
}

void MultitestingWidget::test() {
	if (!neo) return;
	fails.clear();
	m_ui->pushButton->blockSignals(true);
	m_ui->pushButton_2->blockSignals(true);
	m_ui->spinBox->setEnabled(false);
	ProgressWidget*pw=new ProgressWidget(0,true);
	connect(this,SIGNAL(progress(int)),pw,SLOT(setProgress(int)),Qt::QueuedConnection);
	connect(this,SIGNAL(state(QString)),pw,SLOT(setState(QString)),Qt::QueuedConnection);
	connect(this,SIGNAL(batchFinished()),pw,SLOT(close()),Qt::QueuedConnection);
	connect(this,SIGNAL(batchFinished()),this,SLOT(onBatchFinished()),Qt::QueuedConnection);
	pw->show();
	QThreadPool::globalInstance()->start(this);
	//run();
}

void MultitestingWidget::on_pushButton_clicked() {
	testing=true;
	qDebug()<<"On testing set";
	test();
}

void MultitestingWidget::on_pushButton_2_clicked() {
	testing=false;
	qDebug()<<"On training set";
	test();
}

void MultitestingWidget::closeEvent(QCloseEvent*) {
}

void MultitestingWidget::on_spinBox_valueChanged(int v) {
	m_ui->klassLine->setText(fails.at(v).klass);
	m_ui->badKlassLine->setText(fails.at(v).badKlass);
	m_ui->widget->setData(fails.at(v).data);
}

void MultitestingWidget::onBatchFinished() {
	if (!fails.isEmpty()) {
		m_ui->spinBox->setEnabled(true);
		m_ui->spinBox->setMaximum(fails.size()-1);
		on_spinBox_valueChanged(0);
	}
	m_ui->pushButton->blockSignals(false);
	m_ui->pushButton_2->blockSignals(false);
	m_ui->lineEdit->setText(QString::number(rec));
	m_ui->lineEdit_2->setText(QString::number(confused));
	m_ui->lineEdit_3->setText(QString::number(notrec));
}

void MultitestingWidget::setData(Neocognitron*c) {
	neo=c;
	fails.clear();
}
