#include "trainingresultswidget.h"
#include "ui_trainingresultswidget.h"


TrainingResultsWidget::TrainingResultsWidget(QWidget *parent) :
	QWidget(parent), m_ui(new Ui::TrainingResultsWidget) {
	neo=0;
	m_ui->setupUi(this);
}

TrainingResultsWidget::~TrainingResultsWidget() {
	delete m_ui;
}

void TrainingResultsWidget::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void TrainingResultsWidget::showPlane() {
	SLayer::Plane plane=neo->layers[l]->slayer->planes[kout];
	m_ui->lineEdit->setText(QString::number(plane.b));
	m_ui->widget->setData(toMatrixC(plane.a[kin]));
}

void TrainingResultsWidget::on_spinBox_2_valueChanged(int v) {
	l=v;
	m_ui->spinBox->setMaximum(neo->layers[l]->slayer->Kout()-1);
	m_ui->spinBox_3->setMaximum(neo->layers[l]->slayer->Kin()-1);
	m_ui->spinBox->setValue(0);
	m_ui->spinBox_3->setValue(0);
	showPlane();
}

void TrainingResultsWidget::on_spinBox_valueChanged(int v) {
	kout=v;
	showPlane();
}

void TrainingResultsWidget::on_spinBox_3_valueChanged(int v) {
	kin=v;
	showPlane();
}

void TrainingResultsWidget::setData(Neocognitron*c) {
	neo=c;
	l=kout=kin=0;
	m_ui->spinBox_2->setMaximum(neo->layers.size()-1);
	on_spinBox_2_valueChanged(0);
}
