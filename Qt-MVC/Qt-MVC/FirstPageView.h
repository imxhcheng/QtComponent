#pragma once

/*
	Ô­ÎÄÁ´½Ó£ºhttps ://blog.csdn.net/panchang199266/article/details/104849986
*/

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

class FirstPageView : public QWidget
{
	Q_OBJECT
public:
	explicit FirstPageView(QWidget *parent = nullptr);

private:
	void setUi();

private slots:
	void showUiSlot();
	void showFinishedCountResultUISlot();

private:
	QVBoxLayout *mainLayout;

};

