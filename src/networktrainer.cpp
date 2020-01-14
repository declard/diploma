#include "networktrainer.h"

#include "sorgtrainer.h"
#include "lastlayersupertrainer.h"
#include "supertrainer.h"
#include "emptytrainer.h"
#include <QSharedPointer>
#include <QFile>

QHash<QString, QVector<BasicLayer::Data> > makeData(Neocognitron::Patterns patterns,bool learn) {
	typedef BasicLayer::Data Data;
	QHash<QString, QVector<Data> > datas;
	foreach(Neocognitron::Pattern p,patterns)
		if ((learn && p.train) || (!learn && p.test)) {
			Data data;
			data+=toMatrixD(p.data);
			datas[p.klass]+=data;
		}
	return datas;
}

void makeEdge(MatrixD&u,double xs,double ys,double xe,double ye) {
	int		i, j, ia, ii, N=u.size().x();
	double	center, xpol, x1, y1, x2, y2, p, x, y, xa, ya, w;

	center = ((double) N) / 2.0;
	if (fabs(xe - xs) < 0.0001)  return;
	if (xs < xe) {
		xpol = 1.0;
		x1 = center + xs;
		y1 = center - ys;
		x2 = center + xe;
		y2 = center - ye;
	} else {
		xpol = -1.0;
		x1 = center + xe;
		y1 = center - ye;
		x2 = center + xs;
		y2 = center - ys;
	}
	p = (y2 - y1) / (x2 - x1);
	if (x1 < 0.0) {
		y1 = y1 - p * x1;
		x1 = 0.0;
	}
	if (x2 > N) {
		y2 = y2 - p * (x2 - N);
		x2 = N;
	}
	if (fabs(p) < 0.0001)  p = 0.0;
	x = x1;
	y = y1;
	while (x < x2) {
		j = (int) x;
		i = (int) floor(y);
		if ((i == y) && (p < 0.0))  i = i - 1;
		xa = j + 1;
		if (xa > x2)  xa = x2;
		ya = y + p * (xa - x);
		ia = (int) floor(ya);
		if (ia > i) {
			ya = i + 1;
			xa = x + (ya - y) / p;
		} else if (ia < i) {
			ya = i;
			xa = x + (ya - y) / p;
		}
		w = xpol * (xa - x);
		if (i >= 0) {
			if (i < N) {
				u[MatrixIndex(i,j)] += w * ((y + ya) / 2.0 - i);
				for (ii = 0; ii < i; ii++) {
					u[MatrixIndex(ii,j)] += w;
				}
			} else {
				for (ii = 0; ii < N; ii++) {
					u[MatrixIndex(ii,j)] += w;
				}
			}
		}
		x = xa;
		y = ya;
	}
}

