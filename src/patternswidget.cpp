#include "patternswidget.h"
#include "ui_patternswidget.h"
#include <QFileDialog>

#include <QDebug>
PatternsWidget::PatternsWidget(QWidget *parent) :
	QWidget(parent),m_ui(new Ui::PatternsWidget) {
	m_ui->setupUi(this);
	connect(m_ui->patternEditor,SIGNAL(brushChanged(int)),this,SLOT(lineWidthChanged()));
	regexp=QRegExp(m_ui->lineEdit->text());
	regexp2=QRegExp(m_ui->lineEdit_2->text());
}

PatternsWidget::~PatternsWidget() {
	delete m_ui;
}

void PatternsWidget::closeEvent(QCloseEvent*) {
	neo->patterns[selected].data=m_ui->patternEditor->data();
}

void PatternsWidget::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PatternsWidget::save() {
	QListWidget*lw=m_ui->listWidget;
	neo->patterns[lw->row(lw->currentItem())].data=m_ui->patternEditor->data();
	QList<QListWidgetItem*>items=lw->selectedItems();
	for(int i=0;i<items.size();i++) {
		int p=m_ui->listWidget->row(items.at(i));
		const Pattern&pat=neo->patterns[p];
		QString file=QFileDialog::getSaveFileName(
			this,
			"Сохранить как",
			QString("../in/%1.%2.bmp").
				arg(pat.klass).
				arg(pat.name));
		if (file.isEmpty()) break;
		Neocognitron::toImage(neo->patterns.at(p).data).save(file);
	}
}

void PatternsWidget::load() {
	QStringList files=QFileDialog::getOpenFileNames(this,"Открыть","../in");
	if (files.isEmpty()) return;
	foreach(QString file,files) {
		QImage im;
		MatrixC m;

		if (!im.load(file,"GIF")&&!im.load(file,"BMP")) continue;
		QString cat,fname(file.section('/',-1));
		if (regexp.indexIn(fname)==-1)
			cat=fname.left(1);
		else cat=regexp.cap(1);

		im=im.scaled(neo->p->N0,neo->p->N0);
		//im.invertPixels();
		m=Neocognitron::fromImage(im);
		bool learn=(regexp2.indexIn(fname)!=-1);
		Pattern p(file,MatrixC(m),cat,learn,!learn);
		neo->patterns+=p;
		m_ui->listWidget->insertItem(m_ui->listWidget->count(),file);
		if (!klasses.contains(cat)) {
			klasses.insert(cat);
			m_ui->comboBox->addItem(cat);
		}
	}
}

void PatternsWidget::clear() {
	m_ui->patternEditor->fill(0);
}

void PatternsWidget::unload() {
	QListWidget*lw=m_ui->listWidget;
	QList<QListWidgetItem*>items=lw->selectedItems();
	QVector<int>toRemove(items.size());
	if (items.size()==lw->count()) {
		selected=-1;
		m_ui->patternEditor->fill(0);
	}
	for(int i=0;i<items.size();i++) {
		int p=m_ui->listWidget->row(items.at(i));
		toRemove[i]=p;
		delete lw->takeItem(p);
	}
	qSort(toRemove);
	while(!toRemove.isEmpty()) {
		neo->patterns.remove(toRemove.last());
		toRemove.resize(toRemove.size()-1);
	}
}

void PatternsWidget::lineWidthChanged() {
	m_ui->spinBox->setValue(m_ui->patternEditor->brushSize);
}

void PatternsWidget::lineWidthChanged(int s) {
	m_ui->patternEditor->brushSize=s;
}

void PatternsWidget::patternSelected(int i) {
	ImageEditorWidget*pe=m_ui->patternEditor;
	if (i==-1) {
		selected=-1;
		m_ui->patternEditor->fill(0);
		return;
	}
	if (selected!=-1) {
		neo->patterns[selected].data=pe->data();
		selected=i;
	}
	else selected=i;
	Pattern p(neo->patterns.at(selected));
	pe->setData(p.data);
	m_ui->comboBox->setCurrentIndex(m_ui->comboBox->findText(p.klass));
	m_ui->checkBox->setChecked(p.train);
	m_ui->checkBox_2->setChecked(p.test);
}

void PatternsWidget::classChanged(QString klass) {
	neo->patterns[selected].klass=klass;
}

void PatternsWidget::classAdded(QString klass) {
	m_ui->comboBox->addItem(klass);
}

// create new
void PatternsWidget::on_pushButton_5_clicked() {
	QString name=QString("%1").arg(uniqueImageId++,4,10,QChar('0'));
	neo->patterns+=Pattern(name,MatrixIndex(neo->p->N0),m_ui->comboBox->currentText(),true,true);
	neo->patterns.last().data.fill(0);
	m_ui->listWidget->insertItem(m_ui->listWidget->count(),name);
	m_ui->listWidget->setCurrentRow(m_ui->listWidget->count()-1);
}

void PatternsWidget::on_checkBox_toggled(bool checked) {
	neo->patterns[selected].train=checked;
}

void PatternsWidget::on_checkBox_2_toggled(bool checked) {
	neo->patterns[selected].test=checked;
}

void PatternsWidget::on_lineEdit_textChanged(QString expr) {
	regexp=QRegExp(expr);
}

void PatternsWidget::setData(Neocognitron*c) {
	m_ui->comboBox->clear();
	selected=-1;
	m_ui->listWidget->clear();
	uniqueImageId=0;
	neo=c;
	Neocognitron::Patterns ival(c->patterns);
	klasses.clear();
	for(int ii=0;ii<ival.size();ii++) {
		uniqueImageId++;
		Neocognitron::Pattern p(ival.at(ii));
		m_ui->listWidget->insertItem(m_ui->listWidget->count(),p.name);
		QString klass=p.klass;
		if (!klasses.contains(klass)) {
			classAdded(klass);
			klasses.insert(klass);
		}
	}
	if (ival.size()) {
		m_ui->patternEditor->setData(neo->patterns.at(0).data);
		selected=0;
		m_ui->listWidget->setCurrentRow(0);
	}
}

void PatternsWidget::on_lineEdit_2_textChanged(QString s) {
	regexp2=QRegExp(s);
}
