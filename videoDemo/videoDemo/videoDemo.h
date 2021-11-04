#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_videoDemo.h"

class videoDemo : public QMainWindow
{
    Q_OBJECT

public:
    videoDemo(QWidget *parent = Q_NULLPTR);
	QWidget* getWidget();

private:
    Ui::videoDemoClass ui;
};
