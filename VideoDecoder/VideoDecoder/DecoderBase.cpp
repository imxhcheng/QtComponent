#include "stdafx.h"
#include "DecoderBase.h"

#include <windows.h>

DecoderBase *DecoderBase::m_pInstance = nullptr;


DecoderBase::DecoderBase()
{
	initData();
}

DecoderBase::~DecoderBase()
{
	releaseVideo();
	if (m_pInstance)
	{
		delete m_pInstance;
	}
}

void DecoderBase::initData()
{
	m_bRunningThread = false;
	m_bFinishThread = false;

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
	m_pFormatCtx = nullptr;

	m_pFrame = nullptr;
	m_pFrameBGR = nullptr;
}

void DecoderBase::releaseVideo()
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

bool DecoderBase::open(std::string strVideoPath)
{
	releaseVideo();
	initData();

	av_register_all();
	avformat_network_init();

	std::thread thd([&]()
	{
		m_bRunningThread = true;
		AVCodecID nCodecId;
		int nVideoWidth = 0;
		int nVideoHeight = 0;
		m_pVideoUtils->getVideoInfo(strVideoPath, m_vecKeyFrameNo, m_nTotalFrames, m_nTotalKeyFrames,
			m_nFriKeyFrameIndex, nCodecId, nVideoWidth, nVideoHeight, m_bFinishThread);
		printf_s("总帧数：%d  第一个关键帧索引：%d\n", m_nTotalFrames, m_nFriKeyFrameIndex);
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
	m_pCodecCtx = m_pFormatCtx->streams[m_nVideoStream]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
	if (nullptr == m_pCodec)
	{
		printf_s("find avcodec failed!\n");
		return false;
	}

	//打开解码器
	int resOpenCodec = avcodec_open2(m_pCodecCtx, m_pCodec, NULL);
	if (0 > resOpenCodec)
	{
		printf_s("avcodec open failed!\n");
		return false;
	}

	m_dRotation = m_pVideoUtils->getRotation(m_pFormatCtx, m_nVideoStream);
	m_nFrameWidth = m_pCodecCtx->width;
	m_nFrameHeight = m_pCodecCtx->height;
	int nPictureSize = avpicture_get_size(AV_PIX_FMT_BGR24, m_nFrameWidth, m_nFrameHeight);
	if (nullptr != m_pOutBuffer)
	{
		av_free(m_pOutBuffer);
	}
	m_pOutBuffer = (uint8_t*)av_malloc(nPictureSize * sizeof(uint8_t));
	avpicture_fill((AVPicture*)m_pFrameBGR, m_pOutBuffer, AV_PIX_FMT_BGR24, m_nFrameWidth, m_nFrameHeight);

	m_pSwsCtx = sws_getContext(m_nFrameWidth, m_nFrameHeight, m_pCodecCtx->pix_fmt, m_nFrameWidth, m_nFrameHeight,
		AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
	//输出视频信息
	av_dump_format(m_pFormatCtx, 0, strVideoPath.c_str(), 0);

	Sleep(20);	//sleep 20ms 等待线程加载

	//int nIndex = 0;
	//AVPacket *packet = av_packet_alloc();
	//while (true)
	//{
	//	if (av_read_frame(m_pFormatCtx, packet) < 0)
	//	{
	//		av_free_packet(packet);
	//		break;
	//	}
	//	if (packet->stream_index == m_nVideoStream)
	//	{		
	//		int nCurPts = packet->pts;
	//		if (packet->flags & AV_PKT_FLAG_KEY)
	//		{
	//			m_mapKeyIndex[nIndex] = nCurPts;
	//			m_vecKeyFrameNo.push_back(nIndex);
	//		}
	//		avcodec_send_packet(m_pCodecCtx, packet);
	//		int ret = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
	//		if (ret >= 0)
	//		{
	//			cv::Mat image;
	//			//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
	//			sws_scale(m_pSwsCtx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height,
	//				m_pFrameBGR->data, m_pFrameBGR->linesize);
	//			AVFrameToMat(m_pFrameBGR, m_pCodecCtx->width, m_pCodecCtx->height, image);
	//			cv::imshow("frame", image);
	//			cv::waitKey(5);
	//		}
	//		printf_s("nIndex: %d  pts: %d\n", nIndex++, nCurPts);
	//		av_free_packet(packet);
	//	}
	//	else
	//	{
	//		av_free_packet(packet);
	//	}
	//}
	printf_s("\n");

	return true;
}

cv::Mat DecoderBase::getFrame(int nIndex)
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

	//跳转到开始位置
	std::cout << std::endl << "start time：" << m_pFormatCtx->start_time << std::endl;
	std::cout << "duration：" << m_pFormatCtx->duration << std::endl;
	std::cout << "bit rate：" << m_pFormatCtx->bit_rate << std::endl;

	bool bSpaceFrameExit = false;
	int nKeyIndex = m_pVideoUtils->getNearestKeyFrameIndex(m_vecKeyFrameNo, nIndex) - m_nFriKeyFrameIndex;
	auto seek_target = m_pFormatCtx->start_time;
	//int ret = av_seek_frame(m_pFormatCtx, m_nVideoStream, seek_target, AVSEEK_FLAG_BACKWARD);
	int ret = avformat_seek_file(m_pFormatCtx, -1, 0, seek_target, seek_target + 2000, AVSEEK_FLAG_BACKWARD);
	//int ret = av_seek_frame(m_pFormatCtx, -1, (double)m_pFormatCtx->start_time, AVSEEK_FLAG_BACKWARD);
	if (ret < 0)
	{
		bSpaceFrameExit = true;
		ret = av_seek_frame(m_pFormatCtx, m_nVideoStream, seek_target, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD);
		if (ret < 0)
		{
			printf_s("无法定位到起始位置\n");
			return image;
		}
	}

	int nCurIndex = 0;
	int nSpaceFrameIndex = 0;
	AVPacket *packet = av_packet_alloc();
	while (true)
	{
		if (av_read_frame(m_pFormatCtx, packet) < 0)
		{
			av_free_packet(packet);
			break;
		}
		if (packet->stream_index == m_nVideoStream)
		{
			if (bSpaceFrameExit && (m_nFriKeyFrameIndex - 1 > nSpaceFrameIndex))// 第一个关键帧之前的帧
			{
				nSpaceFrameIndex++;
				continue;
			}
			if (nCurIndex >= nKeyIndex && nCurIndex != nIndex)
			{
				avcodec_send_packet(m_pCodecCtx, packet);
				avcodec_receive_frame(m_pCodecCtx, m_pFrame);
			}
			if (nCurIndex != nIndex)
			{
				nCurIndex++;
				av_free_packet(packet);
				continue;
			}

			if (avcodec_send_packet(m_pCodecCtx, packet) < 0)
			{
				av_free_packet(packet);
				break;
			}
			int ret = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
			if (ret >= 0)
			{
				//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
				sws_scale(m_pSwsCtx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height,
					m_pFrameBGR->data, m_pFrameBGR->linesize);
				m_pVideoUtils->AVFrameToMat(m_pFrameBGR, m_pCodecCtx->width, m_pCodecCtx->height, image);
				m_pVideoUtils->rotateImage(image, -(int)m_dRotation);
				//cv::imshow("temp", image);
				//cv::waitKey(0);

				int nCurPts = packet->pts;
				printf_s("pts: %d\n\n", nCurPts);
				av_free_packet(packet);
				break;
			}
			av_free_packet(packet);
		}
		else
		{
			av_free_packet(packet);
		}
	}

	av_packet_free(&packet);
	m_nCurFrameIndex = nIndex;
	return image;
}

void DecoderBase::getNextFrame(cv::Mat &image)
{
	AVPacket *packet = av_packet_alloc();
	while (true)
	{
		if (av_read_frame(m_pFormatCtx, packet) < 0)
		{
			av_free_packet(packet);
			break;
		}
		if (packet->stream_index == m_nVideoStream)
		{
			if (avcodec_send_packet(m_pCodecCtx, packet) < 0)
			{
				av_free_packet(packet);
				continue;
			}
			int ret = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
			if (ret >= 0)
			{
				//转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
				sws_scale(m_pSwsCtx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height,
					m_pFrameBGR->data, m_pFrameBGR->linesize);
				m_pVideoUtils->AVFrameToMat(m_pFrameBGR, m_pCodecCtx->width, m_pCodecCtx->height, image);
				m_pVideoUtils->rotateImage(image, -(int)m_dRotation);

				av_free_packet(packet);
				int nCurPts = packet->pts;
				printf_s("pts: %d\n\n", nCurPts);
				break;
			}
			av_free_packet(packet);
		}
		else
		{
			av_free_packet(packet);
		}
	}
	av_packet_free(&packet);
}

