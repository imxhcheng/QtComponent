#include "CompareViewPanel.h"

USING_NAMESPACE_AISF

// for compareview UI callback
MyCompareViewCallback::MyCompareViewCallback(CompareViewPanel *pPage) :m_pComparePage(pPage)
{

}
MyCompareViewCallback::~MyCompareViewCallback()
{

}

int MyCompareViewCallback::OnCompareViewDraw(AISFCV_CompareViewDrawParam *pParams)
{
	if (m_pComparePage)
		m_pComparePage->DoCompareViewDraw(pParams);

	return 0;
}

int MyCompareViewCallback::DoSingleImgViewInit(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex)
{
	if (m_pComparePage)
		m_pComparePage->DoInitDrawWidget(pParentContainer,eViewIndex);

	return 0;
}

int  MyCompareViewCallback::DoSingleImgViewUnInit(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex)
{
	if (m_pComparePage)
		m_pComparePage->DoUnInitDrawWidget(pParentContainer, eViewIndex);

	return 0;
}


CompareViewPanel::CompareViewPanel(videoDemo *pMainWnd, int PanelType, QWidget *pParentContainer)
: m_pCompareViewCallback(NULL)
, m_ptrCompareview(nullptr)
{
	ui.setupUi(this);

	for (int i = 0; i < ARC_AISF::AISFCV_CompareView_Num; i++)
		m_pDrawWidget[i] = NULL;

	m_pMainWnd = pMainWnd;
	m_pParentContainer = pParentContainer;

}
CompareViewPanel::~CompareViewPanel()
{

}
void CompareViewPanel::Init()
{
	if (m_pMainWnd == nullptr || m_pParentContainer == nullptr)
		return;
	//insert to the parent container

	QString strWidgetName = m_pParentContainer->objectName();
	QGridLayout *pGrid = dynamic_cast<QGridLayout*>(m_pParentContainer->layout());

	MyWidget* m_pWidget = new MyWidget();
	m_pWidget->show();
	m_pWidget->setAutoFillBackground(true);
	ui.setupUi(m_pWidget);
	if (pGrid)
		pGrid->addWidget(m_pWidget, 0, 0);


	//connect(ui.btn_pre, SIGNAL(clicked(bool)), this, SLOT(OnBtnPre_clicked(bool)));
	//connect(ui.btn_next, SIGNAL(clicked(bool)), this, SLOT(OnBtnNext_clicked(bool)));

	m_FileList.clear();
	m_nCurrentFileIndex = -1;
	ShowFileSwitchUI(false);	

}

void CompareViewPanel::UnInit()
{
	ReleaseCompareView();
}
//show page
void  CompareViewPanel::Show(bool bShow)
{
	if (bShow)
	{
		//AUTO_TIME_PRINT("CompareViewPanel::Show");
		ShowCompareView(bShow);
	}
	else
	{
		ReleaseCompareView();
	}
	//MyPanelBase::Show(bShow);
}
void CompareViewPanel::OnPanelResize(QResizeEvent *event)
{
	//MyPanelBase::OnPanelResize(event);
}

void CompareViewPanel::DoCompareViewDraw(AISFCV_CompareViewDrawParam *pParams)
{
	//update the rects on m_pDrawWidget
	//if pParams->nShowViewIndex is -1, then all images' rects on m_pDrawWidget should be showed
	//if pParams->nShowViewIndex is [0,3], then the image rect of pParams->nShowViewIndex should be showed
	if (m_ptrCompareview && m_pDrawWidget && pParams)
	{
		int nViewCount = m_ptrCompareview->GetCompareViewsNum();

		if (pParams->nShowViewIndex == -1)
		{
			//show all
			for (int i = 0; i < nViewCount; i++)
			{
				UpdateDrawViewByViewIndex(i);
			}
		}
		else
		{
			//show only pParams->nShowViewIndex image's rect
			//qrect* rtimg = m_pdrawwidget[pparams->nshowviewindex]->getcurrentimgrect();
			//if (rtimg && rtimg->isvalid())
			//	updatedrawviewbyviewindex(pparams->nshowviewindex);
			//else
			//{
			//	qrect rtview(0, 0, 0, 0);
			//	m_pdrawwidget[pparams->nshowviewindex]->setupdateviewrect(rtview);
			//}
		}
	}
}


