#ifndef NEOCOGNITRONLAYER_H
#define NEOCOGNITRONLAYER_H

#include "basiclayer.h"


class SLayer;
class CLayer;
class Layer : public BasicLayer {
public:
	SLayer*slayer;
	CLayer*clayer;
	friend class SOrgTrainer;
public:

	typedef BasicLayer::Data Data;

	Data activate(Data) const;
	int Kin() const;
	int Kout() const;

	Layer(SLayer*sl,CLayer*cl):slayer(sl),clayer(cl) {}
	MatrixIndex size() const { return 0; }
};
inline static double sqr(double x) { return x*x; }

#endif // NEOCOGNITRONLAYER_H
