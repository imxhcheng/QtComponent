#include "MyDrawWidget.h"

MyDrawWidget::MyDrawWidget(int nViewIndex, QWidget *pParent, Qt::WindowFlags f)
:QWidget(pParent, f), m_bRightMouseDown(false), m_nCurViewIndex(nViewIndex)
{
	setAutoFillBackground(true);
	ResetAllRects();

}


MyDrawWidget::~MyDrawWidget()
{
	
}
void MyDrawWidget::SetCurrentImgRect(QRect imgRect)
{
	if (imgRect.isValid())
	{
		m_lstCurImgRects.push_back(std::move(imgRect));
	}
}
QList<QRect>* MyDrawWidget::GetCurrentImgRects()
{
	return &m_lstCurImgRects;
}
void MyDrawWidget::SetUpdateViewRect(QList<QRect> viewRects)
{

	m_lstUpdateRects.clear();
	m_lstUpdateRects.swap(viewRects);

	m_curDrawRect.setRect(0, 0, 0, 0);
	m_lstCurDrawRects.clear();
	update();
}

void MyDrawWidget::ResetAllRects()
{
	m_curDrawRect.setRect(0, 0, 0, 0);
	m_updateDrawRect.setRect(0, 0, 0, 0);
	m_curImgRect.setRect(0, 0, 0, 0);
}

void MyDrawWidget::paintEvent(QPaintEvent *event)
{
	for (auto rct:m_lstUpdateRects)
	{
		if (rct.isValid())
		{
			QPainter painter(this);
			painter.setPen(QPen(Qt::red, 2, Qt::SolidLine));//set the pen of updated rect from image rect pen 
			painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));//set the brush of updated rect from image rect 
			painter.drawRect(rct.left(), rct.top(), rct.width(), rct.height());
		}
	}

	if (m_curDrawRect.isValid())
	{
		QPainter painter(this);
		painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine));//set the pen of updated rect from image rect pen 
		painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));//set the brush of updated rect from image rect 
		painter.drawRect(m_curDrawRect.left(), m_curDrawRect.top(), m_curDrawRect.width(), m_curDrawRect.height());
	}
}

void MyDrawWidget::resizeEvent(QResizeEvent *event)
{

	QPalette palette;
	palette.setColor(QPalette::Background, QColor(0, 0, 0, 0));
	setPalette(palette);
	setAutoFillBackground(true);

	QWidget::resizeEvent(event);

}
void MyDrawWidget::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
}
void MyDrawWidget::keyReleaseEvent(QKeyEvent *event)
{
	QWidget::keyReleaseEvent(event);
}

void MyDrawWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() & Qt::RightButton)
	{
		//handle right button event here and do not pass the event down
		m_bRightMouseDown = true;
		m_curDrawRect.setTopLeft(event->pos());

		event->accept();
	}
	else
		QWidget::mousePressEvent(event);
}
void MyDrawWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bRightMouseDown)
	{
		//handle right button event here and do not pass the event down

		m_curDrawRect.setBottomRight(event->pos());
		update();

		event->accept();
	}
	else
		QWidget::mouseMoveEvent(event);
}
void MyDrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if ((event->button() & Qt::RightButton) && m_bRightMouseDown)
	{
		//handle right button event here and do not pass the event down

		m_curDrawRect.setBottomRight(event->pos());
		m_bRightMouseDown = false;
		update();
		emit AddRectEnd(m_nCurViewIndex);

		event->accept();
	}
	else
		QWidget::mouseReleaseEvent(event);

}
void MyDrawWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	QWidget::mouseDoubleClickEvent(event);

}	
void MyDrawWidget::wheelEvent(QWheelEvent *event)
{
	QWidget::wheelEvent(event);
}

