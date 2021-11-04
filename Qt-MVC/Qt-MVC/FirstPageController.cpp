#include "FirstPageController.h"

FirstPageController::FirstPageController()
{
	initController();
}

FirstPageController::~FirstPageController()
{

}

void FirstPageController::initController()
{
	connect(this, SIGNAL(startedCountSignal()),this,SLOT(startCountSlot()));
}

void FirstPageController::startCountSlot()
{
	threadCount = new QThread;
	firstPageModel = new FirstPageModel;

	firstPageModel->moveToThread(threadCount);
	connect(threadCount, SIGNAL(started()),firstPageModel,SLOT(countSlot()));
	connect(threadCount, SIGNAL(finished()),threadCount,SLOT(deleteLater()));

	connect(threadCount, SIGNAL(finished()),this,SLOT(finishedCountThreadSlot()));
	connect(this, SIGNAL(finishedCountSignal()),this,SLOT(quitCountThreadSlot()));

	threadCount->start();

}

void FirstPageController::quitCountThreadSlot()
{
	threadCount->quit();
	threadCount->wait();
}

void FirstPageController::finishedCountThreadSlot()
{
	qDebug() << QStringLiteral("完成了计算逻辑");
	emit showFinishedCountSignal();
}
