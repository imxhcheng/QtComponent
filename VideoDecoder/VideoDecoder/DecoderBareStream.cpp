#include "stdafx.h"
#include "DecoderBareStream.h"

#define INBUF_SIZE 4096
#define FF_INPUT_BUFFER_PADDING_SIZE   32

uint8_t in_buffer[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE] = { 0 };
DecoderBareStream *DecoderBareStream::m_pInstance = nullptr;

DecoderBareStream::DecoderBareStream()
{
	initData();
}

DecoderBareStream::~DecoderBareStream()
{
	releaseVideo();
	if (m_pInstance)
	{
		delete m_pInstance;
	}
}

void DecoderBareStream::initData()
{
	m_bRunningThread = false;
	m_bFinishThread = false;
	m_vecKeyFramePos.clear();

	m_nInBufferSize = 4096;
	m_nCurPackgeSize = 0;
	m_pCurDataPos = nullptr;
	m_pFile = nullptr;

	m_nCurFrameIndex = 0;
	m_nTotalFrames = 0;
	m_nTotalKeyFrames = 0;
	m_nFriKeyFrameIndex = 0;
	m_nFrameWidth = 0;
	m_nFrameHeight = 0;
	m_dRotation = 0.0;
	m_nVideoStream = -1;
	m_pOutBuffer = nullptr;

	m_pSwsCtx = nullptr;
	m_pCodec = nullptr;
	m_pCodecCtx = nullptr;
	m_pCodecCtx_1 = nullptr;
	m_pFormatCtx = nullptr;
	m_pCodecParserCtx = nullptr;

	m_pFrame = nullptr;
	m_pFrameBGR = nullptr;
}

void DecoderBareStream::releaseVideo()
{
	//关闭扫描视频线程
	if (m_bRunningThread)
	{
		m_bFinishThread = true;
		while (m_bFinishThread)
		{
			Sleep(5);
		}
	}

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = nullptr;
	}

	if (m_pFrame)
	{
		av_frame_free(&m_pFrame);
		m_pFrame = nullptr;
	}
	if (m_pFrameBGR)
	{
		av_frame_free(&m_pFrameBGR);
		m_pFrameBGR = nullptr;
	}

	if (nullptr != m_pCodecCtx_1)
	{
		//avcodec_free_context(&m_pCodecCtx_1);
		m_pCodec->close(m_pCodecCtx_1);
		avcodec_close(m_pCodecCtx_1);
		m_pCodecCtx_1 = nullptr;
		m_pCodecCtx_1 = nullptr;
	}

	if (m_pSwsCtx)
	{
		sws_freeContext(m_pSwsCtx);
		m_pSwsCtx = nullptr;
	}

	if (nullptr != m_pFormatCtx)
	{
		if (nullptr != m_pCodecCtx)
		{
			m_pCodec->close(m_pCodecCtx);
			//avcodec_free_context(&m_pCodecCtx);
			avcodec_close(m_pCodecCtx);
			av_freep(m_pCodecCtx);
			m_pCodecCtx = nullptr;
			m_pCodec = nullptr;
		}
		avformat_close_input(&m_pFormatCtx);
		avformat_free_context(m_pFormatCtx);
		m_pFormatCtx = nullptr;
	}

	//if (nullptr != m_pCodec)
	//{
	//	avcodec_close(&m_pCodec);
	//	m_pCodec = nullptr;
	//}

	if (nullptr != m_pOutBuffer)
	{
		av_freep(&m_pOutBuffer);
		m_pOutBuffer = nullptr;
	}

	m_nFrameWidth = 0;
	m_nFrameHeight = 0;
	m_nVideoStream = -1;
	m_nTotalFrames = 0;
}

