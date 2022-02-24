#include "stdafx.h"
#include "CVideoUtils.h"

CVideoUtils *CVideoUtils::m_pInstance = nullptr;

CVideoUtils::CVideoUtils()
{
}

CVideoUtils::~CVideoUtils()
{
}

void CVideoUtils::AVFrameToMat(AVFrame *pFrame, int width, int height, cv::Mat &dst)
{
	int pSize = avpicture_get_size(AV_PIX_FMT_BGR24, width, height);

	cv::Mat dstMat(cv::Size(width, height), CV_8UC3);
	////opencv中用于创建一个mat类型变量。
	dstMat.create(cv::Size(width, height), CV_8UC3);
	memcpy(dstMat.data, pFrame->data[0], pSize);

	dst = dstMat;
}

cv::Mat CVideoUtils::rotateImage(cv::Mat &img, int degree)
{
	if (NULL == img.data || 0 == degree % 360)
	{
		return img;
	}

	if (degree < 0)
	{
		degree += 360;
	}
	switch (degree / 90)
	{
	case 1:
	{
		transpose(img, img);
		flip(img, img, 0);
	}
	break;
	case 2:
	{
		flip(img, img, -1);
	}
	break;
	case 3:
	{
		transpose(img, img);
		flip(img, img, 1);
	}
	break;
	default:
		break;
	}

	return img;
}

double CVideoUtils::getRotation(AVFormatContext *pFormatCtx, int nVideoStream)
{
	AVStream *st = nullptr;

	if (pFormatCtx->nb_streams > 0)
	{
		st = pFormatCtx->streams[nVideoStream];
	}

	AVDictionaryEntry *rotate_tag = NULL;
	rotate_tag = av_dict_get(st->metadata, "rotate", rotate_tag, 0);

	double theta = 0;

	if (rotate_tag && *rotate_tag->value && strcmp(rotate_tag->value, "0"))
	{
		theta = atof(rotate_tag->value);
	}

	theta -= 360 * floor(theta / 360 + 0.9 / 360);

	return theta;
}

int CVideoUtils::getVideoInfo(std::string strFilePath, std::vector<int> &vecKeyFrameNo, int &nTotalFrames, int &nTotalKeyFrames,
	int &nFriKeyFrameIndex, AVCodecID &nCodecId, int &nWidth, int &nHeight, bool &bFinish)
{
	nTotalFrames = 0;
	nTotalKeyFrames = 0;
	av_register_all();
	avformat_network_init();

	std::string strVideoType = strFilePath.substr(strFilePath.size() - 4);
	std::transform(strVideoType.begin(), strVideoType.end(), strVideoType.begin(), ::tolower);

	AVFormatContext *pFormatCtxTemp = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtxTemp, strFilePath.c_str(), NULL, NULL) != 0)
	{
		printf_s("%s\n", "failed");
		return -1;
	}
	//获取视频流信息
	if (avformat_find_stream_info(pFormatCtxTemp, NULL) < 0)
	{
		printf_s("%s\n", "couldn`t find stream info");
		return -1;
	}

	//获取视频数据
	int videoStream = -1;
	for (size_t i = 0; i < pFormatCtxTemp->nb_streams; i++)
	{
		if (pFormatCtxTemp->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			//AVMEDIA_TYPE_VIDEO
			//AV_CODEC_ID_H264
			videoStream = i;
		}
	}

	if (videoStream == -1)
	{
		printf_s("%s\n", "find video stream failed");
		system("pause");
		return -1;
	}

	AVCodecContext	*pCodecCtx = pFormatCtxTemp->streams[videoStream]->codec;
	nCodecId = pCodecCtx->codec_id;
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if (pCodec == NULL)
	{
		printf_s("%s\n", "avcode find decoder failed!");
		//system("pause");
		return -1;
	}
	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("avcode open failed!\n");
		//system("pause");
		return -1;
	}
	//av_dump_format(pFormatCtxTemp, 0, filename, 0); //输出视频信息

	nWidth = pCodecCtx->width;
	nHeight = pCodecCtx->height;

	size_t nIndex = 0;
	size_t keyFrameIndex = 0;
	vecKeyFrameNo.clear();
	AVPacket *packet = av_packet_alloc();
	while (true)
	{
		if (bFinish)
		{
			av_free_packet(packet);
			break;
		}

		if (av_read_frame(pFormatCtxTemp, packet) < 0)
		{
			av_free_packet(packet);
			break;
		}
		if (packet->stream_index == videoStream)
		{
			if (packet->flags & AV_PKT_FLAG_KEY)
			{
				if (0 == vecKeyFrameNo.size())
				{
					nFriKeyFrameIndex = nIndex;
				}
				vecKeyFrameNo.push_back(nIndex);
				keyFrameIndex++;
			}
			nIndex++;

			if (0 == nIndex % 50)
			{
				nTotalFrames = nIndex - nFriKeyFrameIndex;	// 去掉无效帧
				nTotalKeyFrames = keyFrameIndex;
				emit sigUpdateTotalFrames(nTotalFrames);
			}
		}
		av_free_packet(packet);
	}

	nTotalFrames = nIndex - nFriKeyFrameIndex;	// 去掉无效帧
	nTotalKeyFrames = keyFrameIndex;
	av_packet_free(&packet);
	avformat_close_input(&pFormatCtxTemp);
	avformat_free_context(pFormatCtxTemp);
	if (nullptr != pFormatCtxTemp)
	{
		if (nullptr != pCodecCtx)
		{
			pCodec->close(pCodecCtx);
			//avcodec_free_context(&m_pCodecCtx);
			avcodec_close(pCodecCtx);
			av_freep(pCodecCtx);
			pCodecCtx = nullptr;
			pCodec = nullptr;
		}
		avformat_close_input(&pFormatCtxTemp);
		avformat_free_context(pFormatCtxTemp);
		pFormatCtxTemp = nullptr;
	}
	emit sigUpdateTotalFrames(nTotalFrames);

	//avcodec_close(pCodecCtx);
	//avformat_close_input(&pFormatCtxTemp);

	return 0;
}

