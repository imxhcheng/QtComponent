#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <math.h>

#include <time.h>
#include <sstream>
#include <windows.h>
#include "mybase64.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/interface.h>
#include <QFile>
#include <QByteArray>
#include <QImage>
#include "shiftEncrption.h"
#include "pch.h"
#include "framework.h"

using namespace std;
using namespace cv;

#define KEYWORD 1314
class stop_watch
{
public:
	stop_watch()
		: elapsed_(0)
	{
		QueryPerformanceFrequency(&freq_);
	}
	~stop_watch() {}
public:
	void start()
	{
		/// ��ȡ��ȷʱ��
		QueryPerformanceCounter(&begin_time_);
	}
	void stop()
	{
		LARGE_INTEGER end_time;
		QueryPerformanceCounter(&end_time);
		elapsed_ = (end_time.QuadPart - begin_time_.QuadPart) * 1000000 / freq_.QuadPart;
	}
	void restart()
	{
		elapsed_ = 0;
		start();
	}
	//΢��
	double elapsed()
	{
		return static_cast<double>(elapsed_);
	}
	//����
	double elapsed_ms()
	{
		return elapsed_ / 1000.0;
	}
	//��
	double elapsed_second()
	{
		return elapsed_ / 1000000.0;
	}

private:
	LARGE_INTEGER freq_;
	LARGE_INTEGER begin_time_;
	long long elapsed_;
};


std::string base64Decode(const char* Data, int DataByte) 
{
	//�����
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte) {
		if (*Data != '\r' && *Data != '\n') {
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=') {
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=') {
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		else {
			Data++;
			i++;
		}
	}
	return strDecode;
}


std::string base64Encode(const unsigned char* Data, int DataByte) 
{
	//�����
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//����ֵ
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++) {
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	//��ʣ�����ݽ��б���
	int Mod = DataByte % 3;
	if (Mod == 1) {
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2) {
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}


	return strEncode;
}

