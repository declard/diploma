#include "basictrainer.h"

MatrixIndex BasicTrainer::findSeedIni(MatrixD usini) const {
	double umax=0.0;
	MatrixIndex seed(-1),n,f,t(usini.size());
	for2DIncr(n,f,t) {
		double us=usini.at(n);
		if (us>umax) {
			umax=us;
			seed=n;
		}
	}
	qDebug()<<"seed"<<seed.toString();
	return seed;
}

QVector< QPair<int,MatrixIndex> > BasicTrainer::findSeeds(BasicLayer::Data us,MatrixIndex D) const {
	QVector< QPair<int,MatrixIndex> > res;
	QHash<int,MatrixIndex> map;
	Matrix<int>kmax(sl->S,-1);
	MatrixD wmax(sl->S,0.0);
	DMatrixD Dwmax(wmax);
	MatrixIndex n,v,f,t(sl->S);
	for2DIncr(n,f,t)
		for(int ks=0;ks<us.size();ks++) {
			double wm=us.at(ks).at(n);
			if (wm>wmax.at(n)) {
				kmax[n]=ks;
				Dwmax[n]=wm;
			}
		}
	MatrixIndex offset((D-1)/2);
	for2DIncr(n,f,t) {
		double wm=wmax.at(n);
		if (wm>0.0) {
			for2DIncr(v,f,D) {
				MatrixIndex i(n-v-offset);
				if (!wmax.contains(i)) continue;
				if (wmax.at(i)>wm) {
					kmax[i]=-1;
					v=D;
				}
				else if (wmax.at(i)==wm)
					Dwmax[i]=wm*0.999;
			}

		}
	}
	for2DIncr(n,f,t) {
		int ks=kmax.at(n);
		if (ks>0 && (!map.contains(ks) || wmax.at(n)>wmax.at(map[ks])))
				map[ks]=n;
	}
	foreach(int ks,map.keys()) {
		res+=QPair<int,MatrixIndex>(ks,map[ks]);
		qDebug()<<"seed"<<ks<<map[ks].toString();
	}
	return res;
}
