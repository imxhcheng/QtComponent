#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ImageViewer.h"

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QApplication>
#include <QFileDialog>
#include <QImage>

class ImageViewer : public QWidget
{
    Q_OBJECT

public:

	enum  Type
	{
		None = 0,
		Amplification,
		Shrink,
		Lift,
		Right,
		Up,
		Down,
		Move,
		Reset
	};

    explicit ImageViewer(QWidget *parent = Q_NULLPTR);

private:
	bool event(QEvent * event);
	void wheelEvent(QWheelEvent* e);     //��껬���¼�

private slots:

	void  onOpenClicked();
	void  paintEvent(QPaintEvent *event);


private:
    Ui::ImageViewerClass ui;

	QPixmap  pix;
	QPixmap  crtPix;
	int action;          //����(�Ŵ�,��С,�ƶ�...)
	int pixW;            //ͼƬ��
	int pixH;            //ͼƬ��

	QRect Paint;         //�滭����
	QImage image;        //�򿪵�ͼƬ


	float ratio;         //���ű���
	QPoint offset;       //һ�ε�ͼƬƫ��ֵ
	QPoint Alloffset;    //��ƫ��
	QLabel label;
	QPushButton  OpenButton;
};
