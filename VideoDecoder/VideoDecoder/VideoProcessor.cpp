#include "stdafx.h"
#include "VideoProcessor.h"

#include <iostream>
#include <fstream>

VideoProcessor::VideoProcessor()
{
	m_pVideoDecoder = nullptr;
}

VideoProcessor::~VideoProcessor()
{
}

bool VideoProcessor::openVideo(std::string strVideoPath)
{
	if (strVideoPath.empty())
	{
		return false;
	}

	bool bBareStream = false;
	std::ifstream fin(strVideoPath, std::ios::binary);
	while (!fin.eof())
	{
		//获取文件大小
		fin.seekg(0, std::ios_base::end);
		int nFileLen = fin.tellg();
		if (nFileLen < 5)
		{
			break;
		}

		fin.seekg(0, std::ios_base::beg);
		char szBuf[5] = { 0 };
		fin.read(szBuf, sizeof(char) * 4);
		if ('\0' == szBuf[0] && '\0' == szBuf[1] && '\0' == szBuf[2] && '\1' == szBuf[3])
		{
			bBareStream = true;
		}
		break;
	}
	fin.close();

	if (bBareStream)
	{
		m_pVideoDecoder = DecoderBareStream::getInstance();
		connect(m_pVideoDecoder->m_pVideoUtils, &CVideoUtils::sigUpdateTotalFrames, [=](int nTotalFrames)
		{
			emit sigUpdateTotalFrames(nTotalFrames);
		});
	}
	else
	{
		m_pVideoDecoder = DecoderBase::getInstance();
		connect(m_pVideoDecoder->m_pVideoUtils, &CVideoUtils::sigUpdateTotalFrames, [=](int nTotalFrames)
		{
			emit sigUpdateTotalFrames(nTotalFrames);
		});
	}

	return m_pVideoDecoder->open(strVideoPath);
}

int VideoProcessor::getTotalFrameNumber()
{
	return m_pVideoDecoder->getTotalFrames();
}

void VideoProcessor::getFrame(cv::Mat &mat, int nIndex)
{
	mat = m_pVideoDecoder->getFrame(nIndex);
}
