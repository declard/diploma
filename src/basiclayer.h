#ifndef LAYER_H
#define LAYER_H

#include <QVector>
#include <QImage>
#include "matrix.h"

#include <QDebug>
#include "imageviewerwidget.h"
#include <QMessageBox>

#define USE_PARALLEL

typedef QVector<MatrixD> Data;

class BasicLayer {
public:
	typedef Data Data;
	static QImage toImage(const MatrixD&);
	static MatrixD fromImage(const QImage&);

	static inline double phi(double x) { return qMax(x,0.); }
	static inline double psi(double x) { return phi(x)/(1+phi(x)); }

	int threads;
	MatrixIndex explicitBorderOffset;
protected:
	static double bound(const MatrixD&matrix,MatrixIndex point);
public:
	virtual Data activate(Data) const=0;
	virtual int Kin() const=0;
	virtual int Kout() const=0;
	virtual MatrixIndex boundariesOffset(const MatrixD&matr) const;
	static MatrixIndex boundariesOffset(const MatrixD&f,const MatrixIndex&t);
	static MatrixIndex boundariesOffset(const MatrixIndex&f,const MatrixIndex&t);
	virtual MatrixIndex size() const=0;
	static int calcN(double A,int Np,int Nt);

	int threadCount() const { return threads; }
	void setThreadCount(int c) { if (c>=1) threads=c; }
};

bool printMatrix(MatrixD);
class SLayer;
void printLayer(SLayer*);

#endif // LAYER_H
