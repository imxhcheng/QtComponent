/*
	Ô­ÎÄÁ´½Ó£ºhttps ://blog.csdn.net/panchang199266/article/details/104849986
*/

#pragma once
#include "Singleton.h"
#include "FirstPageController.h"
#include "FirstPageView.h"
#include <QObject>
#include <QDebug>

class FirstPageModel :public QObject
{
	Q_OBJECT
public:
	explicit FirstPageModel(QObject *parent = nullptr);

public slots:
	void countSlot();
};

