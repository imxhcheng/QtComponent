#pragma once

#include <QObject>
#include <QImage>
#include <QLabel>


#define MIX_SCALE 0.000001

class baseImgView : public QLabel
{
	Q_OBJECT

public:
	baseImgView(QWidget *parent = nullptr);
	~baseImgView();

	void setImg(const QImage& img);
	void setImg(const QString& strImgPath);
	void setImg(const QPixmap& pixmap);

	void setScale(const double& dScale);

private:
	void paintEvent(QPaintEvent *event);
	void paintImage(QPainter &painter, const QPixmap& img, const double& scale);

	void wheelEvent(QWheelEvent * event);

private:

	QPixmap	m_img;			// œ‘ æÕº
	QPixmap m_imgSrc;		// ‘¥Õº


	double	m_dScale;
	int m_nImageWidth;		// Õº∆¨øÌ
	int m_nImageHeight;		// Õº∆¨∏ﬂ

	QRect	m_rtTarget;		// Õº∆¨œ‘ æ«¯”Ú

	double	m_dSpaceScale;
	double	m_dSpaceWidth;
	double	m_dSpaceHeight;
};