//imgType ����png bmp jpg jpeg��opencv�ܹ����б��������ļ�
std::string Mat2Base64(const cv::Mat &img, std::string imgType) 
{
	//Matתbase64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}


cv::Mat Base2Mat(std::string &base64_data) 
{
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}

//int main()
//{
//	//���ܲ���
//	FILE* file = fopen("face_3-azurekinect_00000_color.jpg","rb");
//
//	if (file != NULL)
//	{
//
//#if 1
//		//����
//		//���ļ�
//		fseek(file,0,SEEK_END);
//		long filelen = ftell(file);
//		byte* imgBuff = new byte[filelen + 1];
//		memset(imgBuff, 0x00, filelen + 1);
//
//		//��ȡ�ļ�
//		rewind(file);
//		fread(imgBuff,filelen,1,file);
//		fclose(file);
//
//		//תΪbase64
//		stop_watch timer;
//		char *pOutBuf = new char[filelen * 2];
//		memset(pOutBuf, 0x00, filelen * 2);
//		timer.start();
//		int nBase64 =base64_encode(pOutBuf, imgBuff, filelen, 0);
//
//		delete[]imgBuff;
//		imgBuff = NULL;
//		//����
//		//string strimg = shiftEncrypt(pOutBuf,KEYWORD);
//
//		char* pDst = new char[nBase64*2];
//		int nDstNum = 0;
//		bool bRet = shiftEncrypt2(pOutBuf, nBase64, pDst, nDstNum, KEYWORD);
//
//		timer.stop();
//		cout << timer.elapsed_ms();
//		FILE* fileOut = fopen("img_face.data", "wb+");
//		fwrite(pDst, nDstNum, 1, fileOut);
//		fclose(fileOut);
//		delete[] pOutBuf;
//		delete[] pDst;
//		pOutBuf = NULL;
//#endif
//#if 1
//		//����
//		//FILE* filemiwen = fopen("img.data", "rb");
//		FILE* filemiwen = fopen("img_face.data", "rb");
//		if (filemiwen)
//		{
//			//���ļ�
//			fseek(filemiwen, 0, SEEK_END);
//			long filelenmiwen = ftell(filemiwen);
//			char* imgBuff2 = new char[filelenmiwen + 1];
//			memset(imgBuff2, 0x00, filelenmiwen + 1);
//
//			//��ȡ�ļ�
//			rewind(filemiwen);
//			fread(imgBuff2, filelenmiwen, 1, filemiwen);
//			fclose(filemiwen);
//
//			stop_watch timer1;
//
//			timer1.start();
//			//string strImgSrc = shiftDecrypt(imgBuff2, KEYWORD);
//
//			char* pDst = new char[filelenmiwen * 2];
//			int nDstNum = 0;
//
//			bool bRet = shiftDecrypt2(imgBuff2, filelenmiwen, pDst, nDstNum, KEYWORD);
//
//
//			unsigned char* pChar = (unsigned char*)pDst;
//			unsigned char *pdecBuf = new unsigned char[nDstNum + 1];
//			int nimgNum = base64_decode(pdecBuf, pChar, nDstNum, 0);
//			timer1.stop();
//			cout << timer1.elapsed_ms();
//
//			FILE* fileOut = fopen("img_test_2.jpg", "wb+");
//			fwrite(pdecBuf, nimgNum, 1, fileOut);
//
//			delete[] pdecBuf;
//			delete[] pDst;
//			fclose(fileOut);
//
//			//QImage im;
//			//im.loadFromData(pdecBuf, nimgNum);
//
//			//if (!im.isNull())
//			//{
//			//	cout << "Done!";
//			//}
//			//cout << im.height();
//			//cout << im.width();
//			
//		}
//#endif
//	}
//
//	waitKey(1000000);
//}


//Mat����
int main()
{
	//���ܲ���
	stop_watch timer;

	cv::Mat matSrc = imread("img_test.jpg");

	if (matSrc.empty())
	{
		return -1;
	}
	else 
	{

#if 1
		//����
		//תΪbase64
		timer.start();

		int channelNum = matSrc.channels();

		char* base64Out = new char[matSrc.rows*matSrc.cols*matSrc.channels()];

		//int nLen = base64_encode(base64Out,matSrc.data, matSrc.rows*matSrc.cols * matSrc.channels(), matSrc.rows*matSrc.cols*matSrc.channels());

		std::string strBase64 = base64Decode((const char*)matSrc.data,matSrc.rows*matSrc.cols);

		int nDstNum = 0;
		timer.stop();

		std::string strEncrpted = shiftEncrypt(strBase64, KEYWORD);

		FILE* fileOut = fopen("img_face.data", "wb+");

		fwrite(strEncrpted.c_str(), strEncrpted.length(), 1, fileOut);
		fclose(fileOut);
		cout << timer.elapsed_ms();
#endif
#if 0
		//����
		//FILE* filemiwen = fopen("img.data", "rb");
		FILE* filemiwen = fopen("img_face.data", "rb");
		if (filemiwen)
		{
			//���ļ�
			fseek(filemiwen, 0, SEEK_END);
			long filelenmiwen = ftell(filemiwen);
			char* imgBuff2 = new char[filelenmiwen + 1];
			memset(imgBuff2, 0x00, filelenmiwen + 1);

			//��ȡ�ļ�
			rewind(filemiwen);
			fread(imgBuff2, filelenmiwen, 1, filemiwen);
			fclose(filemiwen);

			stop_watch timer1;

			timer1.start();
			string strImgSrc = shiftDecrypt(imgBuff2, KEYWORD);

			//char* pDst = new char[filelenmiwen * 2];
			//int nDstNum = 0;

			//bool bRet = shiftDecrypt2(imgBuff2, filelenmiwen, pDst, nDstNum, KEYWORD);

			cv::Mat mv = Base2Mat(strImgSrc);

			imwrite("123.jpg",mv);

			timer1.stop();
			cout << timer1.elapsed_ms();
			//QImage im;
			//im.loadFromData(pdecBuf, nimgNum);

			//if (!im.isNull())
			//{
			//	cout << "Done!";
			//}
			//cout << im.height();
			//cout << im.width();
			
		}
#endif
	}

	
}