void CompareViewPanel::SetCompareFiles(QStringList filelist, bool bNeedReload)
{
	int nFileNum = filelist.size() >AISFCV_CompareView_Num ?AISFCV_CompareView_Num : filelist.size();
	char filePath[4][MAX_PATH] = { 0 };

	m_FileList.clear();
	m_nCurrentFileIndex = -1;
	if (nFileNum > 0)
	{
		for (int i = 0; i < nFileNum; i++)
		{
			QString file = filelist.at(i);
			std::string strNativePath = file.toLocal8Bit().data();
			MMemCpy(filePath[i], strNativePath.c_str(), MAX_PATH);
		}

		if (nFileNum == 1)
			GetFileListbyPath(filePath[0]);	

		ShowFileSwitchUI(nFileNum == 1);
	}

	if (m_ptrCompareview)
	{
		if (bNeedReload)
		{
			ReleaseCompareView();
			ShowCompareView(true);

		}
		m_ptrCompareview->DoCompare(filePath, nFileNum);

	}
		
}
void CompareViewPanel::ShowCompareView(bool bShow)
{
	if (nullptr == m_ptrCompareview)
	{
		if (m_pMainWnd == nullptr || m_pParentContainer == nullptr)
			return;

		AISFCreateCompareView(0, nullptr, &m_ptrCompareview);

		if (m_ptrCompareview)
		{
			if (nullptr == m_pCompareViewCallback)
			{
				m_pCompareViewCallback = new MyCompareViewCallback(this);
				m_pCompareViewCallback->AddRef();
			}

			m_ptrCompareview->Init(ui.bottom_layout);
			m_ptrCompareview->SetVideoPlayMode(AISFCV_VPlayMode_Advanced);
			m_ptrCompareview->SetCallback(m_pCompareViewCallback);

			m_ptrCompareview->Show(bShow);

			//m_pMainWnd->DoAutoFitIn();
			//m_pMainWnd->ShowFileInfo();
		}
	}
	else
		m_ptrCompareview->Show(bShow);
}
void CompareViewPanel::ReleaseCompareView()
{
	if (m_ptrCompareview)
	{
		m_ptrCompareview->UnInit();
		m_ptrCompareview = NULL;
	}
	//SafeRelease(m_pCompareViewCallback);
}

//Create one draw rect by right mouse moveing, and get the image rect based on the draw rect
void CompareViewPanel::OnAddRectEnd(int nIndex)
{
	//need get the draw view rects's related image rects
	if (m_pDrawWidget[nIndex])
	{
		QRect* rtView = m_pDrawWidget[nIndex]->GetCurrentViewRect();

		if (m_ptrCompareview && rtView)
		{
			MPOINT ptViewLT = { rtView->left(),rtView->top() };
			MPOINT ptViewRB = {rtView->right(),rtView->bottom()};
			MPOINT ptImgLT = { 0 };
			MPOINT ptImgRB = { 0 };
			QRect rtDrawView = m_pDrawWidget[nIndex]->geometry();
			ARC_AISF::AISFCV_COMPAREVIEW_INDEX eViewIndex = (ARC_AISF::AISFCV_COMPAREVIEW_INDEX)nIndex;
			m_ptrCompareview->ViewtoImageCoordinate(ptViewLT, eViewIndex, ptImgLT);
			m_ptrCompareview->ViewtoImageCoordinate(ptViewRB, eViewIndex, ptImgRB);

			QRect rtImg = { ptImgLT.x,ptImgLT.y,ptImgRB.x - ptImgLT.x,ptImgRB.y - ptImgLT.y };

			m_pDrawWidget[nIndex]->SetCurrentImgRect(rtImg);
		}

		/*¸üÐÂ¿ò*/
		UpdateDrawViewByViewIndex(nIndex);
	}
}

