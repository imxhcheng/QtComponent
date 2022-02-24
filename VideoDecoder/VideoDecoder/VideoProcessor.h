#pragma once

#include "DecoderBase.h"
#include "DecoderBareStream.h"

class VideoProcessor : public QObject
{
	Q_OBJECT
public:
	explicit VideoProcessor();
	~VideoProcessor();

	bool openVideo(std::string strVideoPath);
	int getTotalFrameNumber();
	void getFrame(cv::Mat &mat, int nIndex);

Q_SIGNALS:
	void sigUpdateTotalFrames(int nTotalFrames);

private:
	DecoderInterface *m_pVideoDecoder;
};
