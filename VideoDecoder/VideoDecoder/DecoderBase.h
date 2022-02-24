#pragma once

//#define __STDC_CONSTANT_MACROS

#include "DecoderInterface.h"
#include "CVideoUtils.h"

#include <mutex>

static std::mutex g_mtxInstanceBase;

class DecoderBase : public DecoderInterface
{
public:
	static DecoderBase *getInstance()
	{
		if (nullptr == m_pInstance)
		{
			g_mtxInstanceBase.lock();
			std::lock_guard<std::mutex> lck(g_mtxInstanceBase, std::adopt_lock);
			m_pInstance = new DecoderBase();
		}
		return m_pInstance;
	}

	virtual bool open(std::string strVideoPath);
	virtual inline int getTotalFrames() { return m_nTotalFrames; };
	virtual cv::Mat getFrame(int nIndex);

private:
	DecoderBase();
	virtual ~DecoderBase();

	void initData();
	void releaseVideo();

	void getNextFrame(cv::Mat &image);

public:

private:
	static DecoderBase *m_pInstance;

	AVFrame *m_pFrame;
	AVFrame *m_pFrameBGR;
	std::vector<int> m_vecKeyFrameNo;

	bool m_bFinishThread;
	bool m_bRunningThread;

	int m_nCurFrameIndex;
	int m_nTotalFrames;
	int m_nTotalKeyFrames;
	int m_nFriKeyFrameIndex;
	int m_nFrameWidth;
	int m_nFrameHeight;
	double m_dRotation;
	size_t m_nVideoStream;
	uint8_t *m_pOutBuffer;

	struct SwsContext *m_pSwsCtx;
	AVCodec *m_pCodec;
	AVCodecContext *m_pCodecCtx;
	AVFormatContext *m_pFormatCtx;
};

