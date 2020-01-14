#include "lastclayer.h"
#include <QtConcurrentRun>
#include <QFutureSynchronizer>

MatrixD LastCLayer::calcUc(const MatrixD&Us) const {
	MatrixIndex n,f,t(Us.size());
	MatrixD Uc(1);
	/*double max=0.0;
	for2DIncr(n,f,t) {
		double v=phi(Us.at(n));
		if (v>max) max=v;
	}
	Uc[f]=max;*/
	double sum=0.0;
	for2DIncr(n,f,t) {
		sum+=phi(Us.at(n));
	}
	Uc[f]=sum;
	return Uc;
}

#if defined(USE_PARALLEL)
Data LastCLayer::activate(Data data) const {
	QFutureSynchronizer<Data> futures;
	for(int i=0;i<threads;i++)
		futures.addFuture(QtConcurrent::run(this,&LastCLayer::parActivate,data,i,threads));
	futures.waitForFinished();
	Data res(data.size());
	QList< QFuture<Data> >results(futures.futures());
	for(int i=0;i<data.size();i++)
		res[i]=results[i%threads].result()[i/threads];
	return res;
}

Data LastCLayer::parActivate(Data data,int parOffset,int parStep) const {
	int Ks=data.size();
	Data res;
	res.reserve(Ks);
	for(int ks=parOffset;ks<Ks;ks+=parStep)
		res+=calcUc(data.at(ks));
	return res;
}
#else
Data LastCLayer::activate(Data data) const {
	int Ks=data.size();
	Data res;
	res.reserve(Ks);
	for(int ks=0;ks<Ks;ks++)
		res+=calcUc(data.at(ks));
	return res;
}
#endif
