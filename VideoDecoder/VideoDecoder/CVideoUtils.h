#pragma once

#define __STDC_CONSTANT_MACROS
//#include "DecoderInterface.h"

#include <QObject>

#include <stdio.h>
#include <iostream>
#include <iosfwd>
#include <fstream>
#include <mutex>
//定义BMP文件头
#ifdef WIN32    
#include <windows.h>
#include <wingdi.h>
#endif


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
extern "C"
{
#include <libavcodec/avcodec.h>//处理原始音频和视频流的解码
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>//处理解析视频文件并将包含在其中的流分离出来
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

	//AVFrame转Mat
	void AVFrameToMat(AVFrame *pFrame, int width, int height, cv::Mat &dst);
	//图片旋转
	cv::Mat rotateImage(cv::Mat &img, int degree);
	//获取视频旋转角度
	double getRotation(AVFormatContext *pFormatCtx, int nVideoStream);
	//获取视频帧信息
	int getVideoInfo(std::string strFilePath, std::vector<int> &vecKeyFrameNo, int &nTotalFrames, int &nTotalKeyFrames,
		int &nFriKeyFrameIndex, AVCodecID &nCodecId, int &nWidth, int &nHeight, bool &bFinish);

#ifdef WIN32
	//保存BMP文件的函数    
	void SaveAsBMP(const char *dstFilePath, AVFrame *pFrameRGB, int width, int height, int index, int bpp);
#endif

	//现在我们需要做的是让SaveFrame函数能把RGB信息定稿到一个PPM格式的文件中。
	//我们将生成一个简单的PPM格式文件，请相信，它是可以工作的。
	void SaveAsPPM(const char *dstFilePath, AVFrame *pFrame, int width, int height, int index);

	//获取最近的关键帧索引
	int getNearestKeyFrameIndex(std::vector<int> vecKeyFrameNo, int index);

Q_SIGNALS:
	void sigUpdateTotalFrames(int nTotalFrames);

private:
	explicit CVideoUtils();
	~CVideoUtils();

private:
	static CVideoUtils *m_pInstance;
};

