#include "FirstPageView.h"
#include "Singleton.h"
#include "FirstPageController.h"

#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

FirstPageView::FirstPageView(QWidget *parent /*= nullptr*/)
	:QWidget(parent)
{
	setUi();
}

void FirstPageView::setUi()
{
	QPushButton *firstPage = new QPushButton(QStringLiteral("我是第一页的页面:点击实现计算功能（业务逻辑功能）"));
	connect(firstPage,SIGNAL(clicked(bool)),this,SLOT(showUiSlot()));

	mainLayout = new QVBoxLayout(this);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(firstPage);
	mainLayout->addStretch();

	connect(&Singleton<FirstPageController>::getInstance(),SIGNAL(showFinishedCountSignal()),this,SLOT(showFinishedCountResultUISlot()));
}

void FirstPageView::showUiSlot()
{
	emit Singleton<FirstPageController>::getInstance().startedCountSignal();

}

void FirstPageView::showFinishedCountResultUISlot()
{
	qDebug() << QStringLiteral("显示计算后界面");
	QMessageBox::warning(this, QStringLiteral("结果界面"),QStringLiteral("显示计算后界面！"),QMessageBox::Yes);
}
