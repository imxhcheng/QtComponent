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
	void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件

private slots:

	void  onOpenClicked();
	void  paintEvent(QPaintEvent *event);


private:
    Ui::ImageViewerClass ui;

	QPixmap  pix;
	QPixmap  crtPix;
	int action;          //动作(放大,缩小,移动...)
	int pixW;            //图片宽
	int pixH;            //图片高

	QRect Paint;         //绘画区域
	QImage image;        //打开的图片


	float ratio;         //缩放比例
	QPoint offset;       //一次的图片偏移值
	QPoint Alloffset;    //总偏移
	QLabel label;
	QPushButton  OpenButton;
};
