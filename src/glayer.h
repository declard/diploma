#ifndef GLAYER_H
#define GLAYER_H

#include "basiclayer.h"
#include "matrix.h"

#define GLAYER_AG 3

class GLayer : public BasicLayer {
protected:
	static inline double a(MatrixIndex);
public:
	MatrixD cg;
	MatrixIndex Ng;
	MatrixIndex Ag;
	static MatrixD makeCg(int Ng,double Ag,double Ago,double gamcg);
	GLayer() {}
	Data activate(Data data) const;
	int Kin() const { return 1; }
	int Kout() const { return 2; }
	MatrixIndex Gr() const { return (Ag-1)/2; }
	MatrixIndex size() const { return Ng; }
};
#endif // GLAYER_H
