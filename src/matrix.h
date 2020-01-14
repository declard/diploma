#ifndef MATRIX_H
#define MATRIX_H

#define DONT_CHECK_MATRIX_BOUNDARIES

#include <QSharedData>
#include <QExplicitlySharedDataPointer>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QString>
#include <QStringList>
#include <math.h>

// 2-мерный цикл с шагом S
#define for2DAdd(I,F,T,S) \
	for(I.rx()=F.rx();I.rx()<T.rx();I.rx()+=S.rx()) \
		for(I.ry()=F.ry();I.ry()<T.ry();I.ry()+=S.ry())

// 2-мерный цикл с шагом 1
#define for2DIncr(I,F,T) \
	for(I.rx()=F.rx();I.rx()<T.rx();I.rx()++) \
		for(I.ry()=F.ry();I.ry()<T.ry();I.ry()++)

// n-мерный индекс
template<int I>
class Index {
protected:
	int idx[I];
	static int a(int l,int r) { return l+r; }
	static int s(int l,int r) { return l-r; }
	static int m(int l,int r) { return l*r; }
	static int d(int l,int r) { return l/r; }
	static int mod(int l,int r) { return l%r; }
	static int n(int l) { return -l; }
	template<int(*F)(int,int)>
	static Index<I> map(const Index<I>l,const Index<I>r) {
		Index<I>res;
		for(int i=0;i<I;i++) res[i]=F(l[i],r[i]);
		return res;
	}
	template<int(*F)(int)>
	static Index<I> map(const Index<I>l) {
		Index<I>res;
		for(int i=0;i<I;i++) res[i]=F(l[i]);
		return res;
	}
public:
	Index() { memset(idx,0,I*sizeof(int)); }
	Index(int ix) {
		for(int i=0;i<I;i++)
			idx[i]=ix;
	}
	int&operator[](int i) { return idx[i]; }
	int operator[](int i) const { return idx[i]; }
	int size() const { return I; }
	Index<I> operator+(const Index<I>&i) const { return map<a>(*this,i); }
	Index<I> operator-(const Index<I>&i) const { return map<s>(*this,i); }
	Index<I> operator*(const Index<I>&i) const { return map<m>(*this,i); }
	Index<I> operator/(const Index<I>&i) const { return map<d>(*this,i); }
	Index<I> operator%(const Index<I>&i) const { return map<mod>(*this,i); }

	Index<I>&operator+=(const Index<I>&i) { return *this=map<a>(*this,i); }
	Index<I>&operator-=(const Index<I>&i) { return *this=map<s>(*this,i); }
	Index<I>&operator*=(const Index<I>&i) { return *this=map<m>(*this,i); }
	Index<I>&operator/=(const Index<I>&i) { return *this=map<d>(*this,i); }
	Index<I>&operator%=(const Index<I>&i) { return *this=map<mod>(*this,i); }

	Index<I>operator-() const { return map<n>(*this); }

	// Операторы для проверки допустимых диапазонов
	bool operator>(const Index<I>&ix) const {
		for(int i=0;i<I;i++)
			if (idx[i]>ix[i]) return true;
		return false;
	}
	bool operator<(const Index<I>&ix) const {
		for(int i=0;i<I;i++)
			if (idx[i]<ix[i]) return true;
		return false;
	}
	bool operator>=(const Index<I>&ix) const {
		for(int i=0;i<I;i++)
			if (idx[i]>=ix[i]) return true;
		return false;
	}
	bool operator<=(const Index<I>&ix) const {
		for(int i=0;i<I;i++)
			if (idx[i]<=ix[i]) return true;
		return false;
	}
	// Обычное равенство
	bool operator==(const Index<I>&ix) const {
		for(int i=0;i<I;i++)
			if (idx[i]!=ix[i]) return false;
		return true;
	}
	bool operator!=(const Index<I>&i) const { return !(*this==i); }
	// Произведение компонентов
	int product() const {
		int res=1;
		for(int i=0;i<I;i++)
			res*=idx[i];
		return res;
	}
	double length() const {
		double res=0.0;
		for(int i=0;i<I;i++)
			res+=sqr(idx[i]);
		return sqrt(res);
	}
	QString toString() const {
		QStringList list;
		for(int i=0;i<I;i++)
			list<<QString::number(idx[i]);
		return '('+list.join(",")+')';
	}
};

template<int I>
QDataStream&operator<<(QDataStream&stream,Index<I> idx) {
	for(int i=0;i<I;i++)
		stream<<idx[i];
	return stream;
}

