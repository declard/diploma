#ifndef BASICTRAINER_H
#define BASICTRAINER_H

#include "slayer.h"
#include <QObject>

class BasicTrainer : public QObject {
	Q_OBJECT
protected:
	SLayer*sl;
public:
	virtual void train(BasicLayer*layer)=0;
	MatrixIndex findSeedIni(MatrixD usini) const;
	QVector< QPair<int,MatrixIndex> > findSeeds(BasicLayer::Data us,MatrixIndex D) const;
	virtual ~BasicTrainer() {}
};

#endif // BASICTRAINER_H
