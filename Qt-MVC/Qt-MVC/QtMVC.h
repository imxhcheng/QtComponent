#pragma once

#include <QtWidgets/qwidget.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include "FirstPageView.h"

class QtMVC : public QWidget
{
    Q_OBJECT

public:
    QtMVC(QWidget *parent = Q_NULLPTR);
	~QtMVC();

private:
	void setUi();
	void initMenu();
	void initStackWidget();

private slots:
	void buttonGroupSlot(int nIndex);

private:
	QVBoxLayout *mainLayout;
	QButtonGroup *buttonGroup;
	QStackedLayout *stackLayout;

};
