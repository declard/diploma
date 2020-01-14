#ifndef IMAGEEDITORWIDGET_H
#define IMAGEEDITORWIDGET_H

#include "imageviewerwidget.h"
#include "matrix.h"
#include <QPointF>

class ImageEditorWidget : public ImageViewerWidget {
	Q_OBJECT
public:
	ImageEditorWidget(QWidget*parent=0,MatrixIndex s=MatrixIndex(64,64));
	unsigned int brushSize;
	void fill(char);
protected:
	virtual void mouseMoveEvent(QMouseEvent*e);
	virtual void mousePressEvent(QMouseEvent*e);
	virtual void wheelEvent(QWheelEvent*e);
	bool changeState(QPointF p,Qt::MouseButtons b);
signals:
	void brushChanged(int);
};

#endif // IMAGEEDITORWIDGET_H
