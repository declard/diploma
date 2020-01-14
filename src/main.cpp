#include <QApplication>
#include "MainWindow.h"
#include "messagehandler.h"
#include <QThreadPool>
#include "matrix.h"
#include <QTextCodec>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForName("cp1251"));
	QThreadPool pool;
	int tc=QThread::idealThreadCount();
	if (tc==-1) tc=1;
	pool.setMaxThreadCount(tc+1);
	MainWindow w;
	w.show();
	return a.exec();
}
