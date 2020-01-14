#include "sorgtrainer.h"
#include "layer.h"
#include <math.h>
#include <QHash>

#include <QDebug>
#include <QFormLayout>

bool printMatrix(MatrixD md) {
	if (md.size()==0) return false;
	ImageViewerWidget*v=new ImageViewerWidget();
			MatrixIndex n,f,t(md.size());
			double min=md[f],max=min;
			for2DIncr(n,f,t) {
				if (md[n]<min)
					min=md[n];
				if (md[n]>max)
					max=md[n];
			}
			if (max!=min)
				for2DIncr(n,f,t) {
					md[n]=(md[n]-min)/(max-min)*255;
				}
				v->setData(md);
				v->resize(400,400);
				v->show();
				//return true;
			//}
			return false;
}

void printLayer(SLayer*sl) {
	for(int p=0;p<sl->Kout();p++) {
		for(int k=0;k<sl->Kin();k++) {
			MatrixD md(sl->planes[p].a[k]);
			qDebug()<<sl->planes[p].b;
			printMatrix(md);
		}
	}
	//QMessageBox::information(0,"1","");
}

SOrgTrainer::SOrgTrainer(QVector<Data> prevData) {
	previousData=prevData;
}

void SOrgTrainer::train(BasicLayer*slayer) {
	sl=dynamic_cast<SLayer*>(slayer);
	double r(r);
	qSwap(r,sl->r);
	int ps=previousData.size();
	for(int t=0;t<times;t++) {
		for(int p=0;p<ps;p++) {
			qDebug()<<"SOrg:"<<p<<"of"<<ps;
			progress(100.0/double(times*ps)*(t*ps+p));
			sorg(previousData.at(p));
		}
	}
	qSwap(r,sl->r);
}

void SOrgTrainer::sorg(Layer::Data pdata) {
	MatrixD Uv=sl->calcUv(pdata);
	Data us=sl->activate(pdata);
	MatrixIndex boundOffset=sl->boundariesOffset(pdata.at(0).size())+sl->explicitBorderOffset;
	QVector< QPair<int,MatrixIndex> > ss=findSeeds(us,D);
	for(int s=0;s<ss.size();s++)
		enforcePlane(sl,pdata,q,ss.at(s).first,ss.at(s).second,boundOffset);
	us=sl->activate(pdata);
	MatrixD usini=SResIni(pdata);
	for(int ks=0;ks<us.size();ks++)
		usini=inhibit(usini,us.at(ks));
	MatrixIndex oldseed(-1);
	for(int ks=us.size();ks<KMax;ks++) {
		MatrixIndex seed=findSeedIni(usini);
		if (seed==-1) break;
		if (oldseed==seed) {
			qDebug()<<"...discarded";
			break;
		}
		sl->setKout(ks+1);
		qDebug()<<"Kout changed to"<<ks+1;
		enforcePlane(sl,pdata,q,ks,seed,boundOffset);
		MatrixD data=sl->calcUs(pdata,ks,Uv);
		MatrixIndex n,f,t(data.size());
		bool fl=false;
		for2DIncr(n,f,t)
			if (data.at(n)>0.0) {
				n=t;
				fl=true;
			}
		if (!fl) {
			qDebug()<<"Empty data";
			break;
		}
		usini=inhibit(usini,data);
		oldseed=seed;
	}
	qDebug();
}


MatrixD SOrgTrainer::SResIni(Layer::Data uc) {
	MatrixD res(sl->S,0.0);
	DMatrixD Dres(res);
	MatrixIndex n,f,t(sl->S),bo(sl->boundariesOffset(uc.at(0))+sl->explicitBorderOffset);
	double r(rini);
	qSwap(r,sl->r);
	for2DIncr(n,f,t) {
		Dres[n]=sl->calcUsAt(n,uc,sl->aDefaultValues,bo);
	}
	qSwap(r,sl->r);
	return res;
}

MatrixD SOrgTrainer::inhibit(MatrixD ut,MatrixD uf) {
	DMatrixD Dut(ut);
	MatrixIndex boundOffset((uf.size()-sl->S-1)/2);
	MatrixIndex v,At=(cl.size()-1)/2,n,f,t(sl->S);
	for2DIncr(n,f,t) {
		MatrixIndex offset(n-At-1);
		double uu=ut.at(n);
		if (uu>0.0) {
			for2DIncr(v,f,cl.size()) {
				uu-=cl.at(v)*uf.at(v+offset,0.0);
			}
			Dut[n]=uu;
		}
	}
	return ut;
}

void SOrgTrainer::enforcePlane(SLayer*sl,Layer::Data pdata,double q,int kseed,MatrixIndex nseed,MatrixIndex bo) {
	MatrixIndex At=sl->Ar();
	MatrixIndex v,f,t(sl->A),n;
	int Kc=pdata.size();
	Q_ASSERT(kseed<sl->Kout());
	SLayer::Plane&plane=sl->planes[kseed];
	Q_ASSERT(pdata.size()==plane.a.size());
	MatrixD uv=sl->calcUv(pdata);

	MatrixIndex offset(nseed-At-bo);
	for(int kc=0;kc<Kc;kc++) {
		MatrixD&as=plane.a[kc];
		DMatrixD Das(as);
		MatrixD data=pdata.at(kc);
		for2DIncr(v,f,t) {
			Das[v]+=q*sl->c(v)*data.at(v+offset,0.0);
		}
	}
	f=MatrixIndex(0);
	double b=0.0;
	for(int kc=0;kc<Kc;kc++) {
		for2DIncr(n,f,sl->A) {
			double aa=plane.a.at(kc).at(n);
			double cc=sl->c(n);
			if (aa>0.0&&cc>0.0)
				b+=aa*aa/cc;
		}
	}
	plane.b=sqrt(b);
}
