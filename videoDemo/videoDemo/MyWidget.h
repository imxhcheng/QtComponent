#pragma once
#include <QtWidgets>


class MyWidget :	public QWidget
{
	Q_OBJECT
public:
	MyWidget(QWidget *pParent = NULL, Qt::WindowFlags f = 0);
	virtual ~MyWidget();

signals:
	void onResize(QResizeEvent *event);
	void onMousePressEvent(QMouseEvent *event);
	void onMouseMoveEvent(QMouseEvent *event);
	void onMouseReleaseEvent(QMouseEvent *);
	void onMouseDoubleClickEvent(QMouseEvent *);
	void onMouseWheelEvent(QWheelEvent *);
	void onKeyPressEvent(QKeyEvent *event);
	void onKeyReleaseEvent(QKeyEvent *event);


protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event)Q_DECL_OVERRIDE;

	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

	void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

};

