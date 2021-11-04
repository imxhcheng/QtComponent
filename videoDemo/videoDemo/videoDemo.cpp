#include "videoDemo.h"

videoDemo::videoDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

QWidget* videoDemo::getWidget()
{
	return ui.centralWidget;
}
