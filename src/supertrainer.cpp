#include "supertrainer.h"
#include "slayer.h"

SuperTrainer::SuperTrainer(SuperTrainer::Patterns data,double q) {
	patterns=data;
	this->q=q;
}

void SuperTrainer::train(BasicLayer*layer) {
	sl=dynamic_cast<SLayer*>(layer);
	int Ks=patterns.size();
	sl->setKout(Ks);
	MatrixIndex v,f;
	for(int ks=0;ks<Ks;ks++) {
		PlanePatterns pps=patterns.at(0);
		SLayer::Plane&plane=sl->planes[ks];
		for(int p=0;p<pps.size();p++) {
			BasicLayer::Data d=pps.at(p);
			Q_ASSERT(sl->Kin()==d.size());
			int Kc=d.size();
			for(int kc=0;kc<Kc;kc++) {
				MatrixD m=d.at(kc);
				Q_ASSERT(sl->A==m.size());
				for2DIncr(v,f,sl->A)
					plane.a[kc][v]+=q*sl->c(v)*m.at(v);
			}
			plane.b+=q*sl->calcUvAt(sl->Ar(),d,MatrixIndex());
		}
	}
}