bool DecoderBareStream::open(std::string strVideoPath)
{
	releaseVideo();
	initData();

	av_register_all();
	avformat_network_init();

	std::thread thd([=]()
	{
		m_bRunningThread = true;

		AVCodecID nCodecId;
		int nVideoWidth = 0;
		int nVideoHeight = 0;
		m_pVideoUtils->getVideoInfo(strVideoPath, m_vecKeyFrameNo, m_nTotalFrames, m_nTotalKeyFrames, m_nFriKeyFrameIndex, nCodecId, nVideoWidth, nVideoHeight, m_bFinishThread);
		if (!m_bFinishThread)
		{
			printf_s("总帧数：%d  第一个关键帧索引：%d\n", m_nTotalFrames, m_nFriKeyFrameIndex);
			getFramePos(strVideoPath, m_nFriKeyFrameIndex, nCodecId, nVideoWidth, nVideoHeight, m_vecKeyFramePos);
		}
		m_bRunningThread = false;
		m_bFinishThread = false;
	});
	thd.detach();

	//给图片分配空间
	m_pFrame = av_frame_alloc();
	m_pFrameBGR = av_frame_alloc();
	if (nullptr == m_pFrame || nullptr == m_pFrameBGR)
	{
		return false;
	}

	m_pFormatCtx = avformat_alloc_context();

	//打开视频
	int resOpen = avformat_open_input(&m_pFormatCtx, strVideoPath.c_str(), NULL, NULL);
	if (0 != resOpen)
	{
		printf_s("avformat_open_input: %d\n", resOpen);
		return false;
	}

	//找数据流信息
	int resFindStream = avformat_find_stream_info(m_pFormatCtx, NULL);
	if (0 > resFindStream)
	{
		printf_s("avformat_find_stream_info: %d\n", resFindStream);
		return false;
	}

	//视频帧数（第一次显示使用，防止显示为-1）
	int nTotalFrames = m_pFormatCtx->streams[m_nVideoStream]->nb_frames;
	m_nTotalFrames = nTotalFrames - m_nFriKeyFrameIndex;

	//判断流是否为视频
	m_nVideoStream = -1;
	for (size_t i = 0; i < m_pFormatCtx->nb_streams; ++i)
	{
		if (AVMEDIA_TYPE_VIDEO == m_pFormatCtx->streams[i]->codec->codec_type)
		{
			m_nVideoStream = i;
		}
	}
	if (-1 == m_nVideoStream)
	{
		printf_s("find video stream failed!\n");
		return false;
	}

	//找解码器
	m_pCodecCtx_1 = m_pFormatCtx->streams[m_nVideoStream]->codec;
	m_codecID = m_pCodecCtx_1->codec_id;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx_1->codec_id);
	if (nullptr == m_pCodec)
	{
		printf_s("find avcodec failed!\n");
		return false;
	}

	//打开解码器
	int resOpenCodec = avcodec_open2(m_pCodecCtx_1, m_pCodec, NULL);
	if (0 > resOpenCodec)
	{
		printf_s("avcodec open failed!\n");
		return false;
	}

	//获取视频旋转角度
	m_dRotation = m_pVideoUtils->getRotation(m_pFormatCtx, m_nVideoStream);
	m_nFrameWidth = m_pCodecCtx_1->width;
	m_nFrameHeight = m_pCodecCtx_1->height;

	int nPictureSize = avpicture_get_size(AV_PIX_FMT_BGR24, m_nFrameWidth, m_nFrameHeight);
	if (nullptr != m_pOutBuffer)
	{
		av_free(m_pOutBuffer);
	}
	m_pOutBuffer = (uint8_t*)av_malloc(nPictureSize * sizeof(uint8_t));
	avpicture_fill((AVPicture*)m_pFrameBGR, m_pOutBuffer, AV_PIX_FMT_BGR24, m_nFrameWidth, m_nFrameHeight);

	m_pSwsCtx = sws_getContext(m_nFrameWidth, m_nFrameHeight, m_pCodecCtx_1->pix_fmt, m_nFrameWidth, m_nFrameHeight,
		AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

	//H264、H265以文件形式打开
	auto err = fopen_s(&m_pFile, strVideoPath.c_str(), "rb");
	if (nullptr == m_pFile)
	{
		return false;
	}

	m_pCodecParserCtx = NULL;
	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	if (!m_pCodecCtx)
	{
		printf("Could not allocate video codec context\n");
		return false;
	}
	m_pCodecParserCtx = NULL;
	m_pCodecParserCtx = av_parser_init(m_codecID);
	if (!m_pCodecParserCtx)
	{
		printf("Could not allocate video parser context\n");
		return false;
	}
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)//打开编解码器
	{
		printf("Could not open codec\n");
		return false;
	}
	m_pCodecCtx->width = m_nFrameWidth;
	m_pCodecCtx->height = m_nFrameWidth;

	Sleep(5); // sleep 5ms 等待线程加载
	printf_s("\n");

	return true;
}

