#pragma once

#define __STDC_CONSTANT_MACROS
//#include "DecoderInterface.h"

#include <QObject>

#include <stdio.h>
#include <iostream>
#include <iosfwd>
#include <fstream>
#include <mutex>
//����BMP�ļ�ͷ
#ifdef WIN32    
#include <windows.h>
#include <wingdi.h>
#endif


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
extern "C"
{
#include <libavcodec/avcodec.h>//����ԭʼ��Ƶ����Ƶ���Ľ���
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>//���������Ƶ�ļ��������������е����������
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
}

#ifdef _DEBUG
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
//#pragma comment(lib, "opencv_video249d.lib")
#else
#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
//#pragma comment(lib, "opencv_video249.lib")
#endif // DEBUG

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swresample.lib")


static std::mutex g_mtxInstanceCVideoUtils;

class CVideoUtils : public QObject
{
	Q_OBJECT

public:
	static CVideoUtils *getInstance()
	{
		if (nullptr == m_pInstance)
		{
			g_mtxInstanceCVideoUtils.lock();
			std::lock_guard<std::mutex> lck(g_mtxInstanceCVideoUtils, std::adopt_lock);
			m_pInstance = new CVideoUtils;
		}
		return m_pInstance;
	}

	//AVFrameתMat
	void AVFrameToMat(AVFrame *pFrame, int width, int height, cv::Mat &dst);
	//ͼƬ��ת
	cv::Mat rotateImage(cv::Mat &img, int degree);
	//��ȡ��Ƶ��ת�Ƕ�
	double getRotation(AVFormatContext *pFormatCtx, int nVideoStream);
	//��ȡ��Ƶ֡��Ϣ
	int getVideoInfo(std::string strFilePath, std::vector<int> &vecKeyFrameNo, int &nTotalFrames, int &nTotalKeyFrames,
		int &nFriKeyFrameIndex, AVCodecID &nCodecId, int &nWidth, int &nHeight, bool &bFinish);

#ifdef WIN32
	//����BMP�ļ��ĺ���    
	void SaveAsBMP(const char *dstFilePath, AVFrame *pFrameRGB, int width, int height, int index, int bpp);
#endif

	//����������Ҫ��������SaveFrame�����ܰ�RGB��Ϣ���嵽һ��PPM��ʽ���ļ��С�
	//���ǽ�����һ���򵥵�PPM��ʽ�ļ��������ţ����ǿ��Թ����ġ�
	void SaveAsPPM(const char *dstFilePath, AVFrame *pFrame, int width, int height, int index);

	//��ȡ����Ĺؼ�֡����
	int getNearestKeyFrameIndex(std::vector<int> vecKeyFrameNo, int index);

Q_SIGNALS:
	void sigUpdateTotalFrames(int nTotalFrames);

private:
	explicit CVideoUtils();
	~CVideoUtils();

private:
	static CVideoUtils *m_pInstance;
};

