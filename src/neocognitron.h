#ifndef CNEOCOGNITRON_H
#define CNEOCOGNITRON_H
#include "layer.h"
#include "glayer.h"
#include "slayer.h"
#include "middleclayer.h"
#include "lastclayer.h"
#include <QMap>
#include <QDataStream>
#include "parameterscontainer.h"

class Neocognitron {
	void init();
	void loadMNISTPatterns(QString dir);
public:
	ParametersContainer*p;

	GLayer*glayer;
	QVector<Layer*>layers;
	int trained;

	struct Pattern {
		QString name;
		MatrixC data;
		QString klass;
		bool train,test;
		Pattern(){ train=test=false; }
		Pattern(QString s,Matrix<char>d,QString c,bool tr,bool ts):
				name(s),data(d),klass(c),train(tr),test(ts) {}
	};

	typedef QVector<Pattern> Patterns;
	Patterns patterns;
	QVector<QString>klassMap;

	Neocognitron(ParametersContainer*p);
	Layer::Data activate(Layer::Data data) { return activate(data,layers.size()); }
	Layer::Data activate(Layer::Data data,int count);
	void addNew(QString name,int size,QString cat);
	void addLayer(int i);

	static QImage toImage(MatrixC);
	static MatrixC fromImage(const QImage&);

	QDataStream&save(QDataStream&s) const;
	void load(QDataStream&s);

	QPair< QString,QVector<double> > getClass(Data data) const;
};

#endif // CNEOCOGNITRON_H
