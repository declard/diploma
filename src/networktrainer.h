#ifndef NETWORKTRAINER_H
#define NETWORKTRAINER_H

#include <QThread>
#include "neocognitron.h"

class NetworkTrainer : public QThread {
	Q_OBJECT
	Neocognitron*cognitron;
	ParametersContainer*p;
	void run() { train(); }
public:
	NetworkTrainer(Neocognitron*c,ParametersContainer*par,QObject*parent):
			cognitron(c),p(par),QThread(parent) {}
	void train();
signals:
	void progress(int);
	void state(QString);
};

#endif // NETWORKTRAINER_H
