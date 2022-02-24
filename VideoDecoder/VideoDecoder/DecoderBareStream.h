#pragma once

#include "DecoderInterface.h"
#include "CVideoUtils.h"

#include <mutex>

static std::mutex g_mtxInstanceBareStream;

class DecoderBareStream : public DecoderInterface
{
public:
	static DecoderBareStream *getInstance()
	{
		if (nullptr == m_pInstance)
		{
			g_mtxInstanceBareStream.lock();
			std::lock_guard<std::mutex> lck(g_mtxInstanceBareStream, std::adopt_lock);
			m_pInstance = new DecoderBareStream;
		}
		return m_pInstance;
	}

	virtual bool open(std::string strVideoPath);
	virtual inline int getTotalFrames() { return m_nTotalFrames; };
	virtual cv::Mat getFrame(int nIndex);

private:
	explicit DecoderBareStream();
	~DecoderBareStream();

	void initData();
	void releaseVideo();

	//int getNearestKeyFrameIndex(std::vector<int> vecKeyFrameNo, int index);

	int getFramePos(std::string strFilePath, int nFriKeyFrameIndex, AVCodecID nCodecId, int nVideoWidth, int nVideoHeight, std::vector<long int> &vecKeyFramePos);
	void getNextFrame(cv::Mat &image);

public:
	
private:
	static DecoderBareStream *m_pInstance;

	AVFrame *m_pFrame;
	AVFrame *m_pFrameBGR;
	std::map<int, int> m_mapKeyIndex;
	std::vector<int> m_vecKeyFrameNo;
	std::vector<long int> m_vecKeyFramePos;

	bool m_bFinishThread;	//关闭扫描线程使用
	bool m_bRunningThread;	//线程是否正在运行

	int m_nCurFrameIndex;
	int m_nTotalFrames;
	int m_nTotalKeyFrames;
	int m_nFriKeyFrameIndex;
	int m_nFrameWidth;
	int m_nFrameHeight;
	double m_dRotation;
	size_t m_nVideoStream;
	uint8_t *m_pOutBuffer;

	int m_nInBufferSize;
	int m_nCurPackgeSize;
	uint8_t *m_pCurDataPos;	//数据指针

	FILE *m_pFile;
	AVCodecID m_codecID;
	AVCodecParserContext *m_pCodecParserCtx;

	AVCodec *m_pCodec;
	struct SwsContext *m_pSwsCtx;
	AVCodecContext *m_pCodecCtx;

	//FFPEGE 通用解码使用
	AVCodecContext *m_pCodecCtx_1;
	AVFormatContext *m_pFormatCtx;
};

