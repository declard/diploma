#ifndef LASTLAYERSORGTRAINER_H
#define LASTLAYERSORGTRAINER_H

#include "sorgtrainer.h"

class LastLayerSuperTrainer : public SOrgTrainer {
	QVector<int>categories,catMap;
	void sorg(Data pdata,int cat);
public:
	LastLayerSuperTrainer(QVector<Data> prevData,QVector<int> cats):
			SOrgTrainer(prevData),categories(cats) { Q_ASSERT(prevData.size()==categories.size()); }
	void train(BasicLayer*slayer);
	QVector<int> klassMap() const { return catMap; }
	virtual ~LastLayerSuperTrainer() {}
};

#endif // LASTLAYERSORGTRAINER_H
