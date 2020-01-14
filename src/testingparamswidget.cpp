#include "testingparamswidget.h"
#include "ui_testingparamswidget.h"

TestingWidget::TestingWidget(Neocognitron*cognitron,QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::TestingWidget) {
	m_ui->setupUi(this);
	this->cognitron=cognitron;
	m_ui->spinBox->setMaximum(cognitron->trained*2);
	MatrixC mc(MatrixIndex(cognitron->N0),0);
	MatrixIndex c=cognitron->N0/2,f(3,3),t(62,62),n;
	/*for2DIncr(n,f,t)
		mc[n]=255;*/
	m_ui->widget->setData(mc);
	layer=0;
	connect(m_ui->widget,SIGNAL(brushChanged(int)),this,SLOT(brushSizeChanged(int)));
}

TestingWidget::~TestingWidget() {
	delete m_ui;
}

void TestingWidget::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void TestingWidget::on_pushButton_clicked() {
	data.clear();
	BasicLayer::Data d;
	d+=toMatrixD(m_ui->widget->data());
	d=cognitron->glayer->activate(d);
	data+=d;
	m_ui->spinBox_2->setMaximum(d.size()-1);
	QVector<Layer*>layers(cognitron->layers);
	for(int i=0;i<cognitron->trained;i++) {
		d=layers.at(i)->slayer->activate(d);
		data+=d;
		d=layers.at(i)->clayer->activate(d);
		data+=d;
	}
	//m_ui->spinBox_2->setMaximum(data.size()-1);
	if (m_ui->spinBox->value()!=0)
		m_ui->spinBox->setValue(0);
	else on_spinBox_valueChanged(0);
}

void TestingWidget::on_spinBox_2_valueChanged(int v) {
	MatrixD md(data.at(layer).at(v));
	m_ui->plainTextEdit->setPlainText(md.toString());
	m_ui->widget_2->setData(toMatrixC(md));
}

void TestingWidget::on_spinBox_3_valueChanged(int v) {
	m_ui->widget->brushSize=v;
}

void TestingWidget::on_spinBox_valueChanged(int v) {
	layer=v;
	m_ui->spinBox_2->setMaximum(data.at(layer).size()-1);
	if (m_ui->spinBox_2->value()!=0)
		m_ui->spinBox_2->setValue(0);
	else on_spinBox_2_valueChanged(0);
}

void TestingWidget::brushSizeChanged(int sz) {
	m_ui->spinBox_3->setValue(sz);
}
