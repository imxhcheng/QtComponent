#pragma once
#include "AISFCompareView.h"
#include "ui_compare_panel.h"
#include "ui_draw_panel.h"
#include "MyDrawWidget.h"
#include "videoDemo.h"
#include "MyWidget.h"

USING_NAMESPACE_AISF


class CompareViewPanel;
class MyCompareViewCallback : public IAISFCompareViewCallback
{
public:
	MyCompareViewCallback(CompareViewPanel *pPage);
	virtual ~MyCompareViewCallback();

	//for the video or audio callback
	virtual int OnCompareViewDraw(AISFCV_CompareViewDrawParam *pParams);
	virtual int DoSingleImgViewInit(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex);
	virtual int DoSingleImgViewUnInit(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex) ;

public:
	//Add Reference,return the refer count after add
	MInt32 AddRef()
	{
		//return IncreaseRef();
		return 0;
	}
	//Release referrence,return the refer count after release
	MInt32 Release()
	{
		//return DecreaseRef();
		return 0;
	}
private:
	CompareViewPanel *m_pComparePage;

};

class CompareViewPanel: public QWidget
{
	Q_OBJECT

public:
	//bForCamera or for edit ,true is for camera ,else if or edit
    CompareViewPanel(videoDemo *pMainWnd, int nPanelType, QWidget *pParentContainer);
    virtual ~CompareViewPanel();
	//the subclass need overrided the functions below
	virtual void Init();
	virtual void UnInit();
	//show page
	virtual void  Show(bool bShow);
	virtual void  OnPanelResize(QResizeEvent *event);

	void SetCompareFiles(QStringList filelist, bool bNeedReload);

	void DoCompareViewDraw(AISFCV_CompareViewDrawParam *pParams);

	void DoAutoFitIn(bool bFitin);
	void ShowFileInfo(bool bShow);


	void DoInitDrawWidget(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex);
	void DoUnInitDrawWidget(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex);
private:
	void ReleaseCompareView();
	void ShowCompareView(bool bShow);

	void ShowFileSwitchUI(bool bShow);
	void UpdateFileSwitchBtnStatus();

	void GetFileListbyPath(char* szPath);
	void DoDisplayFilebyIndex(int nCurFileIndex);

public slots:
	void OnBtnPre_clicked(bool bClicked);
	void OnBtnNext_clicked(bool bClicked);

protected slots:
	void OnAddRectEnd(int nIndex);

private:
	void UpdateDrawViewByViewIndex(int nIndex);
private:
	Ui_compare_root ui;
	Ui_draw_root uiDraw;

	videoDemo*		m_pMainWnd;
	QWidget*		m_pParentContainer;

	AISFCompareView*					m_ptrCompareview;
	MyCompareViewCallback*				m_pCompareViewCallback;

	MyDrawWidget*	m_pDrawWidget[ARC_AISF::AISFCV_CompareView_Num];
	QStringList     m_FileList;
	int			    m_nCurrentFileIndex;
	QString			m_strFileFolder;
};

