#pragma once

#include "CVideoUtils.h"

//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//extern "C"
//{
//#include <libavcodec/avcodec.h>//处理原始音频和视频流的解码
//#include <libavutil/opt.h>
//#include <libavutil/channel_layout.h>
//#include <libavutil/common.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/mathematics.h>
//#include <libavutil/samplefmt.h>
//#include <libavformat/avformat.h>//处理解析视频文件并将包含在其中的流分离出来
//#include <libswscale/swscale.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/parseutils.h>
//}
//
//#ifdef _DEBUG
//#pragma comment(lib, "opencv_core249d.lib")
//#pragma comment(lib, "opencv_highgui249d.lib")
//#pragma comment(lib, "opencv_imgproc249d.lib")
////#pragma comment(lib, "opencv_video249d.lib")
//#else
//#pragma comment(lib, "opencv_core249.lib")
//#pragma comment(lib, "opencv_highgui249.lib")
//#pragma comment(lib, "opencv_imgproc249.lib")
////#pragma comment(lib, "opencv_video249.lib")
//#endif // DEBUG
//
//#pragma comment(lib,"avcodec.lib")
//#pragma comment(lib,"avdevice.lib")
//#pragma comment(lib,"avfilter.lib")
//#pragma comment(lib,"avformat.lib")
//#pragma comment(lib,"avutil.lib")
//#pragma comment(lib,"swscale.lib")
//#pragma comment(lib,"postproc.lib")
//#pragma comment(lib,"swresample.lib")

class DecoderInterface
{
public:
	DecoderInterface() { m_pVideoUtils = CVideoUtils::getInstance(); };
	~DecoderInterface() {};

	virtual bool open(std::string strVideoPath) = 0;
	virtual int getTotalFrames() = 0;
	virtual cv::Mat getFrame(int nIndex) = 0;

public:
	CVideoUtils *m_pVideoUtils;

private:

};
