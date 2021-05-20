#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
	, Paint(10, 10, 810, 810)
	, OpenButton("open",this)
	, Alloffset(0,0)
	, label("100%",this)
{
	ratio = 1.0;             //��ʼ��ͼƬ���ű���
	action = ImageViewer::None;

	OpenButton.setGeometry(822, 220, 60, 25);
	connect(&OpenButton, SIGNAL(clicked()), this, SLOT(onOpenClicked()));

	label.move(840, 260);
	resize(890, 850);

	this->setWindowTitle(QString::fromLocal8Bit("ͼƬ�����"));
}

bool ImageViewer::event(QEvent * event)
{
	static bool press = false;
	static QPoint PreDot;

	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

		//�ж�����Ƿ����������,�����λ���Ƿ��ڻ滭����
		if (mouse->button() == Qt::LeftButton &&Paint.contains(mouse->pos()))
		{
			press = true;
			QApplication::setOverrideCursor(Qt::OpenHandCursor); //���������ʽ
			PreDot = mouse->pos();
		}

	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

		//�ж�����Ƿ�������ͷ�,��֮ǰ���ڻ滭����
		if (mouse->button() == Qt::LeftButton && press)
		{
			QApplication::setOverrideCursor(Qt::ArrowCursor); //�Ļ������ʽ
			press = false;
		}
	}

	if (event->type() == QEvent::MouseMove)              //�ƶ�ͼƬ
	{
		if (press)
		{
			QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

			offset.setX(mouse->x() - PreDot.x());
			offset.setY(mouse->y() - PreDot.y());
			PreDot = mouse->pos();
			action = ImageViewer::Move;

			this->update();
		}
	}
	return QWidget::event(event);
}

void ImageViewer::wheelEvent(QWheelEvent* e)
{
	if (e->delta() < 0) 
	{      //�ϻ�,�Ŵ�

		action = ImageViewer::Shrink;
		this->update();
	}
	else 
	{       //�»�����С
		action = ImageViewer::Amplification;
		this->update();
	}

	e->accept();
}

void ImageViewer::onOpenClicked()
{
	QString str = QFileDialog::getOpenFileName(this,
		"open",
		"D:",
		"img (*.png *.jpg)");

	if (!str.isNull())
	{
		image.load(str);
		pix = pix.fromImage(image);

		crtPix = pix;
		pixW = image.width();            //ͼƬ��
		pixH = image.height();           //ͼƬ��
		qDebug() << str << pixW << pixH;
		this->setWindowTitle("ͼƬ�����(" + str + ")");
	}
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //����

	if (image.isNull())
	{
		return;
	}

	int NowW = ratio *pixW;
	int NowH = ratio *pixH;

	if (action == ImageViewer::Shrink)           //��С
	{
		ratio -= 0.05*ratio;
		if (ratio < 0.018)
			ratio = 0.01;

		/*��ʾ����*/
		QString str;
		str.sprintf("%.0f%", ratio * 100);
		label.setText(str);
		qDebug() << "��С:" << ratio;
	}
	else  if (action == ImageViewer::Amplification)           //�Ŵ�
	{

		ratio += 0.05*ratio;
		if (ratio > 4.5)
			ratio = 5.000;

		/*��ʾ����*/
		QString str;
		str.sprintf("%.0f%", ratio * 100);
		label.setText(str);
		qDebug() << "�Ŵ�:" << ratio;
	}


	if (action == ImageViewer::Amplification || action == ImageViewer::Shrink || action == ImageViewer::Reset)      //����ͼƬ
	{
		NowW = ratio *pixW;
		NowH = ratio *pixH;


		crtPix = pix.scaled(NowW, NowH, Qt::KeepAspectRatio, Qt::SmoothTransformation); //����װ��

		action = ImageViewer::None;
	}

	if (action == ImageViewer::Move)                    //�ƶ�
	{
		int offsetx = Alloffset.x() + offset.x();
		Alloffset.setX(offsetx);

		int offsety = Alloffset.y() + offset.y();
		Alloffset.setY(offsety);
		action = ImageViewer::None;
	}

	if (abs(Alloffset.x()) >= (Paint.width() / 2 + NowW / 2 - 10))    //����Xƫ��ֵ
	{
		if (Alloffset.x() > 0)
			Alloffset.setX(Paint.width() / 2 + NowW / 2 - 10);
		else
			Alloffset.setX(-Paint.width() / 2 + -NowW / 2 + 10);

	}
	if (abs(Alloffset.y()) >= (Paint.height() / 2 + NowH / 2 - 10))    //����Yƫ��ֵ
	{
		if (Alloffset.y() > 0)
			Alloffset.setY(Paint.height() / 2 + NowH / 2 - 10);
		else
			Alloffset.setY(-Paint.height() / 2 + -NowH / 2 + 10);

	}

	int x = Paint.width() / 2 + Alloffset.x() - NowW / 2;
	if (x < 0)
		x = 0;

	int y = Paint.height() / 2 + Alloffset.y() - NowH / 2;
	if (y < 0)
		y = 0;

	int  sx = NowW / 2 - Paint.width() / 2 - Alloffset.x();
	if (sx < 0)
		sx = 0;

	int  sy = NowH / 2 - Paint.height() / 2 - Alloffset.y();
	if (sy < 0)
		sy = 0;

	int w = (NowW - sx) > Paint.width() ? Paint.width() : (NowW - sx);
	if (w > (Paint.width() - x))
		w = Paint.width() - x;

	int h = (NowH - sy) > Paint.height() ? Paint.height() : (NowH - sy);
	if (h > (Paint.height() - y))
		h = Paint.height() - y;



	painter.drawTiledPixmap(x + Paint.x(), y + Paint.y(), w, h, crtPix, sx, sy);             //�滭ͼ��


}