//update the display rects based on the image rects
void CompareViewPanel::UpdateDrawViewByViewIndex(int nIndex)
{
	//update the image's rects on m_pDrawWidget
	if (nIndex<AISFCV_CompareView_First || nIndex>AISFCV_CompareView_Fourth || m_pDrawWidget[nIndex] == NULL)
		return;

	//int nFrameIndex = m_ptrCompareview->GetFrameIndexByViewIndex((ARC_AISF::AISFCV_COMPAREVIEW_INDEX)nIndex);
	//LOGCATE("CompareViewPanel::DoCompareViewDraw viewindex=%d,current frameIndex=%d", nIndex, nFrameIndex);

	QList<QRect>* lstImg = m_pDrawWidget[nIndex]->GetCurrentImgRects();
	QList<QRect> lstNewRcts;

	for (auto pRct:*lstImg)
	{
		if (pRct.isValid())
		{
			MPOINT ptImgLT = { pRct.left(),pRct.top() };
			MPOINT ptImgRB = { pRct.right(),pRct.bottom() };
			MPOINT ptViewLT = { 0 };
			MPOINT ptViewRB = { 0 };
			m_ptrCompareview->ImagetoViewCoordinate((ARC_AISF::AISFCV_COMPAREVIEW_INDEX)nIndex, ptImgLT, ptViewLT);
			m_ptrCompareview->ImagetoViewCoordinate((ARC_AISF::AISFCV_COMPAREVIEW_INDEX)nIndex, ptImgRB, ptViewRB);

			QRect rtView = { ptViewLT.x,ptViewLT.y,ptViewRB.x - ptViewLT.x,ptViewRB.y - ptViewLT.y };

			QRect rtDrawView = m_pDrawWidget[nIndex]->geometry();

			//LOGCATE("CompareViewPanel::DoCompareViewDraw rtDrawRoot={%d,%d,%d,%d},imgRect={%d,%d,%d,%d}, eViewIndex=%d, get ViewRect={%d,%d,%d,%d}",
			//	rtDrawView.left(), rtDrawView.top(), rtDrawView.right(), rtDrawView.bottom(),
			//	ptImgLT.x, ptImgLT.y, ptImgRB.x, ptImgRB.y, nIndex, ptViewLT.x, ptViewLT.y, ptViewRB.x, ptViewRB.y);

			lstNewRcts.push_back(rtView);
		}
	}

	m_pDrawWidget[nIndex]->SetUpdateViewRect(lstNewRcts);

	/**/

	
}

void CompareViewPanel::DoAutoFitIn(bool bFitin)
{
	if (m_ptrCompareview)
		m_ptrCompareview->SetAutoFitInFlag(bFitin);

}
void CompareViewPanel::ShowFileInfo(bool bShow)
{
	if (m_ptrCompareview)
		m_ptrCompareview->ShowFileInfo(bShow);
}


void CompareViewPanel::ShowFileSwitchUI(bool bShow)
{
	//if (ui.bottom_layout_cover)
		//ui.bottom_layout_cover->setVisible(bShow);

	if (bShow)
	{
		UpdateFileSwitchBtnStatus();		
	}
}

void CompareViewPanel::UpdateFileSwitchBtnStatus()
{
	if (m_FileList.size() <= 1 )
	{
		//ui.btn_next->setEnabled(false);
		//ui.btn_pre->setEnabled(false);
	}
	else
	{
		//ui.btn_pre->setEnabled(m_nCurrentFileIndex >= 1);
		//ui.btn_next->setEnabled(m_nCurrentFileIndex < (m_FileList.size() - 1));
	}
}

void CompareViewPanel::OnBtnPre_clicked(bool bClicked)
{
	if (m_nCurrentFileIndex >=0)
	{
		m_nCurrentFileIndex--;
		UpdateFileSwitchBtnStatus();

		DoDisplayFilebyIndex(m_nCurrentFileIndex);
	}
}

