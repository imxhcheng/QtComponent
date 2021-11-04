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
	QPushButton *firstPage = new QPushButton(QStringLiteral("���ǵ�һҳ��ҳ��:���ʵ�ּ��㹦�ܣ�ҵ���߼����ܣ�"));
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
	qDebug() << QStringLiteral("��ʾ��������");
	QMessageBox::warning(this, QStringLiteral("�������"),QStringLiteral("��ʾ�������棡"),QMessageBox::Yes);
}
