#include "lastlayersupertrainer.h"

void LastLayerSuperTrainer::train(BasicLayer*slayer) {
	sl=dynamic_cast<SLayer*>(slayer);
	double r(r);
	qSwap(r,sl->r);
	for(int t=0;t<times;t++) {
		for(int p=0;p<previousData.size();p++) {
			sorg(previousData.at(p),categories.at(p));
		}
	}
	qSwap(r,sl->r);

qDebug()<<catMap;

}

void LastLayerSuperTrainer::sorg(Data pdata,int cat) {
	MatrixIndex boundOffset=sl->boundariesOffset(pdata.at(0).size())+sl->explicitBorderOffset;
	int kseed=-1;
	MatrixIndex wseed(-1);
	if (sl->Kout()>0) {
		MatrixIndex n,f,t(sl->S);
		double wmax=0.0;
		Data us=sl->activate(pdata);
		for(int ks=0;ks<us.size();ks++) {
			MatrixD md(us.at(ks));
			for2DIncr(n,f,t) {
				double w=md.at(n);
				if (w>wmax) {
					wseed=n;
					kseed=ks;
					wmax=w;
				}
			}
		}
		if (kseed!=-1&&catMap.at(kseed)!=cat)
			kseed=-1;
	}
	if (kseed==-1) {
		int ks=sl->Kout();
		MatrixD usini=SResIni(pdata);
		qDebug()<<usini.toString();
		wseed=findSeedIni(usini);
		if (wseed==-1) return;
		catMap+=cat;
		sl->setKout(ks+1);
		kseed=ks;
	}
	enforcePlane(sl,pdata,q,kseed,wseed,boundOffset);
}
