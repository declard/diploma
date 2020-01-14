#ifndef LASTCLAYER_H
#define LASTCLAYER_H

#include "clayer.h"

class LastCLayer : public CLayer {
#if defined(USE_PARALLEL)
	Data parActivate(Data data,int parOffset,int parStep) const;
#endif
public:

	MatrixD calcUc(const MatrixD&Us) const;

	Data activate(Data) const;
	int Kin() const { return 1; }
	int Kout() const { return 1; }
	MatrixIndex size() const { return MatrixIndex(1); }
};

#endif // LASTCLAYER_H
