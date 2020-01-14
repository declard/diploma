#ifndef PARAMETERSCONTAINER_H
#define PARAMETERSCONTAINER_H

#include <QAbstractTableModel>
#include <QAbstractSpinBox>
#include <QHash>
#include <QItemDelegate>
#include <QVector>

class ParametersContainer : public QAbstractTableModel {
	Q_OBJECT
	//enum { scalarVariablesCount=11 };
	//QHash<QString,QAbstractSpinBox*(*)(QWidget*)>vectorsSBFactory;
public:
	enum	{	unsupervised,	supervised,	lastUnsupervised,	none};

	int threads;
	int learnFromLayer;
	int learnToLayer;

	bool useGLayer;

	int N0;
	int Ng;
	double Ag;
	double Ago;
	double gamcg;

	struct LayerParameters {
		int Nc;
		int Ns;
		int mc;
		int KMax;
		int K;
		int repeat;
		int trainType;

		double As;
		double Aco;
		double Ac;
		double D;
		double Asini;
		double Al;

		double gamS;
		double coS;
		double gamSini;
		double coSini;
		double gamC;
		double gamCo;
		double gamCp;
		double coC;
		double pnC;
		double gaml;
		double col;

		double thetal;
		double thetar;

		double qs;
		double thrini;
	};
	QVector<LayerParameters>parameters;


	ParametersContainer(QObject*parent=0);
	Qt::ItemFlags flags(const QModelIndex&index )const;
	QVariant data(const QModelIndex&index,int role=Qt::DisplayRole) const;
	QVariant headerData(int section,Qt::Orientation orientation,int role=Qt::DisplayRole) const;
	int rowCount(const QModelIndex&parent=QModelIndex()) const;
	int columnCount(const QModelIndex&parent=QModelIndex()) const;
	bool setData(const QModelIndex&index,const QVariant&value,int role=Qt::EditRole);

	QWidget*getEditor(QWidget*, const QModelIndex &) const;

	LayerParameters&operator[](int i) { return parameters[i]; }
	const LayerParameters&at(int i) const { return parameters.at(i); }

	void setReadOnly() { ro=true; }
	int layersCount() const { return parameters.size(); }

	QDataStream&save(QDataStream&s) const;
	void load(QDataStream&s);

	void reset() {
		QAbstractTableModel::reset();
		modelReset();
	}
protected:
	bool ro;
	int intVectorsCount,doubleVectorsCount;
	QVector<int LayerParameters::*>intVectorsRefs;
	QVector<double LayerParameters::*>doubleVectorsRefs;
	QStringList intVectorsNames,doubleVectorsNames,intVectorsHumanNames,doubleVectorsHumanNames;
public slots:
	void layersCountChanged(int);
signals:
	void modelReset();
};

class ParametersContainerDelegate : public QItemDelegate {
	Q_OBJECT
public:
	ParametersContainerDelegate(QObject*parent):QItemDelegate(parent) {}
	QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // PARAMETERSCONTAINER_H
