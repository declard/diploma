#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include <QtGui/QDialog>
#include "parameters.h"

namespace Ui {
	class CreateDialog;
}

class CreateDialog : public QDialog {
	Q_OBJECT
public:
	CreateDialog(QWidget *parent = 0);
	~CreateDialog();

	Parameters*takeParameters(QWidget*parent);
protected:
	void changeEvent(QEvent *e);

private:
	Ui::CreateDialog *m_ui;
};

#endif // CREATEDIALOG_H
