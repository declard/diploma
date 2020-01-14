#ifndef PATTERNSWIDGET_H
#define PATTERNSWIDGET_H

#include <QtGui/QWidget>
#include "neocognitron.h"
#include <QLabel>
#include <QListWidgetItem>
#include <QSet>
#include <QRegExp>

namespace Ui {
	class PatternsWidget;
}

class PatternsWidget : public QWidget {
	Q_OBJECT
	int uniqueImageId;
	Neocognitron*neo;
	int selected;
	QSet<QString> klasses;
	QRegExp regexp,regexp2;

	typedef Neocognitron::Pattern Pattern;
	typedef Neocognitron::Patterns Patterns;

public:
	PatternsWidget(QWidget *parent = 0);
	~PatternsWidget();

	void setData(Neocognitron*c);
protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent*event);

private:
	Ui::PatternsWidget *m_ui;

	void select(int i);
public slots:
	void save();
	void load();
	void clear();
	void unload();
	void lineWidthChanged(int);
	void lineWidthChanged();
	void patternSelected(int);
	void classChanged(QString);
	void classAdded(QString);

private slots:
	void on_lineEdit_2_textChanged(QString );
 void on_lineEdit_textChanged(QString );
	void on_checkBox_2_toggled(bool checked);
	void on_checkBox_toggled(bool checked);
	void on_pushButton_5_clicked();
};

#endif // PATTERNSWIDGET_H
