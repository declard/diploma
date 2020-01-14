#ifndef EMPTYTRAINER_H
#define EMPTYTRAINER_H

#include "basictrainer.h"

class EmptyTrainer : public BasicTrainer {
public:
	EmptyTrainer();
	void train(BasicLayer*) {}
};

#endif // EMPTYTRAINER_H
