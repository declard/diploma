#include "createdialog.h"
#include "ui_createdialog.h"

CreateDialog::CreateDialog(QWidget *parent) :
		QDialog(parent),
		m_ui(new Ui::CreateDialog) {
	m_ui->setupUi(this);
	m_ui->widget->setData(new ParametersContainer(m_ui->widget));
}

CreateDialog::~CreateDialog() {
	delete m_ui;
}

void CreateDialog::changeEvent(QEvent *e) {
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

Parameters*CreateDialog::takeParameters(QWidget*parent) {
	m_ui->widget->setParent(parent);
	m_ui->widget->setReadOnly();
	return m_ui->widget;
}
