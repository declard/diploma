#ifndef SORGTRAINER_H
#define SORGTRAINER_H

#include "layer.h"
#include "slayer.h"
#include "basictrainer.h"
#include <QPair>

class SOrgTrainer : public BasicTrainer {
	Q_OBJECT
public:
	BasicLayer::Data data;
	typedef Layer::Data Data;

	double r;			// Сенс нейронов
	double q;			// Скорость обучения
	int times;			// How many times to present patterns
	QVector<Data>previousData;	// Data from prev. layer for every pattern
	Data patterns;
	MatrixIndex D;
	double co;
	double asgamma;
	double rini;
	int KMax;
	MatrixD cl;

	MatrixD SResIni(Data uc);
	void sorg(Data pdata);
	MatrixD inhibit(MatrixD ut,MatrixD uf);
public:
	static void enforcePlane(SLayer*sl,Data pdata,double q,int kseed,MatrixIndex nseed,MatrixIndex  bo);
	SOrgTrainer(QVector<Data> prevData);
	virtual void train(BasicLayer*slayer);
	virtual ~SOrgTrainer() {}
signals:
	void progress(int);
};

#endif // SORGTRAINER_H
