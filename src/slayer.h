#ifndef SLAYER_H
#define SLAYER_H

#include "layer.h"

class SLayer : public BasicLayer {
protected:
public:
	MatrixIndex S;		// ������ S- � V-����������
	MatrixIndex A;		// ����������� ������� S-������
	double r;			// �������������
	int m_Kin;
	MatrixD cs;

	static MatrixD makeC(int Na,double A,double gamma,double co);

	double c(MatrixIndex) const;	// ����� �� C � V
	MatrixIndex Ar() const { return (A-1)/2; }

	struct Plane {
		Plane(Data _a,double _b):a(_a),b(_b){}
		Plane() {}
		Data a;
		double b;
	};				// ����� C-S � V-S
	QVector<Plane>planes;

	MatrixD aDefaultValue;
	Data aDefaultValues;
	double bDefaultValue;
	void setADefaultValue(MatrixD m);

	double calcUvAt(MatrixIndex n,Data UcS,MatrixIndex bo) const;
	double calcUsAt(MatrixIndex n,Data UcS,Data planea,MatrixIndex bo) const;
	MatrixD calcUv(Data UcS) const;
	MatrixD calcUs(Data UcS,int k,MatrixD UvS) const;

public:
	int Kin() const { return m_Kin; }
	void setKin(int k);
	int Kout() const { return planes.size(); }
	void setKout(int k);
	MatrixIndex size() const { return S; }

	typedef BasicLayer::Data Data;

	SLayer(MatrixIndex ReceptiveField,MatrixIndex PlaneSize,double selectivity,MatrixD aini);

	virtual Data activate(Data data) const;
#if defined(USE_PARALLEL)
private:
	Data parActivate(Data data,MatrixD Uv,int parOffset,int parStep) const;
#endif
};


#endif // SLAYER_H
