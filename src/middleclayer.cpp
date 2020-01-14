#include "middleclayer.h"
#include <math.h>
#include <QtConcurrentRun>
#include <QFutureSynchronizer>

double MiddleCLayer::d(MatrixIndex v) const {
	return ds.at(v);
}

void MiddleCLayer::initDs(
	int		Na,
	double	A,
	double	Ao,
	double	gamma,
	double	gammao,
	double	gammap,
	double	co,
	double	pn
	)
{
	ds=MatrixD(MatrixIndex(Na));
	DMatrixD Dds(ds);
	int		i, j;
	double	x, y, y2, r2, Spos, Sneg, SSneg, cc, C, Co, CCo, CC;
	double	an = (Na - 1) * 0.5;
	double	A2 = A * A;
	double	Ao2 = Ao * Ao;

	if (Ao2 < A2) {
		C = 1 / A2;
		Co = 1 / Ao2;
		CCo = co * Co / (Co - C);
		CC = co * C / (Co - C);
	}

	Spos = 0.0;
	Sneg = 0.0;
	for (i = 0; i < Na; i++) {
		y = i - an;
		y2 = y * y;
		for (j = 0; j < Na; j++) {
			MatrixIndex n(i,j);
			x = j - an;
			r2 = y2 + x * x;
			if (r2 <= Ao2) {
				cc = co * pow(gamma, r2 / Ao2);
				Dds[n] = cc;
				Spos += cc;
			} else if (r2 <= A2) {
				cc = CCo * pow(gammao, r2 / Ao2) - CC * pow(gammap, r2 / A2);
				Dds[n] = cc;
				if (cc > 0.0)  Spos += cc;
				else           Sneg -= cc;
			} else {
				ds[n] = 0.0;
			}
		}
	}
	if (Ao2 < A2) {
		SSneg = pn * Spos / Sneg;
		for (i = 0; i < Na; i++) {
			for (j = 0; j < Na; j++) {
				if (Dds[MatrixIndex(i,j)] < 0.0) {
					Dds[MatrixIndex(i,j)] *= SSneg;
				}
			}
		}
	}
}

MiddleCLayer::MiddleCLayer(
	MatrixIndex ReceptiveField,
	MatrixIndex PlaneSize,
	MatrixIndex CompressFactor
) {
	C=PlaneSize;
	D=ReceptiveField;
	factor=CompressFactor;
	threads=2;
	explicitBorderOffset=MatrixIndex(0);
}

MatrixD MiddleCLayer::calcUc(const MatrixD&Us) const {
	MatrixIndex n,v,f,t(C),Dt=Dr();
	MatrixIndex boundOffset(boundariesOffset(Us)/factor+explicitBorderOffset);
	//qDebug()<<"C"<<(MatrixIndex(0)-Dr()-boundOffset).toString()<<(D+t*factor-Dr()-boundOffset).toString()<<t.toString()<<Us.size().toString()<<D.toString();
	MatrixD Uc(C);
	DMatrixD DUc(Uc);
	double sum;
	for2DIncr(n,f,C) {
		sum=0;
		MatrixIndex offset(factor*n-Dt+boundOffset);
		for2DIncr(v,f,D)
			sum+=d(v)*Us.at(v+offset,0.0);
		DUc[n]=psi(sum);
	}
	return Uc;
}

#if defined(USE_PARALLEL)
Data MiddleCLayer::activate(Data data) const {
	QFutureSynchronizer<Data> futures;
	for(int i=0;i<threads;i++)
		futures.addFuture(QtConcurrent::run(this,&MiddleCLayer::parActivate,data,i,threads));
	futures.waitForFinished();
	Data res(data.size());
	QList< QFuture<Data> >results(futures.futures());
	for(int i=0;i<data.size();i++)
		res[i]=results[i%threads].result()[i/threads];
	return res;
}

Data MiddleCLayer::parActivate(Data data,int parOffset,int parStep) const {
	int Ks=data.size();
	Data res;
	res.reserve(Ks);
	for(int ks=parOffset;ks<Ks;ks+=parStep)
		res+=calcUc(data.at(ks));
	return res;
}
#else
Data MiddleCLayer::activate(Data data) const {
	int Ks=data.size();
	Data res;
	res.reserve(Ks);
	for(int ks=0;ks<Ks;ks++)
		res+=calcUc(data.at(ks));
	return res;
}
#endif
