#include "parameters.h"
#include "ui_parameters.h"

Parameters::Parameters(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::Parameters) {
	m_ui->setupUi(this);
	QTableView*tv=m_ui->tableView;
	m_container=0;
	tv->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	tv->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	tv->setItemDelegate(new ParametersContainerDelegate(tv));
}

Parameters::~Parameters() {
	delete m_ui;
}

void Parameters::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void Parameters::setReadOnly() {
	qobject_cast<ParametersContainer*>(m_ui->tableView->model())->setReadOnly();
	m_ui->N0->setReadOnly(true);
	m_ui->LC->setReadOnly(true);
	m_ui->TC->setReadOnly(true);
	m_ui->Ag->setReadOnly(true);
	m_ui->Ago->setReadOnly(true);
	m_ui->Ng->setReadOnly(true);
	m_ui->gamcg->setReadOnly(true);
}

void Parameters::on_N0_valueChanged(int v) {
	m_container->N0=v;
}

void Parameters::on_LC_valueChanged(int v) {
	m_container->layersCountChanged(v);
}

void Parameters::on_TC_valueChanged(int v) {
	m_container->threads=v;
}

void Parameters::on_Ng_valueChanged(int v) {
	m_container->Ng=v;
}

void Parameters::on_gamcg_valueChanged(double v) {
	m_container->gamcg=v;
}

void Parameters::on_Ag_valueChanged(double v) {
	m_container->Ag=v;
}

void Parameters::on_Ago_valueChanged(double v) {
	m_container->Ago=v;
}

void Parameters::modelReset() {
	m_ui->N0->setValue(m_container->N0);
	m_ui->Ng->setValue(m_container->Ng);
	m_ui->TC->setValue(m_container->threads);
	m_ui->gamcg->setValue(m_container->gamcg);
	m_ui->Ag->setValue(m_container->Ag);
	m_ui->Ago->setValue(m_container->Ago);
	m_ui->LC->setValue(m_container->layersCount());
}

void Parameters::setData(ParametersContainer*c) {
	m_container=c;
	m_ui->tableView->setModel(m_container);
	connect(m_container,SIGNAL(modelReset()),this,SLOT(modelReset()));
	modelReset();
}