cv::Mat DecoderBareStream::getFrame(int nIndex)
{
	cv::Mat image;
	if (nIndex > m_nTotalFrames)
	{
		return image;
	}
	if (nIndex - 1 == m_nCurFrameIndex && m_nCurFrameIndex != 0)
	{
		m_nCurFrameIndex = nIndex;
		getNextFrame(image);
		return image;
	}

	while (0 == m_vecKeyFrameNo.size())	// 防止获取不到第一帧
	{
		Sleep(5);
	}

	size_t keyFrameIndex = m_pVideoUtils->getNearestKeyFrameIndex(m_vecKeyFrameNo, nIndex) - m_nFriKeyFrameIndex;
	printf_s("keyframeindex: %d\n", keyFrameIndex);

	while (keyFrameIndex >= m_vecKeyFramePos.size())
	{
		Sleep(5);
	}
	long int pos = m_vecKeyFramePos[keyFrameIndex];
	fseek(m_pFile, pos, SEEK_SET);
	printf_s("keyframepos: %d\n", pos);

	AVPacket packet;
	av_init_packet(&packet);

	int nCurIndex = 0;
	long int nCurPos = 0;
	while (true)
	{
		m_nCurPackgeSize = fread(in_buffer, 1, m_nInBufferSize, m_pFile);//读取文件
		if (m_nCurPackgeSize == 0 || nCurIndex > nIndex - keyFrameIndex)
			break;
		m_pCurDataPos = in_buffer;

		while (m_nCurPackgeSize > 0)
		{
			//输入必须是只包含视频编码数据“裸流”（例如H.264、HEVC码流文件），而不能是包含封装格式的媒体数据（例如AVI、MKV、MP4）。
			int len = av_parser_parse2(		//获取编码数据包
				m_pCodecParserCtx, m_pCodecCtx,
				&packet.data, &packet.size,
				m_pCurDataPos, m_nCurPackgeSize,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			m_pCurDataPos += len;
			m_nCurPackgeSize -= len;

			if (packet.size == 0)
				continue;

			nCurPos += packet.size;

			int got_picture = 0;
			int ret = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_picture, &packet);
			if (got_picture && nCurIndex == nIndex - keyFrameIndex)
			{
				sws_scale(m_pSwsCtx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameBGR->data, m_pFrameBGR->linesize);
				m_pVideoUtils->AVFrameToMat(m_pFrameBGR, m_pCodecCtx->width, m_pCodecCtx->height, image);
				m_pVideoUtils->rotateImage(image, -(int)m_dRotation);

				m_nCurFrameIndex = nCurIndex + keyFrameIndex;
				av_free_packet(&packet);
				return image;
			}
			nCurIndex++;

			av_free_packet(&packet);
		}
	}

	return image;
}

