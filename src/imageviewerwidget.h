#ifndef IMAGEVIEWERWIDGET_H
#define IMAGEVIEWERWIDGET_H
#include <QWidget>
#include "matrix.h"
class ImageViewerWidget : public QWidget {
	Q_OBJECT
protected:
	MatrixC matr;
public:
	ImageViewerWidget(QWidget*parent=0,MatrixIndex s=MatrixIndex(64,64));
	void setU0size(MatrixIndex s);
	MatrixIndex U0size() const { return matr.size(); }
	void setData(const MatrixC&m);
	MatrixC data() const { return matr; }
protected:
	virtual void paintEvent(QPaintEvent*e);
};

MatrixD toMatrixD(MatrixC mc);
MatrixC toMatrixC(MatrixD md);
MatrixC toMatrixC2(MatrixD md);
#endif // IMAGEVIEWERWIDGET_H