template<int I>
QDataStream&operator>>(QDataStream&stream,Index<I>&idx) {
	for(int i=0;i<I;i++)
		stream>>idx[i];
	return stream;
}

// Матричный 2-мерный индекс. Введён для удобства
#if 0
class MatrixIndex : public Index<2> {
public:
	MatrixIndex(const QSize&s) {
		idx[0]=s.width();
		idx[1]=s.height();
	}
	MatrixIndex(const QPoint&s) {
		idx[0]=s.x();
		idx[1]=s.y();
	}
	MatrixIndex():Index<2>(0) {}
	MatrixIndex(const Index<2>&i):Index<2>(i) {}
	MatrixIndex(int x,int y) { idx[0]=x; idx[1]=y; }
	int x() const { return idx[0]; }
	int y() const { return idx[1]; }
	int&rx() { return idx[0]; }
	int&ry() { return idx[1]; }
	double length() const {
		double res=idx[0]*idx[0]+idx[1]*idx[1];
		return sqrt(res);
	}
	static MatrixIndex min(const MatrixIndex&l,const MatrixIndex&r) {
		return MatrixIndex(qMin(l.x(),r.x()),qMin(l.y(),r.y()));
	}
	static MatrixIndex max(const MatrixIndex&l,const MatrixIndex&r) {
		return MatrixIndex(qMax(l.x(),r.x()),qMax(l.y(),r.y()));
	}
	operator QPoint() const { return QPoint(x(),y()); }
	operator QPointF() const { return QPointF(x(),y()); }
	operator QSize() const { return QSize(x(),y()); }
	operator QSizeF() const { return QSizeF(x(),y()); }
};
#else
class MatrixIndex {
	int m_x,m_y;
public:
	MatrixIndex(const QSize&s) {
		m_x=s.width();
		m_y=s.height();
	}
	MatrixIndex(const QPoint&s) {
		m_x=s.x();
		m_y=s.y();
	}
	MatrixIndex() { m_x=m_y=0; }
	MatrixIndex(int i) { m_x=m_y=i; }
	MatrixIndex(int x,int y) { m_x=x; m_y=y; }
	int x() const { return m_x; }
	int y() const { return m_y; }
	int&rx() { return m_x; }
	int&ry() { return m_y; }
	const int&rx() const { return m_x; }
	const int&ry() const { return m_y; }
	static MatrixIndex min(const MatrixIndex&l,const MatrixIndex&r) {
		return MatrixIndex(qMin(l.x(),r.x()),qMin(l.y(),r.y()));
	}
	static MatrixIndex max(const MatrixIndex&l,const MatrixIndex&r) {
		return MatrixIndex(qMax(l.x(),r.x()),qMax(l.y(),r.y()));
	}
	operator QPoint() const { return QPoint(x(),y()); }
	operator QPointF() const { return QPointF(x(),y()); }
	operator QSize() const { return QSize(x(),y()); }
	operator QSizeF() const { return QSizeF(x(),y()); }

	MatrixIndex operator+(const MatrixIndex&i) const { return MatrixIndex(m_x+i.m_x,m_y+i.m_y); }
	MatrixIndex operator-(const MatrixIndex&i) const { return MatrixIndex(m_x-i.m_x,m_y-i.m_y); }
	MatrixIndex operator*(const MatrixIndex&i) const { return MatrixIndex(m_x*i.m_x,m_y*i.m_y); }
	MatrixIndex operator/(const MatrixIndex&i) const { return MatrixIndex(m_x/i.m_x,m_y/i.m_y); }
	MatrixIndex operator%(const MatrixIndex&i) const { return MatrixIndex(m_x%i.m_x,m_y%i.m_y); }

	MatrixIndex&operator+=(const MatrixIndex&i) { m_x+=i.m_x; m_y+=i.m_y; return *this; }
	MatrixIndex&operator-=(const MatrixIndex&i) { m_x-=i.m_x; m_y-=i.m_y; return *this; }
	MatrixIndex&operator*=(const MatrixIndex&i) { m_x*=i.m_x; m_y*=i.m_y; return *this; }
	MatrixIndex&operator/=(const MatrixIndex&i) { m_x/=i.m_x; m_y/=i.m_y; return *this; }
	MatrixIndex&operator%=(const MatrixIndex&i) { m_x%=i.m_x; m_y%=i.m_y; return *this; }

	MatrixIndex operator-() const { return MatrixIndex(-m_x,-m_y); }