#ifdef WIN32
void CVideoUtils::SaveAsBMP(const char *dstFilePath, AVFrame *pFrameRGB, int width, int height, int index, int bpp)
{
	char buf[5] = { 0 };   //bmp头     
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	FILE *fp;
	char *filename = new char[255];  //文件存放路径，根据自己的修改     
	sprintf_s(filename, 255, "%s\\%d.bmp", dstFilePath, index);
	auto err = fopen_s(&fp, filename, "wb+");
	if (0 != err) {
		printf_s("open file failed!\n");
		return;
	}
	bmpheader.bfType = 0x4d42;
	bmpheader.bfReserved1 = 0;
	bmpheader.bfReserved2 = 0;
	bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpheader.bfSize = bmpheader.bfOffBits + width * height*bpp / 8;
	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biWidth = width;
	bmpinfo.biHeight = -height;
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = bpp;
	bmpinfo.biCompression = BI_RGB;
	bmpinfo.biSizeImage = (width*bpp + 31) / 32 * 4 * height;
	bmpinfo.biXPelsPerMeter = 100;
	bmpinfo.biYPelsPerMeter = 100;
	bmpinfo.biClrUsed = 0;
	bmpinfo.biClrImportant = 0;
	fwrite(&bmpheader, sizeof(bmpheader), 1, fp);
	fwrite(&bmpinfo, sizeof(bmpinfo), 1, fp);
	fwrite(pFrameRGB->data[0], width*height*bpp / 8, 1, fp);
	fclose(fp);
}
#endif

void CVideoUtils::SaveAsPPM(const char *dstFilePath, AVFrame *pFrame, int width, int height, int index)
{
	char szFilename[32];
	int  y;

	// Open file
	sprintf_s(szFilename, "%s\\%d.ppm", dstFilePath, index);
	FILE *pFile = nullptr;
	auto err = fopen_s(&pFile, szFilename, "wb");

	if (pFile == NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	// Write pixel data
	for (y = 0; y < height; y++)
	{
		fwrite(pFrame->data[0] + y * pFrame->linesize[0], 1, width * 3, pFile);
	}

	// Close file
	fclose(pFile);
}

int CVideoUtils::getNearestKeyFrameIndex(std::vector<int> vecKeyFrameNo, int index)
{
	int nIndex = 0;

	size_t keyFrameSize = vecKeyFrameNo.size();
	std::vector<int> vecKeyFrameNoTemp = vecKeyFrameNo;

	std::vector<int>::iterator result = std::find(vecKeyFrameNoTemp.begin(), vecKeyFrameNoTemp.end(), index);
	if (result == vecKeyFrameNoTemp.begin())
	{
		return index;
	}
	else if (result != vecKeyFrameNoTemp.end())
	{
		nIndex = *(result - 1);
		return nIndex;
	}

	while (true)
	{
		if (keyFrameSize < 1)
		{
			nIndex = index;
			break;
		}
		else if (keyFrameSize < 3)
		{
			if (keyFrameSize < 2)
			{
				nIndex = vecKeyFrameNoTemp[0];
			}
			else
			{
				if (index < vecKeyFrameNoTemp[1])
				{
					nIndex = vecKeyFrameNoTemp[0];
				}
				else
				{
					nIndex = vecKeyFrameNoTemp[1];
				}
			}
			break;
		}
		else
		{
			int median = keyFrameSize / 2;
			if (vecKeyFrameNoTemp[median - 1] < index && vecKeyFrameNoTemp[median] > index)
			{
				nIndex = vecKeyFrameNoTemp[median - 1];
				break;
			}
			else if (vecKeyFrameNoTemp[median - 1] > index)
			{
				std::vector<int>::const_iterator first = vecKeyFrameNoTemp.begin();
				std::vector<int>::const_iterator last = vecKeyFrameNoTemp.begin() + median;
				vecKeyFrameNoTemp.assign(first, last);
				keyFrameSize = vecKeyFrameNoTemp.size();
				continue;
			}
			else if (vecKeyFrameNoTemp[median] < index)
			{
				std::vector<int>::const_iterator first = vecKeyFrameNoTemp.begin() + median;
				std::vector<int>::const_iterator last = vecKeyFrameNoTemp.end();
				vecKeyFrameNoTemp.assign(first, last);
				keyFrameSize = vecKeyFrameNoTemp.size();
				continue;
			}
			else if (vecKeyFrameNoTemp[median - 1] == index)
			{
				nIndex = vecKeyFrameNoTemp[median - 1];
				break;
			}
			else if (vecKeyFrameNoTemp[median] == index)
			{
				nIndex = vecKeyFrameNoTemp[median];
				break;
			}
		}
	}

	return nIndex;
}

