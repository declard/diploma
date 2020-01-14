#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QtGui/QWidget>
#include "parameterscontainer.h"

namespace Ui {
	class Parameters;
}

class Parameters : public QWidget {
	Q_OBJECT
	ParametersContainer*m_container;
public:
	Parameters(QWidget *parent = 0);
	~Parameters();

	void setReadOnly();
	void setData(ParametersContainer*c);
	ParametersContainer*container() { return m_container; }
protected:
	void changeEvent(QEvent *e);

private:
	Ui::Parameters *m_ui;

private slots:
	void on_N0_valueChanged(int v);
	void on_LC_valueChanged(int v);
	void on_TC_valueChanged(int v);
	void on_Ng_valueChanged(int v);
	void on_gamcg_valueChanged(double v);
	void on_Ag_valueChanged(double v);
	void on_Ago_valueChanged(double v);
	void modelReset();
};

#endif // PARAMETERS_H
