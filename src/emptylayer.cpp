#include "emptylayer.h"

EmptyLayer::EmptyLayer() {}
BasicLayer::Data EmptyLayer::activate(BasicLayer::Data data) const { return data; }
int EmptyLayer::Kin() const { return 1; }
int EmptyLayer::Kout() const { return 1; }