void DecoderBareStream::getNextFrame(cv::Mat &image)
{

	AVPacket packet;
	av_init_packet(&packet);

	int nCurIndex = 0;
	long int nCurPos = 0;
	while (true)
	{
		while (m_nCurPackgeSize > 0)
		{
			//输入必须是只包含视频编码数据“裸流”（例如H.264、HEVC码流文件），而不能是包含封装格式的媒体数据（例如AVI、MKV、MP4）。
			int len = av_parser_parse2(		//获取编码数据包
				m_pCodecParserCtx, m_pCodecCtx,
				&packet.data, &packet.size,
				m_pCurDataPos, m_nCurPackgeSize,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			m_pCurDataPos += len;
			m_nCurPackgeSize -= len;

			if (packet.size == 0)
				continue;

			nCurPos += packet.size;

			int got_picture = 0;
			int ret = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_picture, &packet);
			if (got_picture)
			{
				sws_scale(m_pSwsCtx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameBGR->data, m_pFrameBGR->linesize);
				m_pVideoUtils->AVFrameToMat(m_pFrameBGR, m_pCodecCtx->width, m_pCodecCtx->height, image);
				m_pVideoUtils->rotateImage(image, -(int)m_dRotation);

				av_free_packet(&packet);
				return;
			}
			printf_s("总帧数：%d, 当前帧：%d\n", m_nTotalFrames, nCurIndex);
			nCurIndex++;

			av_free_packet(&packet);
		}

		m_nCurPackgeSize = fread(in_buffer, 1, m_nInBufferSize, m_pFile);//读取文件
		if (m_nCurPackgeSize == 0)
			break;
		m_pCurDataPos = in_buffer;
	}
}

int DecoderBareStream::getFramePos(std::string strFilePath, int nFriKeyFrameIndex, AVCodecID nCodecId, int nVideoWidth, int nVideoHeight, std::vector<long int> &vecKeyFramePos)
{
	// 获取帧位置
	AVPacket packet;
	FILE *pVideo = nullptr;
	fopen_s(&pVideo, strFilePath.c_str(), "rb");
	if (nullptr == pVideo)
	{
		return -1;
	}

	AVCodecParserContext *pCodecParserCtxTemp = NULL;
	AVCodecContext *pCodecCtxTemp = avcodec_alloc_context3(m_pCodec);
	if (!pCodecCtxTemp)
	{
		printf("Could not allocate video codec context\n");
		return -1;
	}
	pCodecParserCtxTemp = av_parser_init(nCodecId);
	if (!pCodecParserCtxTemp)
	{
		printf("Could not allocate video parser context\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtxTemp, m_pCodec, NULL) < 0)//打开编解码器
	{
		printf("Could not open codec\n");
		return -1;
	}

	pCodecCtxTemp->width = nVideoWidth;
	pCodecCtxTemp->height = nVideoHeight;

	vecKeyFramePos.clear();
	uint8_t in_buffer[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE] = { 0 };
	int m_nCurPackgeSize;
	uint8_t *m_pCurDataPos;  //数据指针
	av_init_packet(&packet);

	int index = 0;
	long int nCurPos = 0;
	while (true)
	{
		m_nCurPackgeSize = fread(in_buffer, 1, m_nInBufferSize, pVideo);//读取文件
		if (m_nCurPackgeSize == 0)
			break;
		m_pCurDataPos = in_buffer;

		while (m_nCurPackgeSize > 0)
		{
			//输入必须是只包含视频编码数据“裸流”（例如H.264、HEVC码流文件），而不能是包含封装格式的媒体数据（例如AVI、MKV、MP4）。
			int len = av_parser_parse2(		//获取编码数据包
				pCodecParserCtxTemp, pCodecCtxTemp,
				&packet.data, &packet.size,
				m_pCurDataPos, m_nCurPackgeSize,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			m_pCurDataPos += len;
			m_nCurPackgeSize -= len;

			if (packet.size == 0 || index < nFriKeyFrameIndex)
				continue;

			vecKeyFramePos.push_back(nCurPos);
			nCurPos += packet.size;

			index++;
			av_free_packet(&packet);
		}
	}
	fclose(pVideo);
	return 0;
}