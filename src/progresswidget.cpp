#include "progresswidget.h"
#include "ui_progresswidget.h"

ProgressWidget::ProgressWidget(QWidget *parent,bool noCancel) :
		QWidget(parent),
		m_ui(new Ui::ProgressWidget) {
	m_ui->setupUi(this);
	if (noCancel) m_ui->pushButton->setEnabled(false);
	else {
		connect(m_ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
		connect(m_ui->pushButton,SIGNAL(clicked()),this,SIGNAL(cancel()));
	}
}

ProgressWidget::~ProgressWidget() {
	delete m_ui;
}

void ProgressWidget::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void ProgressWidget::setProgress(int v) {
	m_ui->progressBar->setValue(v);
}

void ProgressWidget::setState(QString s) {
	m_ui->label->setText(s);
}
