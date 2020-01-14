#ifndef SUPERTRAINER_H
#define SUPERTRAINER_H
#include "basiclayer.h"
#include "basictrainer.h"

class SLayer;
class SuperTrainer : public BasicTrainer {
public:
	typedef QVector<BasicLayer::Data> PlanePatterns;
	typedef QVector<PlanePatterns> Patterns;
private:
	Patterns patterns;
	double q;
	SLayer*sl;
public:
	SuperTrainer(Patterns data,double q);
	void train(BasicLayer*layer);
};

#endif // SUPERTRAINER_H