QVector<BasicLayer::Data> angle(int N,int K,double ang,double x0,double y0) {
	double pi = 3.141592653589793;
	double radang2 = ang * pi / 360.0;
	double radstep = pi * 0.0625;
	double R = ((double) N) * 0.43;
	double radcstep = (2.0 * pi / K);
	double radc = 0;
	QVector<BasicLayer::Data> us(K);
	MatrixIndex n,f,t(N);
	for(int k=0;k<K;k++) {
		radc += radcstep;
		double rada = radc + radang2;
		double radb = radc - radang2;
		MatrixD u(MatrixIndex(N),0.0);
		double Rcosa = R * cos(rada);
		double Rsina = R * sin(rada);
		double Rcosb = R * cos(radb);
		double Rsinb = R * sin(radb);
		makeEdge(u, Rcosa + x0, Rsina + y0, x0, y0);
		makeEdge(u, x0, y0, Rcosb + x0, Rsinb + y0);
		double x1 = Rcosb + x0;
		double y1 = Rsinb + y0;
		for (double rad = radb; rad < rada; rad += radstep) {
			double x2 = R * cos(rad) + x0;
			double y2 = R * sin(rad) + y0;
			makeEdge(u, x1, y1, x2, y2);
			x1 = x2;
			y1 = y2;
		}
		makeEdge(u, x1, y1, Rcosa + x0, Rsina + y0);

		for2DIncr(n,f,t) {
			if (u[n] < 0.0001)  u[n] = 0.0;
			else if (u[n] > 0.9999)  u[n] = 1.0;
		}
		us[k]+=u;
	}
	return us;
}
#include <QDateTime>
void NetworkTrainer::train() {
	QVector<Layer*>layers=cognitron->layers;
	int layer=cognitron->trained,layersCount=p->learnToLayer;
	QString startedAt=QDateTime::currentDateTime().toString(Qt::ISODate);
	qDebug()<<"Training for"<<layersCount-layer<<"layers started at"<<startedAt;
	int nextKin=0;
	if (layer==0) {
		state("Training level 0");
		QVector<BasicLayer::Data> edges=angle(p->N0,p->at(0).K,180.0,0.5,-0.5);
		SLayer*sl=layers[0]->slayer;
		MatrixIndex boundOffset=BasicLayer::boundariesOffset(cognitron->glayer->Ng,sl->S)+sl->explicitBorderOffset;
		sl->setKin(2);
		sl->setKout(p->at(0).K);
		for(int k=0;k<edges.size();k++) {
			BasicLayer::Data edge=edges.at(k);
			edge=cognitron->glayer->activate(edge);
			SOrgTrainer::enforcePlane(sl,edge,p->at(0).qs,k,MatrixIndex(p->at(0).Ns/2),boundOffset);
		}
	}
	qDebug()<<"layer 0 trained";

	state("Preparing data");
	QHash<QString, QVector<BasicLayer::Data> > initData=makeData(cognitron->patterns,true);
	QVector< QVector< BasicLayer::Data > > datas;
	QVector<int> klasses;
	QVector<QString> klassNames;
	{
		QMap<QString,int>datamap;
		int datamapl=0;
		QHashIterator< QString,QVector<BasicLayer::Data> > ii(initData);
		while(ii.hasNext()) {
			ii.next();
			datas+=ii.value();
			if (!datamap.contains(ii.key())) {
				datamap[ii.key()]=datamapl++;
			}
			klasses+=datamap[ii.key()];
			klassNames+=ii.key();
		}
	}

	double total=0.0,now=0.0;
	for(int ii=0;ii<datas.size();ii++)
		total+=datas.at(ii).size();
	qDebug()<<total<<100./total;
	for(int ii=0;ii<datas.size();ii++) {
		QVector< BasicLayer::Data >&data=datas[ii];
		for(int iii=0;iii<data.size();iii++) {
			progress(100.0/total*now);
			now+=1.0;
			data[iii]=cognitron->activate(data.at(iii),layer);
			qDebug()<<"activated on pattern"<<MatrixIndex(ii,iii).toString();
		}
	}
	qDebug()<<"init data is ready";
	nextKin=datas.at(0).at(0).size();
	for(;layer<layersCount;layer++) {
		state(QString("Training layer %1 (total %2)").arg(layer).arg(layersCount));
		qDebug()<<"starting layer"<<layer<<"training";
		Layer*lr=layers[layer];
		SLayer*sl=lr->slayer;
		sl->setKin(nextKin);
		switch(p->at(layer).trainType) {
			case ParametersContainer::unsupervised:
			case ParametersContainer::lastUnsupervised: {
				QVector< BasicLayer::Data > newDatas;
				QVector<int> newKlasses;
				for(int ii=0;ii<datas.size();ii++) {
					QVector<BasicLayer::Data> d=datas.at(ii);
					newDatas+=d;
					newKlasses+=QVector<int>(d.size(),klasses.at(ii));
				}

				sl->setKin(newDatas.at(0).size());
				QSharedPointer<SOrgTrainer> tr;
				if (p->at(layer).trainType==ParametersContainer::unsupervised) {
					tr=QSharedPointer<SOrgTrainer>(
						new SOrgTrainer(newDatas));
					connect(tr.data(),SIGNAL(progress(int)),this,SIGNAL(progress(int)));
				}
				else tr=QSharedPointer<SOrgTrainer>(
					new LastLayerSuperTrainer(newDatas,newKlasses));

				tr->r=p->at(layer).thetal/(1-p->at(layer).thetal);
				tr->co=p->at(layer).coSini;
				int d=((int)(p->at(layer).D+0.5))*2;
				tr->D=MatrixIndex(d);
				tr->asgamma=p->at(layer).gamSini;
				tr->rini=p->at(layer).thrini/(1-p->at(layer).thrini);
				tr->KMax=p->at(layer).KMax;
				tr->times=p->at(layer).repeat;
				tr->q=p->at(layer).qs;
				tr->cl=SLayer::makeC(((int)p->at(layer).Al)*2+1,p->at(layer).Al,p->at(layer).gaml,p->at(layer).col);

				tr->train(sl);
				if (p->at(layer).trainType==ParametersContainer::lastUnsupervised) {
					QVector<int>klassMap=dynamic_cast<LastLayerSuperTrainer*>(tr.data())->klassMap();
					cognitron->klassMap.clear();
					cognitron->klassMap.reserve(klassMap.size());
					foreach(int n,klassMap)
						cognitron->klassMap+=klassNames[n];
				}
				break;
			}
			case ParametersContainer::supervised: {
				SuperTrainer tr(datas,p->at(layer).qs);
				tr.train(sl);
				break;
			}
			default:;
		}
		qDebug()<<"layer"<<layer<<"training finished";
		nextKin=sl->Kout();

		if (layer!=layersCount-1) {
			state(QString("Propagating data from layer %1 (total %2)").arg(layer).arg(layersCount-1));
			double total=0,now=0;
			for(int ii=0;ii<datas.size();ii++)
				total+=datas.at(ii).size();
			for(int ii=0;ii<datas.size();ii++) {
				QVector< BasicLayer::Data >&data=datas[ii];
				for(int iii=0;iii<data.size();iii++) {
					progress(100.0/total*now);
					now+=1.0;
					BasicLayer::Data ddd=lr->activate(data.at(iii));
					qDebug()<<"activated on pattern"<<MatrixIndex(ii,iii).toString();
					data[iii]=ddd;
				}
			}
			qDebug()<<"data for the next layer is ready";
		}
		cognitron->trained++;
	}
	qDebug()<<"Training for"<<layers.size()<<"layers finished at"<<QDateTime::currentDateTime().toString(Qt::ISODate)<<"(started at"<<startedAt<<")";
}
