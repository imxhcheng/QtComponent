#include "videoDemo.h"
#include <QtWidgets/QApplication>
#include "CompareViewPanel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    videoDemo w;
    w.show();

	AISFCompareViewInitLibs();

	CompareViewPanel* pCompareViewer = new CompareViewPanel(&w, 1, w.getWidget());

	pCompareViewer->Init();
	pCompareViewer->Show(true);
	pCompareViewer->ShowFileInfo(true);

	QStringList lstFile;

	//lstFile.append(QString::fromLocal8Bit("C:\\Users\\cxh3662\\Desktop\\00093-1.mp4"));
	lstFile.append(QString::fromLocal8Bit("D:\\素材\\全格式\\视频\\00093-1.mp4"));
	pCompareViewer->SetCompareFiles(lstFile, false);

    return a.exec();

	AISFCompareViewUnInitLibs();

}
