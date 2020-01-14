#include "testingwidget.h"
#include "ui_testingwidget.h"
#include <QFileDialog>

TestingWidget::TestingWidget(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::TestingWidget) {
	m_ui->setupUi(this);
	layer=0;
	neo=0;
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
	d=neo->glayer->activate(d);
	data+=d;
	m_ui->spinBox->setMaximum(neo->trained*2);
	m_ui->spinBox_2->setMaximum(d.size()-1);
	QVector<Layer*>layers(neo->layers);
	for(int i=0;i<neo->trained;i++) {
		d=layers.at(i)->slayer->activate(d);
		data+=d;
		d=layers.at(i)->clayer->activate(d);
		data+=d;
	}
	//m_ui->spinBox_2->setMaximum(data.size()-1);
	if (m_ui->spinBox->value()!=0)
		m_ui->spinBox->setValue(0);
	else on_spinBox_valueChanged(0);
	QPair< QString,QVector<double> > res=neo->getClass(data.last());
	m_ui->label->setText(res.first);
}

void TestingWidget::on_spinBox_2_valueChanged(int v) {
	MatrixD md(data.at(layer).at(v));
	//m_ui->plainTextEdit->setPlainText(md.toString());
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

void TestingWidget::setData(Neocognitron*newNeo) {
	this->setEnabled(true);
	neo=newNeo;
	layer=0;
	MatrixC mc(MatrixIndex(neo->p->N0),0);
	/*MatrixIndex c=neo->p->N0/2,f(3,3),t(62,62),n;
	for2DIncr(n,f,t)
		mc[n]=255;*/
	m_ui->widget->setData(mc);
}

void TestingWidget::on_buttonLoad_clicked() {
	QString file=QFileDialog::getOpenFileName(this,"Open");
	if (file.isEmpty()) return;
	QImage im(file);
	if (im.isNull()) return;
	int sz=neo->p->N0;
	im=im.scaled(sz,sz);
	//im.invertPixels();
	m_ui->widget->setData(Neocognitron::fromImage(im));
}

void TestingWidget::on_buttonSave_clicked() {
	QString file=QFileDialog::getSaveFileName(this,"Сохранить как");
	if (file.isEmpty()) return;
	Neocognitron::toImage(m_ui->widget->data()).save(file);
}
