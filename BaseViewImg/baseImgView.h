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

	QPixmap	m_img;			// ��ʾͼ
	QPixmap m_imgSrc;		// Դͼ


	double	m_dScale;
	int m_nImageWidth;		// ͼƬ��
	int m_nImageHeight;		// ͼƬ��

	QRect	m_rtTarget;		// ͼƬ��ʾ����

	double	m_dSpaceScale;
	double	m_dSpaceWidth;
	double	m_dSpaceHeight;
};
