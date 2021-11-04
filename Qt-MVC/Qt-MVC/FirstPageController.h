/*
	Ô­ÎÄÁ´½Ó£ºhttps ://blog.csdn.net/panchang199266/article/details/104849986
*/

#pragma once

#include <QObject>
#include <QThread>

#include "FirstPageModel.h"
#include "Singleton.h"

class FirstPageModel;

class FirstPageController:public QObject
{
	SINGLETON(FirstPageController);

	Q_OBJECT

signals:
	void startedCountSignal();
	void finishedCountSignal();
	void showFinishedCountSignal();

private:
	void initController();


private slots:
	void startCountSlot();
	void quitCountThreadSlot();
	void finishedCountThreadSlot();

private:
	QThread *threadCount;
	FirstPageModel *firstPageModel;

};

