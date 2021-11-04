#include "MyWidget.h"


MyWidget::MyWidget(QWidget *pParent, Qt::WindowFlags f)
:QWidget(pParent, f)
{
	setAutoFillBackground(true);
}


MyWidget::~MyWidget()
{

}
void MyWidget::paintEvent(QPaintEvent *event)
{
	return QWidget::paintEvent(event);
}
void MyWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	emit onResize(event);
}
void MyWidget::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
	emit onKeyPressEvent(event);
}
void MyWidget::keyReleaseEvent(QKeyEvent *event)
{
	QWidget::keyReleaseEvent(event);
	emit onKeyReleaseEvent(event);
}

void MyWidget::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	emit onMousePressEvent(event);
}
void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
	QWidget::mouseMoveEvent(event);
	emit onMouseMoveEvent(event);
}
void MyWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);
	emit onMouseReleaseEvent(event);
}
void MyWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	QWidget::mouseDoubleClickEvent(event);
	emit onMouseDoubleClickEvent(event);

}	
void MyWidget::wheelEvent(QWheelEvent *event)
{
	QWidget::wheelEvent(event);
	emit onMouseWheelEvent(event);

}