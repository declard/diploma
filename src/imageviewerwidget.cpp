#include "imageviewerwidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QPointF>
#include <QRectF>

QPointF operator*(const QPointF&l,const QPointF&r) {
	return QPointF(l.x()*r.x(),l.y()*r.y());
}

ImageViewerWidget::ImageViewerWidget(QWidget*parent,MatrixIndex s):QWidget(parent),matr(s,0) {
}

void ImageViewerWidget::setU0size(MatrixIndex s) {
	matr.resize(s,0);
	update();
}

void ImageViewerWidget::setData(const MatrixC&m) {
	matr=m;
	update();
}
void ImageViewerWidget::paintEvent(QPaintEvent*e) {
	MatrixIndex U0sz(matr.size());
	QPainter p(this);
	double dx,dy,x,y;
	int i;
	dx=(double)(width()-1)/(double)U0sz.x();
	dy=(double)(height()-1)/(double)U0sz.y();

	MatrixIndex v,f,t(U0sz);
	QPointF d(dx,dy);
	for2DIncr(v,f,t) {
		char ch=matr.at(v);
		//if (ch) {
			//int c=255-ch*2;
			int c=ch;
			QRgb rgb=qRgb(c,c,c);
			p.fillRect(QRectF(v*d,MatrixIndex(v+1)*d),/*Qt::black*/rgb);
		//}
	}
	p.setPen(Qt::lightGray);
	for(i=0,x=0;i<=U0sz.x();i++,x+=dx)
		p.drawLine(x,0,x,height());

	for(i=0,y=0;i<=U0sz.y();i++,y+=dy)
		p.drawLine(0,y,width(),y);
	e->accept();
}

MatrixD toMatrixD(MatrixC mc) {
	MatrixD md(mc.size());
	MatrixIndex i,f,to(mc.size());
	for2DIncr(i,f,to)
		md[i]=(double(-mc[i]));
	return md;
}

MatrixC toMatrixC(MatrixD md) {
	MatrixIndex n,f,t(md.size());
	double min=md[f],max=min;
	for2DIncr(n,f,t) {
		if (md[n]<min)
			min=md[n];
		if (md[n]>max)
			max=md[n];
	}
	if (max!=min)
		for2DIncr(n,f,t) {
		md[n]=(md[n]-min)/(max-min)*255;
	}
	else md.fill(0.0);
	return md;
}

MatrixC toMatrixC2(MatrixD md) {
	MatrixIndex n,f,t(md.size());
	double min=md[f],max=min;
	for2DIncr(n,f,t) {
		if (md[n]<min)
			min=md[n];
		if (md[n]>max)
			max=md[n];
	}
	if (max!=min)
		for2DIncr(n,f,t) {
		md[n]=~int((md[n]-min)/(max-min)*255-128);
	}
	else md.fill(0.0);
	return md;
}
