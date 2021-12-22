#include "baseImgView.h"

baseImgView::baseImgView(QWidget *parent)
	: QLabel(parent)
	, m_nImageWidth(0)
	, m_nImageHeight(0)
{
}

baseImgView::~baseImgView()
{
}

void baseImgView::setImg(const QImage& img)
{
	m_imgSrc = m_img = QPixmap::fromImage(img);

	m_nImageWidth = m_imgSrc.width();
	m_nImageHeight = m_imgSrc.height();

	update();
}

void baseImgView::setImg(const QString& strImgPath)
{
	m_imgSrc = m_img = QPixmap(strImgPath);

	m_nImageWidth = m_imgSrc.width();
	m_nImageHeight = m_imgSrc.height();

	update();
}

void baseImgView::setImg(const QPixmap& pixmap)
{
	m_imgSrc = m_img = pixmap;

	m_nImageWidth = m_imgSrc.width();
	m_nImageHeight = m_imgSrc.height();

	update();
}

void baseImgView::setScale(const double& dScale)
{
	m_dScale = dScale;

	update();
}

void baseImgView::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	paintImage(painter, m_img, m_dScale);
}

void baseImgView::paintImage(QPainter &painter, const QPixmap& img, const double& scale)
{
	QSize sizeTarget = img.size() * scale;
	m_rtTarget = QRect((width() - sizeTarget.width()) / 2, (height() - sizeTarget.height()) / 2, sizeTarget.width(), sizeTarget.height());
	painter.drawPixmap(m_rtTarget, img);

	// 空白区域设定
	if (m_dSpaceScale > MIX_SCALE)
	{
		QSize noSpaceSize = img.size() * scale / (1.0 - m_dSpaceScale);
		painter.drawRect(QRectF(QPointF(0.0, 0.0), QSize(noSpaceSize.width() - 1, noSpaceSize.height() - 1)));

		m_dSpaceWidth = noSpaceSize.width() * m_dSpaceScale / 2;
		m_dSpaceHeight = noSpaceSize.height() * m_dSpaceScale / 2;
		painter.translate(m_dSpaceWidth, m_dSpaceHeight);
	}
	else
	{
		m_dSpaceWidth = 0;
		m_dSpaceHeight = 0;
	}
}

void baseImgView::wheelEvent(QWheelEvent * event)
{
	// 滚轮远离使用者旋转
	if ((event->modifiers() == Qt::ControlModifier) && event->delta() > 0)
	{
		m_dScale = m_dScale * 1.1;
	}
	else if ((event->modifiers() == Qt::ControlModifier) && event->delta() < 0)
	{
		m_dScale = m_dScale / 1.1;
	}
	update();

	QWidget::wheelEvent(event);
}