void CompareViewPanel::OnBtnNext_clicked(bool bClicked)
{
	if (m_nCurrentFileIndex <= (m_FileList.size() - 1))
	{
		m_nCurrentFileIndex++;
		UpdateFileSwitchBtnStatus();

		DoDisplayFilebyIndex(m_nCurrentFileIndex);
	}
}

void CompareViewPanel::GetFileListbyPath(char* szPath)
{
	QFileInfo fileInfo(szPath);
	m_strFileFolder = "";
	QString fileName = fileInfo.fileName();
	m_strFileFolder = fileInfo.absolutePath();

	QDir dir(m_strFileFolder);
	m_FileList = dir.entryList(QDir::Files);

	for (int i = 0; i < m_FileList.size(); i++)
	{
		QString strPath = m_FileList.at(i);

		if (strPath.compare(fileName, Qt::CaseInsensitive) == 0)
		{
			m_nCurrentFileIndex = i;
			break;
		}
	}
}

void CompareViewPanel::DoDisplayFilebyIndex(int nCurFileIndex)
{
	if (nCurFileIndex < 0 || nCurFileIndex >= m_FileList.size() || m_FileList.size() <= 1)
		return;

	QString strPath = m_FileList.at(nCurFileIndex);

	int nFileNum =1;
	char filePath[4][MAX_PATH] = { 0 };

	std::string strFileName = strPath.toLocal8Bit().data();;
	std::string strFilePath = m_strFileFolder.toLocal8Bit().data();
	sprintf(filePath[0], "%s/%s", strFilePath.c_str(), strFileName.c_str());

	//LOGCATE("CompareViewPanel::DoDisplayFilebyIndex fileindex=%d,filePath=%s",nCurFileIndex,filePath[0]);

	if (m_ptrCompareview)
	{
		ReleaseCompareView();
		ShowCompareView(true);

		m_ptrCompareview->DoCompare(filePath, nFileNum);

	}
}

//add draw rect widget to the compare single glview root panel
void CompareViewPanel::DoInitDrawWidget(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex)
{
	if (eViewIndex < AISFCV_CompareView_First || 
		eViewIndex>AISFCV_CompareView_Fourth || 
		nullptr == pParentContainer)
		return;

	//if (IniSetting::IfShowDrawPanel(IniSetting::SETTING_COMPAREVIEW))
	{
		if (m_pDrawWidget[eViewIndex] == NULL)
		{
			m_pDrawWidget[eViewIndex] = new MyDrawWidget(eViewIndex);
			uiDraw.setupUi(m_pDrawWidget[eViewIndex]);
		
			QGridLayout *pGrid = dynamic_cast<QGridLayout*>(pParentContainer->layout());
			if(pGrid)
				pGrid->addWidget(m_pDrawWidget[eViewIndex], 0, 0);
			
			m_pDrawWidget[eViewIndex]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

			connect(m_pDrawWidget[eViewIndex], SIGNAL(AddRectEnd(int)), this, SLOT(OnAddRectEnd(int)));

			pParentContainer->setFocus();
		}
	}

}
//remove the draw rect widget
void CompareViewPanel::DoUnInitDrawWidget(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex)
{
	if (eViewIndex < AISFCV_CompareView_First ||
		eViewIndex>AISFCV_CompareView_Fourth ||
		nullptr == pParentContainer)
		return;
	
	if(m_pDrawWidget[eViewIndex])
	{
		QGridLayout *pGrid = dynamic_cast<QGridLayout*>(pParentContainer->layout());

		QLayoutItem *child;
		while ((child = pGrid->takeAt(0)) != 0)
		{
			QWidget* pWidget = child->widget();
			if (pWidget == m_pDrawWidget[eViewIndex])
			{
				pWidget->setParent(NULL);

				//disconnect(m_pDrawWidget[eViewIndex], 0, this, 0);
				//SafeDelete(m_pDrawWidget[eViewIndex]);

				delete child;
				break;
			}
		}
	}
}



