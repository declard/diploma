#include "glayer.h"

double GLayer::a(MatrixIndex p) {
#if GLAYER_AG==3
	static double values[3][3]={
		{.0,	-.25,	.0},
		{-.25,	1,		-.25},
		{.0,	-.25,	.0}
	};
#else
#if GLAYER_AG==5
	static double values[5][5]={
		{ 1.312, -2.653, -3.58325, -2.653, 1.312 },
		{ -2.653, -1.401, 4.599, -1.401, -2.653 },
		{ -3.58325, 4.599, 17.517, 4.599, -3.58325 },
		{ -2.653, -1.401, 4.599, -1.401, -2.653 },
		{ 1.312, -2.653, -3.58325, -2.653, 1.312 }
	};
#else
#error
#endif
#endif
	return values[p.y()][p.x()]*2
#if GLAYER_AG==5
		/36
#endif
		;
}

MatrixD GLayer::makeCg(int Ng,double Ag,double Ago,double gamcg) {
	int		i, j;
	double	x, y, y2, r2, Spos, Sneg, SSpos, SSneg, cc;
	double	gn = (Ng - 1) * 0.5;
	double	Ag2 = Ag * Ag;
	double	Ago2 = Ago * Ago;
	double	C = 1 / Ag2;
	double	Co = 1 / Ago2;
	MatrixIndex cgs(Ng);
	MatrixD cg(cgs);

	Spos = 0.0;
	Sneg = 0.0;
	for (i = 0; i < Ng; i++) {
		y = i - gn;
		y2 = y * y;
		for (j = 0; j < Ng; j++) {
			x = j - gn;
			r2 = y2 + x * x;
			if (r2 <= Ag2) {
				cc = Co * pow(gamcg, r2 / Ago2) - C * pow(gamcg, r2 / Ag2);
				cg[MatrixIndex(i,j)] = cc;
				if (cc > 0.0)  Spos += cc;
				else           Sneg -= cc;
			} else {
				cg[MatrixIndex(i,j)] = 0.0;
			}
		}
	}
	SSpos = 1.0 / Spos;
	SSneg = 1.0 / Sneg;
	for (i = 0; i < Ng; i++) {
		for (j = 0; j < Ng; j++) {
			if (cg[MatrixIndex(i,j)] > 0.0) {
				cg[MatrixIndex(i,j)] *= SSpos;
			} else {
				cg[MatrixIndex(i,j)] *= SSneg;
			}
		}
	}
	return cg;
}

BasicLayer::Data GLayer::activate(BasicLayer::Data data) const {
	const MatrixD&plane=data[0];
	MatrixIndex boundOffset(boundariesOffset(plane));
	MatrixIndex v,Gt=Gr();
	//qDebug()<<(MatrixIndex(0)-Gr()-boundOffset).toString()<<(Ag+size()-Gr()-boundOffset).toString()<<size().toString()<<plane.size().toString()<<Ag.toString();
	Data res(2);
	res[0].resize(Ng);
	res[1].resize(Ng);
	MatrixD&res0(res[0]),&res1(res[1]);
	DMatrixD Dres0(res0),Dres1(res1);
	MatrixIndex n,f;
	for2DIncr(n,f,Ng) {
		MatrixIndex offset(n-Gt-boundOffset);
		double sum;
		sum=0;
		for2DIncr(v,f,Ag) {
			sum+=cg.at(v)*plane.at(v+offset,0.0);
		}
		Dres0[n]=qMax(sum,0.);
		Dres1[n]=qMax(-sum,0.);
	}
	return res;
}
