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

#define ENCRYPT_SIZE   131072    //128 * 1024
#define DNCRYPT_SIZE   349528
#define MAX_IMAGE_SIZE 3145728  // 3 * 1024 * 1024 

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
		/// 获取精确时间
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
	//微秒
	double elapsed()
	{
		return static_cast<double>(elapsed_);
	}
	//毫秒
	double elapsed_ms()
	{
		return elapsed_ / 1000.0;
	}
	//秒
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
	//解码表
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
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
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
	//对剩余数据进行编码
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

//imgType 包括png bmp jpg jpeg等opencv能够进行编码解码的文件
std::vector<uchar> Mat2Base64(const cv::Mat &img, std::string imgType)
{
	//Mat转base64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return vecImg;
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

int MatToByteArray(const cv::Mat mat, std::vector<unsigned char>& buff)
{
	if (mat.empty())
	{
		return -1;
	}
	std::vector<int> param = std::vector<int>(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 90; // default(95) 0-100
	cv::imencode(".jpg", mat, buff, param);
	return 0;
}

#if 0
//int main()
//{
//	//加密测试
//	FILE* file = fopen("face_3-azurekinect_00000_color.jpg","rb");
//
//	if (file != NULL)
//	{
//
//#if 1
//		//加密
//		//打开文件
//		fseek(file,0,SEEK_END);
//		long filelen = ftell(file);
//		byte* imgBuff = new byte[filelen + 1];
//		memset(imgBuff, 0x00, filelen + 1);
//
//		//读取文件
//		rewind(file);
//		fread(imgBuff,filelen,1,file);
//		fclose(file);
//
//		//转为base64
//		stop_watch timer;
//		char *pOutBuf = new char[filelen * 2];
//		memset(pOutBuf, 0x00, filelen * 2);
//		timer.start();
//		int nBase64 =base64_encode(pOutBuf, imgBuff, filelen, 0);
//
//		delete[]imgBuff;
//		imgBuff = NULL;
//		//加密
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
//		//解密
//		//FILE* filemiwen = fopen("img.data", "rb");
//		FILE* filemiwen = fopen("img_face.data", "rb");
//		if (filemiwen)
//		{
//			//打开文件
//			fseek(filemiwen, 0, SEEK_END);
//			long filelenmiwen = ftell(filemiwen);
//			char* imgBuff2 = new char[filelenmiwen + 1];
//			memset(imgBuff2, 0x00, filelenmiwen + 1);
//
//			//读取文件
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


//std::string Mat2Base64(const cv::Mat & img, std::string imgType)
//{
//	//Matתbase64
//	std::string img_data;
//	std::vector<uchar> vecImg;
//	std::vector<int> vecCompression_params;
//	vecCompression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
//	vecCompression_params.push_back(90);
//	imgType = "." + imgType;
//	cv::imencode(imgType, img, vecImg, vecCompression_params);
//	img_data = base64Encode(vecImg.data(), vecImg.size());
//	return img_data;
//}

#endif
//Mat测试
int main()
{
	int encryptLen = 128 * 1024;
	int decryptLen = 349528;
	int remainlen = 0;

	//加密测试
	stop_watch timer;

	cv::Mat matSrc = imread("img.jpg");

	if (matSrc.empty())
	{
		return -1;
	}
	else 
	{

#if 1
		//加密
		//Mat转bin
		timer.start();

		std::vector<unsigned char> vImg;

		MatToByteArray(matSrc, vImg);



		char* pimg = new char[vImg.size() + 1];
		memset(pimg,0x00, vImg.size() + 1);

		for (int i=0;i< vImg.size();i++)
		{
			pimg[i] = vImg[i];
		}

		FILE* fileMat = fopen("imgMat.jpg", "wb+");
		fwrite(pimg,vImg.size(),1,fileMat);
		fclose(fileMat);

		//
		if (vImg.size() > ENCRYPT_SIZE)
		{
			//转为base64
			stop_watch timer;
			char *pOutBuf = new char[vImg.size() * 2];
			memset(pOutBuf, 0x00, vImg.size() * 2);
			timer.start();
			base64_encode(pOutBuf, (const unsigned char*)pimg, ENCRYPT_SIZE, 0);

			//加密
			string strimg = shiftEncrypt(pOutBuf, KEYWORD);
			timer.stop();
			cout << timer.elapsed_ms();
			FILE* fileOut = fopen("img.data", "wb+");
			fwrite(strimg.c_str(), strimg.length(), 1, fileOut);
			fwrite(pimg+ ENCRYPT_SIZE, vImg.size() - ENCRYPT_SIZE, 1, fileOut);
			fclose(fileOut);
			delete[] pOutBuf;
			pOutBuf = NULL;
		}

#endif
#if 1
		//解密
		FILE* filemiwen = fopen("img.data", "rb");
		if (filemiwen)
		{
			//打开文件
			fseek(filemiwen, 0, SEEK_END);
			long filelenmiwen = ftell(filemiwen);
			/*if (filelenmiwen > encryptLen)
				filelenmiwen = encryptLen;*/
			remainlen = filelenmiwen - decryptLen;
			char* imgBuff2 = new char[filelenmiwen + 1];
			memset(imgBuff2, 0x00, filelenmiwen + 1);

			//读取文件
			rewind(filemiwen);
			fread(imgBuff2, filelenmiwen, 1, filemiwen);
			fclose(filemiwen);

			char* remainBuff = new char[remainlen + 1];
			memset(remainBuff, 0x00, remainlen + 1);

			memcpy(remainBuff,imgBuff2+ decryptLen, remainlen);

			stop_watch timer1;

			timer1.start();
			string strImgSrc = shiftDecrypt(imgBuff2, KEYWORD);
			unsigned char* pChar = (unsigned char*)strImgSrc.c_str();
			unsigned char *pdecBuf = new unsigned char[strImgSrc.length() + 1];
			int nimgNum = base64_decode(pdecBuf, pChar, strImgSrc.length(), 0);
			timer1.stop();
			cout << timer1.elapsed_ms();

			FILE* file = fopen("dec_test.jpg", "wb+");
			fwrite(pdecBuf, 1, nimgNum, file);
			fwrite(remainBuff, 1, remainlen, file);
			fclose(file);

			
		}
#endif
	}

	
}