#include "parameterscontainer.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

#define COPY_VAR(name,type,hname) \
		for(int i=0;i<sizeof(:: name)/sizeof(type);i++) \
			parameters[i].name=:: name[i]; \
		type##VectorsRefs+=&LayerParameters::name; \
		type##VectorsCount++; \
		type##VectorsNames+=#name; \
		type##VectorsHumanNames+=hname;

#include "layersparams.h"
ParametersContainer::ParametersContainer(QObject*parent):
		QAbstractTableModel(parent),parameters(::layersCount) {
	intVectorsCount=doubleVectorsCount=0;
	this->N0=::N0;
	this->Ng=::Ng;

	this->Ag=::Ag;
	this->Ago=::Ago;
	this->gamcg=::gamcg;

	COPY_VAR(Ns,int,"Сторона S-плоскости");
	COPY_VAR(Nc,int,"Сторона C-плоскости");
	COPY_VAR(mc,int,"Шаг C-клеток");
	COPY_VAR(K,int,"Начальное кол-во плоскостей");
	COPY_VAR(KMax,int,"Максимальное кол-во плоскостей");
	COPY_VAR(repeat,int,"Итераций");
	//COPY_VAR(trainType,int,"Тип обучения");
	for(int i=0;i<sizeof(:: trainType)/sizeof(int);i++)
		parameters[i].trainType=:: trainType[i];

	COPY_VAR(As,double,"Радиус a-связей");
	COPY_VAR(gamS,double,"Затухание a-связей");
	COPY_VAR(coS,double,"Центральное значение a-связей");
	COPY_VAR(thetal,double,"Селективность");

	COPY_VAR(Asini,double,"Радиус a-связей\nинициирующих плоскостей");
	COPY_VAR(gamSini,double,"Затухание инициирующих\na-связей");
	COPY_VAR(coSini,double,"Центральное значение\nинициирующих a-связей");
	COPY_VAR(thrini,double,"Селективность\nинициирующих a-связей");

	COPY_VAR(Aco,double,"Радиус положительных d-связей");
	COPY_VAR(Ac,double,"Полный радиус d-связей");
	COPY_VAR(gamC,double,"Затухание положительных d-связей в Ao");
	COPY_VAR(gamCo,double,"Затухание отрицательных d-связей в Ao");
	COPY_VAR(gamCp,double,"Затухание отрицательных d-связей в A");
	COPY_VAR(coC,double,"Центральное значение d-связей");
	COPY_VAR(pnC,double,"Отношение отрицательной\nкомпоненты к положительной");

	COPY_VAR(Al,double,"Радиус области\nторможения при обучении");
	COPY_VAR(gaml,double,"Затухание области\nторможения при обучении");
	COPY_VAR(col,double,"Центральное значение области\nторможения при обучении");

	COPY_VAR(thetar,double,"Селективность при обучении");
	COPY_VAR(D,double,"Радиус конкуренции");
	COPY_VAR(qs,double,"Скорость обучения");

	this->learnFromLayer=::learnFromLayer;
	this->learnToLayer=::learnToLayer;
	threads=2;

	useGLayer=true;
	ro=false;
}

int ParametersContainer::rowCount(const QModelIndex&) const {
	return intVectorsCount+doubleVectorsCount;
}

int ParametersContainer::columnCount(const QModelIndex&) const {
	return parameters.size();
}

QVariant ParametersContainer::headerData(int section,Qt::Orientation orientation,int role) const {
	if (role!=Qt::DisplayRole) return QVariant();
	if (orientation==Qt::Horizontal) return QString("Слой %1").arg(section+1);
	if (section<intVectorsCount)
		return intVectorsHumanNames.at(section);
	section-=intVectorsCount;
	return doubleVectorsHumanNames.at(section);
}

Qt::ItemFlags ParametersContainer::flags(const QModelIndex&index) const {
	if (!index.isValid()) return Qt::ItemFlags();
	if (ro) return Qt::ItemIsEnabled;
	int col=index.row();
	if (col<intVectorsCount) {
		if (index.column()==layersCount()-1 &&
			intVectorsNames.at(col)=="Nc") return Qt::ItemIsEnabled;
	}
	return Qt::ItemIsEditable|Qt::ItemIsEnabled;
}

QVariant ParametersContainer::data(const QModelIndex&index,int role) const {
	if (!index.isValid() ||
		(role!=Qt::EditRole && role!=Qt::DisplayRole))
		return QVariant();
	int row=index.column(),col=index.row();
	if (col<intVectorsCount)
		return parameters.at(row).*intVectorsRefs.at(col);
	col-=intVectorsCount;
	return parameters.at(row).*doubleVectorsRefs.at(col);
}

bool ParametersContainer::setData(const QModelIndex&index,const QVariant&value,int role) {
	(void)role;
	if (!index.isValid()) return false;
	int row=index.column(),col=index.row();
	if (col<intVectorsCount) {
		parameters[row].*intVectorsRefs[col]=value.toInt();
		return true;
	}
	col-=intVectorsCount;
	parameters[row].*doubleVectorsRefs[col]=value.toDouble();
	return true;
}

QWidget*ParametersContainer::getEditor(QWidget*parent, const QModelIndex & i) const {
	if (!i.isValid()) return 0;
	int v=i.row();
	if (v<intVectorsCount) {
		QSpinBox*w=new QSpinBox(parent);
		w->setMaximum(5000);
		return w;
	}
	else {
		v-=intVectorsCount;
		QDoubleSpinBox*w=new QDoubleSpinBox(parent);
		w->setMaximum(5000.0);
		return w;
	}

}

void ParametersContainer::layersCountChanged(int c) {
	parameters.resize(c);
	reset();
}

QDataStream&ParametersContainer::save(QDataStream&s) const {
	int psz=parameters.size();
	s<<parameters.size()<<threads<<learnFromLayer<<learnToLayer<<useGLayer<<N0<<Ng<<Ag<<Ago<<gamcg;
	for(int i=0;i<psz;i++) {
		for(int ii=0;ii<intVectorsCount;ii++)
			s<<parameters[i].*intVectorsRefs[ii];
		for(int ii=0;ii<doubleVectorsCount;ii++)
			s<<parameters[i].*doubleVectorsRefs[ii];
	}
	return s;
}

void ParametersContainer::load(QDataStream&s) {
	int psz;
	s>>psz>>threads>>learnFromLayer>>learnToLayer>>useGLayer>>N0>>Ng>>Ag>>Ago>>gamcg;
	parameters.resize(psz);
	for(int i=0;i<psz;i++) {
		for(int ii=0;ii<intVectorsCount;ii++)
			s>>(parameters[i].*intVectorsRefs[ii]);
		for(int ii=0;ii<doubleVectorsCount;ii++)
			s>>(parameters[i].*doubleVectorsRefs[ii]);
	}
}

QWidget * ParametersContainerDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
	const ParametersContainer*model=qobject_cast<const ParametersContainer*>(index.model());
	QWidget*widget=model->getEditor(parent,index);
	if (!widget) return QItemDelegate::createEditor(parent,option,index);
	return widget;
}
