#include "FirstPageModel.h"


FirstPageModel::FirstPageModel(QObject *parent /*= nullptr*/)
{

}

void FirstPageModel::countSlot()
{
	int a = 1, b = 2;

	for (int i=0;i<1000;i++)
	{
		for (int j=0;j<1000000;j++)
		{
			//for (int k = 0; k < 1000; j++)
			//{
				int c = a + b;
			//}
		}
	}
	emit Singleton<FirstPageController>::getInstance().finishedCountSignal();
	qDebug() << QStringLiteral("ҳ��1���߼����ܼ�����Ϊ==%1").arg(a+b);
}
