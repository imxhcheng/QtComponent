#pragma once
#include <QtWidgets>
#include "AISFCompareView.h"

class MyDrawWidget :public QWidget
{
	Q_OBJECT
public:
	MyDrawWidget(int nViewIndex,QWidget *pParent = NULL, Qt::WindowFlags f = 0);
	virtual ~MyDrawWidget();

public:
	QRect* GetCurrentViewRect() { return &m_curDrawRect; };
	void SetCurrentImgRect(QRect imgRect);
	QList<QRect>* GetCurrentImgRects();
	void SetUpdateViewRect(QList<QRect> viewRects);

	void ResetAllRects();

signals:
	void AddRectEnd(int nIndex);

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

private:

	int				m_nCurViewIndex;
	bool			m_bRightMouseDown;

	QRect			m_curDrawRect;				//View Rect
	QRect			m_updateDrawRect;			//View Rect
	QRect			m_curImgRect;				// Image Rect
	QList<QRect>	m_lstCurDrawRects;
	QList<QRect>	m_lstCurImgRects;
	QList<QRect>	m_lstUpdateRects;

};

