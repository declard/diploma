#include "basiclayer.h"

double BasicLayer::bound(const MatrixD&m,MatrixIndex point) {
	if (point<0||point>=m.size()) return 0;
	return m.at(point);
}

QImage BasicLayer::toImage(const MatrixD&m) {
	QImage res(m.size(),QImage::Format_RGB16);
	MatrixIndex i,f,t(m.size());
	for2DIncr(i,f,t) {
		char v(m.at(i));
		res.setPixel(i,qRgb(v,v,v));
	}
	return res;
}

MatrixD BasicLayer::fromImage(const QImage&im) {
	MatrixD res(im.size());
	MatrixIndex i,f,t(im.size());
	for2DIncr(i,f,t) {
		QRgb v(im.pixel(i));
		res[i]=qGray(v);
	}
	return res;
}

MatrixIndex BasicLayer::boundariesOffset(const MatrixD&matr) const {
	return boundariesOffset(matr,size());
}

MatrixIndex BasicLayer::boundariesOffset(const MatrixD&f,const MatrixIndex&t) {
	return boundariesOffset(f.size(),t);
}

MatrixIndex BasicLayer::boundariesOffset(const MatrixIndex&f,const MatrixIndex&t) {
	return (t-f)/2;
}

int BasicLayer::calcN(double A,int Np,int Nt) {
	int res;
	if (((Np+Nt)%2)==0)
		res=((int)A)*2+1;
	else res=((int)(A+0.5))*2;
	//qDebug()<<A<<Np<<Nt<<res;
	return res;
}
