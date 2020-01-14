#include "matrix.h"

QDataStream&operator<<(QDataStream&stream,MatrixIndex idx) {
	stream<<idx.x();
	stream<<idx.y();
	return stream;
}

QDataStream&operator>>(QDataStream&stream,MatrixIndex&idx) {
	stream>>idx.rx();
	stream>>idx.ry();
	return stream;
}

template<>
Matrix<double>&Matrix<double>::fill(const double&t) {
	if (t==0.0) {
		trimOrDetach();
		memset(slicep,0,sizeof(double)/sizeof(int)*slicer.size().product());
		return *this;
	}
	return fill(Rect(MatrixIndex(),size()),t);
}

MatrixDataBase MatrixDataBase::SharedNull=MatrixDataBase(MatrixIndex(0));
QExplicitlySharedDataPointer<MatrixDataBase> MatrixDataBase::SharedNullPointer=
		QExplicitlySharedDataPointer<MatrixDataBase>(&MatrixDataBase::SharedNull);
