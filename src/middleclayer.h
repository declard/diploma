#ifndef MIDDLECLAYER_H
#define MIDDLECLAYER_H

#include "clayer.h"
class MiddleCLayer : public CLayer {
protected:
public:
	MatrixIndex C;		// Размер C-плоскостей
	MatrixIndex D;		// Рецептивная область C-клеток
	MatrixIndex factor;// Коэффициент сжатия

	MatrixD ds;
	double d(MatrixIndex) const;	// Связи от S к C
	MatrixIndex Dr() const { return (D-1)/2; }

	void initDs(
		int		Na,
		double	A,
		double	Ao,
		double	gamma,
		double	gammao,
		double	gammap,
		double	co,
		double	pn);
	MatrixD calcUc(const MatrixD&Us) const;

private:
#if defined(USE_PARALLEL)
	Data parActivate(Data data,int parOffset,int parStep) const;
#endif
public:

	MiddleCLayer(MatrixIndex ReceptiveField,MatrixIndex PlaneSize,MatrixIndex CompressFactor);

	Data activate(Data) const;
	int Kin() const { return 1; }
	int Kout() const { return 1; }
	MatrixIndex size() const { return C; }
};
#endif // MIDDLECLAYER_H