	MatrixIndex operator+(int i) const { return MatrixIndex(m_x+i,m_y+i); }
	MatrixIndex operator-(int i) const { return MatrixIndex(m_x-i,m_y-i); }
	MatrixIndex operator*(int i) const { return MatrixIndex(m_x*i,m_y*i); }
	MatrixIndex operator/(int i) const { return MatrixIndex(m_x/i,m_y/i); }
	MatrixIndex operator%(int i) const { return MatrixIndex(m_x%i,m_y%i); }

	MatrixIndex&operator+=(int i) { m_x+=i; m_y+=i; return *this; }
	MatrixIndex&operator-=(int i) { m_x-=i; m_y-=i; return *this; }
	MatrixIndex&operator*=(int i) { m_x*=i; m_y*=i; return *this; }
	MatrixIndex&operator/=(int i) { m_x/=i; m_y/=i; return *this; }
	MatrixIndex&operator%=(int i) { m_x%=i; m_y%=i; return *this; }

	// Операторы для проверки допустимых диапазонов
	bool operator>(const MatrixIndex&ix) const {
		return m_x>ix.m_x||m_y>ix.m_y;
	}
	bool operator<(const MatrixIndex&ix) const {
		return m_x<ix.m_x||m_y<ix.m_y;
	}
	bool operator>=(const MatrixIndex&ix) const {
		return m_x>=ix.m_x||m_y>=ix.m_y;
	}
	bool operator<=(const MatrixIndex&ix) const {
		return m_x<=ix.m_x||m_y<=ix.m_y;
	}
	// Обычное равенство
	bool operator==(const MatrixIndex&ix) const {
		return m_x==ix.m_x&&m_y==ix.m_y;
	}
	bool operator!=(const MatrixIndex&i) const { return !(*this==i); }
	// Произведение компонентов
	int product() const {
		return m_x*m_y;
	}
	double length() const {
		return sqrt(double(m_x*m_x+m_y*m_y));
	}
	QString toString() const {
		return QString("(%1,%2)").arg(m_x).arg(m_y);
	}
};

QDataStream&operator<<(QDataStream&stream,MatrixIndex idx);
QDataStream&operator>>(QDataStream&stream,MatrixIndex&idx);
#endif

// Аналог QRect для индексов матрицы
class Rect {
	MatrixIndex tl,sz;
public:
	Rect():tl(0,0),sz(0,0) {}
	Rect(const Rect&r):tl(r.tl),sz(r.sz) {}
	Rect(MatrixIndex topleft,MatrixIndex s):
			tl(topleft),sz(s) {}
	Rect(int x,int y,int w, int h):tl(x,y),sz(w,h) {}
	const MatrixIndex&topLeft() const { return tl; }
	MatrixIndex bottomRight() const { return sz+tl; }
	int x() const { return tl.x(); }
	int y() const { return tl.y(); }
	int width() const { return sz.x(); }
	int height() const { return sz.y(); }
	int top() const { return tl.y(); }
	int bottom() const { return (sz+tl).y(); }
	int left() const { return tl.x(); }
	int right() const { return (sz+tl).x(); }
	const MatrixIndex&size() const { return sz; }
	QString toString() const {
		return '('+tl.toString()+','+sz.toString()+')';
	}
};

// Внутренняя структура матриц
struct MatrixDataBase : public QSharedData {
	static MatrixDataBase SharedNull;
	static QExplicitlySharedDataPointer<MatrixDataBase> SharedNullPointer;
	MatrixIndex s;
	MatrixDataBase():QSharedData(SharedNull){}
	MatrixDataBase(MatrixIndex sz):s(sz) {}
	MatrixDataBase(const MatrixDataBase&md):QSharedData(md),s(md.s) {}
};
template<typename T>
struct MatrixData : public MatrixDataBase {
	static T&simpleIndex(T*t,const MatrixIndex&size,const MatrixIndex&i) {
		return t[size.x()*i.y()+i.x()];
	}
	T*t;
	MatrixData():t(0) {}
	MatrixData(const MatrixData&md):MatrixDataBase(md) {
		t=new T[s.product()];
		MatrixIndex i,f;
		for2DIncr(i,f,s)
			simpleIndex(t,s,i)=simpleIndex(md.t,s,i);
	}
	MatrixData(MatrixIndex sz):MatrixDataBase(sz) {
		t=new T[s.product()];
	}
	~MatrixData() {
		delete[]t;
	}

};

