#ifndef EMPTYLAYER_H
#define EMPTYLAYER_H

#include "basiclayer.h"

class EmptyLayer : public BasicLayer {
public:
	EmptyLayer();
	Data activate(Data) const;
	int Kin() const;
	int Kout() const;
};

#endif // EMPTYLAYER_H
