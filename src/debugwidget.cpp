#include "debugwidget.h"
#include "ui_debugwidget.h"

DebugWidget::DebugWidget(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::DebugWidget) {
	m_ui->setupUi(this);
	installAsHandler();
}

DebugWidget::~DebugWidget() {
	delete m_ui;
}

void DebugWidget::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void DebugWidget::installAsHandler() {
	installMessageHandler(this);
	connect(this,SIGNAL(newMessage(QString)),
			this,SLOT(addHandledMessage(QString)),
			Qt::QueuedConnection);
}

void DebugWidget::closeEvent(QCloseEvent*) {
	installMessageHandler(0);
	closed();
}

void DebugWidget::handleMessage(QtMsgType type,const char*msg) {
	(void)type;
	newMessage(msg);
}

void DebugWidget::addHandledMessage(QString msg) {
	m_ui->plainTextEdit->appendPlainText(msg);
}
