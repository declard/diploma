#include "imageeditorwidget.h"
#include <QMouseEvent>

ImageEditorWidget::ImageEditorWidget(QWidget*parent,MatrixIndex s):ImageViewerWidget(parent,s) {
	brushSize=0;
}

void ImageEditorWidget::mouseMoveEvent(QMouseEvent*e) {
	if (!changeState(e->posF(),e->buttons()))
		QWidget::mouseMoveEvent(e);
	else e->accept();
}
void ImageEditorWidget::mousePressEvent(QMouseEvent*e) {
	if (!changeState(e->posF(),e->buttons()))
		QWidget::mouseMoveEvent(e);
	else e->accept();
}

void ImageEditorWidget::wheelEvent(QWheelEvent*e) {
	if (e->delta()>0) {
		brushSize++;
		brushChanged(brushSize);
	}
	else if (e->delta()<0) {
		if (brushSize) {
			brushSize--;
			brushChanged(brushSize);
		}
	}
	e->accept();
}

template<class T>
static T&boundRef(Matrix<T>&m,MatrixIndex i) {
	static T t;
	if (i<0||i>=m.size())
		return t;
	return m[i];
}
bool ImageEditorWidget::changeState(QPointF p,Qt::MouseButtons b) {
	MatrixIndex U0sz(matr.size());
	unsigned char l;
	if (b==Qt::LeftButton) l=255;
	else if (b==Qt::MidButton) l=127;
	else if (b==Qt::RightButton) l=0;
	else return false;
	double dx,dy;
	dx=(double)width()/(double)U0sz.x();
	dy=(double)height()/(double)U0sz.y();
	MatrixIndex i(p.x()/dx,p.y()/dy);
	if (i>=matr.size()) return false;
	update();
	MatrixIndex v,bs(brushSize),f(i-bs),t(i+bs+1);
	for2DIncr(v,f,t)
		boundRef(matr,v)=l;
	return true;
}

void ImageEditorWidget::fill(char c) {
	matr.fill(c);
	update();
}
