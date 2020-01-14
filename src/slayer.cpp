#include "slayer.h"
#include <math.h>
#include <QtConcurrentRun>
#include <QFutureSynchronizer>

double SLayer::c(MatrixIndex v) const {
	return cs.at(v);
}

SLayer::SLayer(
		MatrixIndex ReceptiveField,
		MatrixIndex PlaneSize,
		double selectivity,
		MatrixD aini
) {
	S=PlaneSize;
	A=ReceptiveField;
	r=selectivity;
	bDefaultValue=0.0;
	m_Kin=0;
	setADefaultValue(aini);
	double sum=0.0;
	MatrixIndex i,f,t(aini.size());
	for2DIncr(i,f,t)
		sum+=aini.at(i)*aini.at(i);
	bDefaultValue=sqrt(sum);
	explicitBorderOffset=MatrixIndex(0);
}

MatrixD SLayer::makeC(int Na,double A,double gamma,double co) {
	MatrixIndex n,f,t(Na);
	double	x, y, r2;
	double	an = (Na - 1) * 0.5;
	double	A2 = A * A;

	MatrixD r(t);
	for2DIncr(n,f,t) {
		y = n.y() - an;
		x = n.x() - an;
		r2 = y * y + x * x;
		if (r2 <= A2) {
			r[n] = co * pow(gamma, r2 / A2);
		} else {
			r[n] = 0.0;
		}
	}
	return r;
}

void SLayer::setADefaultValue(MatrixD m) {
	aDefaultValue=m;
	aDefaultValues=Data(Kin(),m);
}

double SLayer::calcUvAt(MatrixIndex n,Layer::Data UcS,MatrixIndex bo) const {
	double sum=0;
	int Kc=UcS.size();
	MatrixIndex offset(n-Ar()-bo),v,f;
	for(int kc=0;kc<Kc;kc++) {
		const MatrixD&Uc=UcS.at(kc);
		for2DIncr(v,f,A)
			sum+=c(v)*sqr(Uc.at(v+offset,0.0));
	}
	return sqrt(sum);
}

MatrixD SLayer::calcUv(Layer::Data UcS) const {
	MatrixIndex boundOffset=boundariesOffset(UcS.at(0).size())+explicitBorderOffset;
	MatrixIndex n,f,At=Ar();
	//qDebug()<<"V"<<(MatrixIndex(0)-Ar()-boundOffset).toString()<<(S+size()-Ar()-boundOffset).toString()<<size().toString()<<UcS.at(0).size().toString()<<A.toString();
	MatrixD Uv(S);
	DMatrixD DUv(Uv);
	for2DIncr(n,f,S) {
		DUv[n]=calcUvAt(n,UcS,boundOffset);
	}
	return Uv;
}

double SLayer::calcUsAt(MatrixIndex n,Layer::Data UcS,Data planea,MatrixIndex bo) const {
	if (!UcS.size()) return 0.0;
	MatrixIndex f,v,At=Ar();
	MatrixIndex offset(n-At-bo);
	double sum=0.0;
	int Kc=UcS.size();
	Q_ASSERT(planea.size()==UcS.size());
	for(int kc=0;kc<Kc;kc++) {
		MatrixD a=planea.at(kc);
		MatrixD Uc=UcS.at(kc);
		Q_ASSERT(A==a.size());
		for2DIncr(v,f,A)
			sum+=a.at(v)*Uc.at(v+offset,0.0);
	}
	return sum;
}

MatrixD SLayer::calcUs(Layer::Data UcS,int k,MatrixD Uv) const {
	MatrixIndex boundOffset(boundariesOffset(UcS.at(0))+explicitBorderOffset);
	//qDebug()<<"S"<<(MatrixIndex(0)-Ar()-boundOffset).toString()<<(A+size()-Ar()-boundOffset).toString()<<"this"<<size().toString()<<"another"<<UcS.at(0).size().toString()<<A.toString();
	MatrixIndex n,f;
	const Plane&plane=planes.at(k);
	double var0=plane.b*r/(r+1);
	MatrixD Us(S);
	DMatrixD DUs(Us);
	for2DIncr(n,f,S) {
		double sum=calcUsAt(n,UcS,plane.a,boundOffset);
		DUs[n]=r*phi((1+sum)/(1+var0*Uv.at(n))-1);
	}
	return Us;
}

#if defined(USE_PARALLEL)
Layer::Data SLayer::activate(Layer::Data data) const {
	MatrixIndex boundOffset=boundariesOffset(data.at(0));
	MatrixD UvS=calcUv(data);
	QFutureSynchronizer<Data> futures;
	for(int i=0;i<threads;i++)
		futures.addFuture(QtConcurrent::run(this,&SLayer::parActivate,data,UvS,i,threads));
	futures.waitForFinished();
	Data res(Kout());
	QList< QFuture<Data> >results(futures.futures());
	for(int i=0;i<Kout();i++)
		res[i]=results[i%threads].result()[i/threads];
	return res;
}

Layer::Data SLayer::parActivate(Layer::Data data,MatrixD Uv,int parOffset,int parStep) const {
	int Kc=Kout();
	Q_ASSERT(data.size()==Kin());
	Data res;
	res.reserve(Kc);
	for(int kc=parOffset;kc<Kc;kc+=parStep)
		res+=calcUs(data,kc,Uv);
	return res;
}
#else
Layer::Data SLayer::activate(Layer::Data data) const {
	MatrixD Uv=calcUv(data);
	int Ks=data.size();
	(void)Ks;
	int Kc=Kout();
	Q_ASSERT(Ks==m_Kin);
	Data res;
	res.reserve(Kc);
	for(int kc=0;kc<Kc;kc++)
		res+=calcUs(data,kc,Uv);
	return res;
}
#endif

void SLayer::setKin(int k) {
	int diff=k-m_Kin;
	if (diff>0) {
		for(int i=0;i<planes.size();i++) {
			Data&plane=planes[i].a;
			plane.insert(plane.end(),diff,aDefaultValue);
		}
		aDefaultValues.insert(aDefaultValues.end(),diff,aDefaultValue);
	}
	else {
		for(int i=0;i<planes.size();i++)
			planes[i].a.resize(k);
		aDefaultValues.resize(k);
	}
	m_Kin=k;
}

void SLayer::setKout(int k) {
	int diff=k-planes.size();
	if (diff>0) planes.insert(planes.end(),diff,Plane(aDefaultValues,bDefaultValue));
	else planes.resize(k);
}
