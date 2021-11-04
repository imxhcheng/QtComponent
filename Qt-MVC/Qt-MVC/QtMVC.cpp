#include "QtMVC.h"

QtMVC::QtMVC(QWidget *parent)
    : QWidget(parent)
{
	setUi();
}

QtMVC::~QtMVC()
{

}

void QtMVC::setUi()
{
	mainLayout = new QVBoxLayout;
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	//����QWidgetҳ����Ҫ�ƶ�������thisָ�룬��Ȼ���������bug
	QWidget *mainWidget = new QWidget(this);
	mainWidget->setLayout(mainLayout);
	mainWidget->setObjectName("mainWidget");
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(mainWidget);
	this->setLayout(layout);

	initMenu();
	initStackWidget();
}

void QtMVC::initMenu()
{
	const QSize btnSize(120, 90);
	QPushButton *firstPageBtn = new QPushButton(QStringLiteral("\n\n\n\n\n��һҳ"));
	firstPageBtn->setObjectName("firstPageBtn");
	firstPageBtn->setCheckable(true);
	firstPageBtn->setChecked(true);
	firstPageBtn->setFixedSize(btnSize);

	QPushButton *secondPageBtn = new QPushButton(QStringLiteral("\n\n\n\n\n�ڶ�ҳ"));
	secondPageBtn->setObjectName("secondPageBtn");
	secondPageBtn->setCheckable(true);
	secondPageBtn->setChecked(true);
	secondPageBtn->setFixedSize(btnSize);

	buttonGroup = new QButtonGroup();
	buttonGroup->setObjectName("buttonGroup");
	buttonGroup->addButton(firstPageBtn);
	buttonGroup->addButton(secondPageBtn);
	//ֻ��һ����ť�ܱ�ѡ��
	buttonGroup->setExclusive(true);
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupSlot(int)));

	QHBoxLayout *menuLayout = new QHBoxLayout();
	menuLayout->setMargin(0);
	menuLayout->setSpacing(0);
	menuLayout->addWidget(firstPageBtn);
	menuLayout->addWidget(secondPageBtn);
	menuLayout->addStretch();

	mainLayout->addLayout(menuLayout);

}

void QtMVC::initStackWidget()
{
	//�����˵�������
	FirstPageView *firstPageView = new FirstPageView(this);
	stackLayout = new QStackedLayout;
	stackLayout->addWidget(firstPageView);
	stackLayout->setCurrentIndex(0);
	mainLayout->addLayout(stackLayout);

}

void QtMVC::buttonGroupSlot(int nIndex)
{
	QPushButton *checkedButton = qobject_cast<QPushButton*>(buttonGroup->checkedButton());
	QString checkedButtonName = checkedButton->objectName();
	if (checkedButtonName.compare(QStringLiteral("firstPageBtn")) == 0)
	{
		qDebug() << QStringLiteral("�����˵�һҳ�˵���ť");
		stackLayout->setCurrentIndex(0);
	}
	else if (checkedButtonName.compare(QStringLiteral("secondPageBtn")) == 0)
	{
		qDebug() << QStringLiteral("�����˵ڶ�ҳ�˵���ť");
		stackLayout->setCurrentIndex(1);
	}

}
