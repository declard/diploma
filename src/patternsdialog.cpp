#include "patternsdialog.h"
#include "ui_patternsdialog.h"

PatternsDialog::PatternsDialog(QWidget *parent) :
		QDialog(parent),
		m_ui(new Ui::PatternsDialog)
{
	m_ui->setupUi(this);
}

PatternsDialog::~PatternsDialog()
{
	delete m_ui;
}

void PatternsDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

PatternsWidget*PatternsDialog::patterns() const {
	return m_ui->widget;
}
