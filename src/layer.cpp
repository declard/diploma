#include "layer.h"
#include "slayer.h"
#include "clayer.h"

Layer::Data Layer::activate(Layer::Data data) const {
	data=slayer->activate(data);
	return clayer->activate(data);
}

int Layer::Kin() const { return slayer->Kin(); }
int Layer::Kout() const { return slayer->Kout(); }