template<typename T>
class DetachedMatrix;

// Implicitly shared matrix.
template<typename T>
class Matrix {
	friend class DetachedMatrix<T>;
protected:
	typedef MatrixData<T> Data;
	typedef QExplicitlySharedDataPointer<Data> DataPointer;
	DataPointer d;
	T*slicep;
	Rect slicer;
	T&index(const MatrixIndex&i) {
		return Data::simpleIndex(slicep,d->s,i);
	}
	T&checkedIndex(const MatrixIndex&i) {
#if !defined(DONT_CHECK_MATRIX_BOUNDARIES)
		if (i<0||i>=slicer.size())
			qFatal("Wrong index %s in `T&checkedIndex(const MatrixIndex&)`",
					i.toString().toLocal8Bit().constData());
#endif
		return Data::simpleIndex(slicep,d->s,i);
	}
	const T&checkedIndex(const MatrixIndex&i) const {
#if !defined(DONT_CHECK_MATRIX_BOUNDARIES)
		if (i<0||i>=slicer.size())
			qFatal("Wrong index %s in `const T&checkedIndex(const MatrixIndex&) const`",
					i.toString().toLocal8Bit().constData());
#endif
		return Data::simpleIndex(const_cast<T*>(slicep),d->s,i);
	}
	void setSlice(const Rect&s) {
		if (s.topLeft()<slicer.topLeft()||
			s.bottomRight()>slicer.bottomRight())
			qFatal("Wrong slice %s on %s in `void setSlice(const Rect&)`",
				   s.toString().toLocal8Bit().constData(),
				   slicer.toString().toLocal8Bit().constData());
		slicer=s;
		slicep+=s.x()+s.y()*d->s.x();
	}
	Matrix<T>&resize(const MatrixIndex&sz,const T*filler,bool) {
		if (sz==slicer.size()) return *this;
		trimSlice();
		DataPointer nd=DataPointer(new Data(sz));
		MatrixIndex i,f,t(sz);
		if (filler)
			for2DIncr(i,f,t)
				Data::simpleIndex(nd->t,sz,i)=*filler;
		t=qMin(sz,slicer.size());
		for2DIncr(i,f,t)
			Data::simpleIndex(nd->t,sz,i)=index(i);
		d=nd;
		slicer=Rect(MatrixIndex(),sz);
		slicep=d->t;
		return *this;
	}

	/*static void fill(T*ptr,int offset,int size,const T*filler) {
		if (!filler) return;
		size+=offset;
		for(int i=offset;i<size;i++) ptr[i]=*filler;
	}*/
	void trimOrDetach() {
		if (!trimSlice()) detach();
	}
	Matrix(Matrix<T>*m):d(m->d),slicer(MatrixIndex(),d->s) {
		slicep=d->t;
	}
public:
	Matrix() {
		d=new MatrixData<T>;
		slicep=0;
	}
	Matrix(T*data,MatrixIndex size):slicer(MatrixIndex(),size) {
		d=new MatrixData<T>(size);
		slicep=d->t;
		memcpy(slicep,data,sizeof(T)*size.product());
	}
	Matrix(MatrixIndex size):slicer(MatrixIndex(),size) {
		d=new MatrixData<T>(size);
		slicep=d->t;
	}
	// Матрица заданного размера с заполнением
	Matrix(MatrixIndex size,const T&t):slicer(MatrixIndex(),size) {
		d=new MatrixData<T>(size);
		slicep=d->t;
		fill(t);
	}
	template<typename Tm>
	Matrix(const Matrix<Tm>&tm):slicer(MatrixIndex(),tm.size()) {
		d=new MatrixData<T>(tm.size());
		slicep=d->t;
		MatrixIndex i,f,t(tm.size());
		for2DIncr(i,f,t)
			(*this)[i]=tm.at(i);
	}
	Matrix(const Matrix&m):d(m.d),slicer(m.slicer),slicep(m.slicep) {}
	// Срез копии матрицы
	Matrix(const Matrix&m,const Rect&s):d(m.d),slicer(m.slicer),slicep(m.slicep) {
		setSlice(s);
	}
	Matrix<T>&operator=(const Matrix&m) {
		d=m.d;
		slicer=m.slicer;
		slicep=d->t;
		return *this;
	}
	// Срез матрицы
	Matrix<T>&slice(const Rect&s) {
		setSlice(s);
		return *this;
	}
	/*Matrix<T>unsliced() const {
		return Matrix<T>(this);
	}*/
	/* Обрезка не используемых срезом частей матрицы.
	   @ret был ли detach()
	*/
	bool trimSlice() {
		if (slicer.topLeft()==0&&slicer.size()==d->s) return false;
		DataPointer nd=DataPointer(new Data(slicer.size()));
		MatrixIndex i,f,t(d->s);
		for2DIncr(i,f,t)
				Data::simpleIndex(nd->t,nd->s,i)=index(i);
		d=nd;
		slicer=Rect(MatrixIndex(0,0),slicer.size());
		slicep=d->t;
		return true;
	}
	// Глубокое копирование
	Matrix<T>&detach() {
		d.detach();
		slicep=d->t;
		return *this;
	}
	const T&operator[](const MatrixIndex&i) const { return checkedIndex(i); }
	T&operator[](const MatrixIndex&i) { trimOrDetach(); return checkedIndex(i); }
	const T&at(const MatrixIndex&i) const { return checkedIndex(i); }
	const T&at(const MatrixIndex&i,const T&t) const {
		if (!contains(i)) return t;
		return at(i);
	}
	T value(const MatrixIndex&i) const { return checkedIndex(i); }

