#ifndef _AISFCOMPAREVIEW_H_20201028_
#define _AISFCOMPAREVIEW_H_20201028_

#include <QtWidgets>
#include "AISFCommonDef.h"
#include "AISFReferenceInter.h"
#include "ammem.h"

START_NAMESPACE_AISF

typedef enum
{
	AISFCV_VPlayMode_Normal = 0,   //normal display by time
	AISFCV_VPlayMode_Advanced,  //sync frames frame by frame
	AISFCV_VPlayMode_Num
}AISFCV_VIDEO_PLAY_MODE;

typedef enum
{
	AISFCV_CompareView_First = 0,
	AISFCV_CompareView_Second,
	AISFCV_CompareView_Third,
	AISFCV_CompareView_Fourth,
	AISFCV_CompareView_Num
}AISFCV_COMPAREVIEW_INDEX;

typedef struct
{
	int nShowViewIndex;// [-1,3],if -1,means all views are displaying, or only one view is displaying 
}AISFCV_CompareViewDrawParam;

class IAISFCompareViewCallback :public AISFReferenceInter
{
public:
	IAISFCompareViewCallback() {}
	virtual ~IAISFCompareViewCallback() {}

	//for the video or audio callback
	virtual int OnCompareViewDraw(AISFCV_CompareViewDrawParam *pParams) = 0;
	virtual int DoSingleImgViewInit(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex) = 0;
	virtual int DoSingleImgViewUnInit(QWidget *pParentContainer, const AISFCV_COMPAREVIEW_INDEX eViewIndex) = 0;
};

class AISFCompareView :public AISFReferenceInter
{
public:
	//Initialization and Uninitialization
	virtual int Init(QWidget *pParentContainer) = 0;
	virtual void UnInit() = 0;

	//Display 
	virtual void Show(bool bShow) = 0;
	virtual bool IsShow() = 0;

	//set callback
	virtual void SetCallback(IAISFCompareViewCallback* pCallback) = 0;

	//Set video play mode, should be invoked before DoCompare
	virtual void SetVideoPlayMode(AISFCV_VIDEO_PLAY_MODE eVideoPlayMode) = 0;
	//Set Compare files info 
	virtual void DoCompare(const char pchFilePathList[AISFCV_CompareView_Num][MAX_PATH], const int nFileNum) = 0;

	//compareview related operations 
	//according to UI toolbar outside
	virtual void SetAutoFitInFlag(bool bAutoFitIn) = 0;  
	virtual void ShowFileInfo(bool bShow) = 0; 
	virtual void ShowExifInfo(bool bShow) = 0; //Empty implemetation
	virtual void ShowHistogram(bool bShow) = 0; //Empty implemetation
	virtual void DoExit() = 0; //Empty implemetation

	//Get Compare View info
	virtual int GetCompareViewsNum() = 0;
	virtual int GetFrameIndexByViewIndex(AISFCV_COMPAREVIEW_INDEX eViewIndex) = 0;  //Empty implemetation

	virtual char* GetFilePathByViewIndex(AISFCV_COMPAREVIEW_INDEX eViewIndex) = 0;
	virtual int GetViewIndexByFilePath(char* pFilePath) = 0;

	//The AISFCompareView's parent widget coordiate and image cooridate conversion
	//The AISFCompareView's parent widget coordiate to image cooridate
	//[in]:ptCompareSingleView,The AISFCompareView's single GLView widget coordiate
	//[in]:eViewIndex, The viewindex of the ptCompareView located
	//[out]:ptImage,the image cooridate of the located view
	virtual void ViewtoImageCoordinate(const MPOINT &ptCompareSingleView,const AISFCV_COMPAREVIEW_INDEX &eViewIndex, MPOINT &ptImage) = 0;
	//The image coordiate to AISFCompareView's parent widget cooridate
	//[in]:eViewIndex, The viewindex of the ptImage
	//[in]:ptImage,the image cooridate of the located view
	//[out]:ptCompareSingleView,The AISFCompareView's single GLView widget coordiate
	virtual void ImagetoViewCoordinate(const AISFCV_COMPAREVIEW_INDEX &eViewIndex, const MPOINT &ptImage, MPOINT &ptCompareSingleView) = 0;

	virtual const MPBASE_Version *GetVersion() = 0;
};

END_NAMESPACE_AISF

#ifdef __cplusplus
extern "C" {
#endif

	//you should first call AISFCompareViewInitLibs,then AISFCreateCompareView, fininally AISFCompareViewUnInitLibs
	int AISFCompareViewInitLibs();

	int AISFCreateCompareView(const int ClassID,
		const char *pCompareViewLibName,
		AISF_NS::AISFCompareView** ppCompareView);

	void AISFCompareViewUnInitLibs();

#ifdef __cplusplus
}
#endif


#define AISF_CREATE_COMPAREVIEW_EXPORT    "AISFCreateCompareView"
typedef int(*AISFCreateCompareViewFunc)(const int ClassID,
	const char *pCompareViewLibName,
	AISF_NS::AISFCompareView** ppCompareView);


#endif //_AISFCOMPAREVIEW_H_20201028_