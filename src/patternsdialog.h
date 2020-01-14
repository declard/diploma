#ifndef PATTERNSDIALOG_H
#define PATTERNSDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
	class PatternsDialog;
}

class PatternsWidget;
class PatternsDialog : public QDialog {
	Q_OBJECT
public:
	PatternsDialog(QWidget *parent = 0);
	~PatternsDialog();

	PatternsWidget*patterns() const;
protected:
	void changeEvent(QEvent *e);

private:
	Ui::PatternsDialog *m_ui;
};

#endif // PATTERNSDIALOG_H