	bool contains(MatrixIndex i) const { return !(i<0 || i>=size()); }
	// Заполнение
	Matrix<T>&fill(const Rect&r,const T&t) {
		trimOrDetach();
		MatrixIndex i;
		for2DIncr(i,r.topLeft(),r.bottomRight())
			index(i)=t;
		return *this;
	}
	Matrix<T>&fill(const T&t) {
		return fill(Rect(MatrixIndex(),size()),t);
	}
	// Изменение геометрии
	Matrix<T>&resize(const MatrixIndex&size,const T&t) {
		return resize(size,&t,true);
	}
	Matrix<T>&resize(const MatrixIndex&size) {
		return resize(size,0,true);
	}
	// Транспонирование
	Matrix<T>&transpose() {
		trimOrDetach();
		for(int i=0;i<r;i++)
			for(int ii=0;ii<c;i++)
				qSwap(a[r*i+ii],a[c*i+ii]);
		qSwap(r,c);
		return *this;
	}

	const MatrixIndex&size() const { return slicer.size(); }
	QString toString() const {
		QStringList list;
		for(int i=0;i<slicer.size().y();i++) {
			QStringList values;
			for(int ii=0;ii<slicer.size().x();ii++)
				values<<QString("%1").arg(at(MatrixIndex(ii,i)),9,'g',7,'0');
			list<<values.join(",");
		}
		return "{"+list.join("},\n{")+"}";
	}
};

template<typename T>
class DetachedMatrix {
	Matrix<T>&matrix;
public:
	DetachedMatrix(Matrix<T>&m):matrix(m) { m.detach(); }
	inline const T&operator[](const MatrixIndex&i) const { return matrix.checkedIndex(i); }
	inline T&operator[](const MatrixIndex&i) { return matrix.checkedIndex(i); }
	inline const T&at(const MatrixIndex&i) const { return matrix.checkedIndex(i); }
	inline const T&at(const MatrixIndex&i,const T&t) const {
		if (!matrix.contains(i)) return t;
		return matrix.at(i);
	}
};

static QDataStream&operator<<(QDataStream&stream,char c) {
	stream<<(int)c;
	return stream;
}

template<class T>
QDataStream&operator<<(QDataStream&stream,Matrix<T> matrix) {
	//stream<<QString("Matrix");
	matrix.trimSlice();
	MatrixIndex n,f,t(matrix.size());
	stream<<t;
	for2DIncr(n,f,t)
		stream<<matrix.at(n);
	return stream;
}

static QDataStream&operator>>(QDataStream&stream,char&c) {
	int i;
	stream>>i;
	c=i;
	return stream;
}

template<class T>
QDataStream&operator>>(QDataStream&stream,Matrix<T>&matrix) {
	/*QString s;
	stream>>s;
	if (s!="Matrix") throw "Must be matrix";*/
	MatrixIndex n,f,t;
	stream>>t;
	matrix.resize(t);
	for2DIncr(n,f,t)
		stream>>matrix[n];
	return stream;
}

typedef Matrix<char> MatrixC;
typedef Matrix<int> MatrixI;
typedef Matrix<double> MatrixD;

typedef DetachedMatrix<char> DMatrixC;
typedef DetachedMatrix<int> DMatrixI;
typedef DetachedMatrix<double> DMatrixD;

#endif // MATRIX_H
