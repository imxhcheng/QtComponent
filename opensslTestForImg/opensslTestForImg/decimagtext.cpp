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

using namespace std;
using namespace cv;

#define C1 52845
#define C2 22719
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

//移位加密
std::string shiftEncrypt(std::string S, unsigned short Key)
{
	std::string Result, str;
	unsigned int i, j = 0;

	Result = S; // 初始化结果字符串
	for (i = 0; i < S.size(); i++) // 依次对字符串中各字符进行操作
	{
		Result[i] = S[i] ^ (Key >> 8);
		//	Result.SetAt(i, S.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((unsigned char)Result[i] + Key)*C1 + C2;
		//	Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	S = Result; // 保存结果
	Result = "";
	//Result.Empty(); // 清除结果
	for (i = 0; i < S.size(); i++) // 对加密结果进行转换
	{
		j = (unsigned char)S[i]; // 提取字符
		// 将字符转换为两个字母保存
		str = "12"; // 设置str长度为2
		str[0] = 65 + j / 26;//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		str[1] = 65 + j % 26;
		Result += str;
	}
	return Result;
}

//移位解密
std::string shiftDecrypt(std::string S, unsigned short Key)
{
	std::string Result, str;
	unsigned int i, j = 0;

	Result = ""; // 清除结果
	for (i = 0; i < S.size() / 2; i++) // 将字符串两个字母一组进行处理
	{
		j = ((unsigned char)S[2 * i] - 65) * 26;//相应的，解密处要改为相同的数

		j += (unsigned char)S[2 * i + 1] - 65;
		str = "1"; // 设置str长度为1
		str[0] = j;
		Result += str; // 追加字符，还原字符串
	}
	S = Result; // 保存中间结果
	for (i = 0; i < S.size(); i++) // 依次对字符串中各字符进行操作
	{
		Result[i] = (unsigned char)S[i] ^ (Key >> 8); // 将密钥移位后与字符异或
		Key = ((unsigned char)S[i] + Key) * C1 + C2; // 产生下一个密钥
	}
	return Result;
}

int main()
{
	//加密测试
	FILE* file = fopen("test.png","rb");

	if (file != NULL)
	{

#if 0
		//加密
		//打开文件
		fseek(file,0,SEEK_END);
		long filelen = ftell(file);
		byte* imgBuff = new byte[filelen + 1];
		memset(imgBuff, 0x00, filelen + 1);

		//读取文件
		rewind(file);
		fread(imgBuff,filelen,1,file);
		fclose(file);

		//转为base64
		stop_watch timer;
		char *pOutBuf = new char[filelen * 2];
		memset(pOutBuf, 0x00, filelen * 2);
		timer.start();
		base64_encode(pOutBuf, imgBuff, filelen, 0);

		delete[]imgBuff;
		imgBuff = NULL;
		//加密
		string strimg = shiftEncrypt(pOutBuf,KEYWORD);
		timer.stop();
		cout << timer.elapsed_ms();
		FILE* fileOut = fopen("img.data", "wb+");
		fwrite(strimg.c_str(), strimg.length(), 1, fileOut);
		fclose(fileOut);
		delete[] pOutBuf;
		pOutBuf = NULL;
#endif
#if 1
		//解密
		FILE* filemiwen = fopen("img.data", "rb");
		if (filemiwen)
		{
			//打开文件
			fseek(filemiwen, 0, SEEK_END);
			long filelenmiwen = ftell(filemiwen);
			char* imgBuff2 = new char[filelenmiwen + 1];
			memset(imgBuff2, 0x00, filelenmiwen + 1);

			//读取文件
			rewind(filemiwen);
			fread(imgBuff2, filelenmiwen, 1, filemiwen);
			fclose(filemiwen);

			stop_watch timer1;

			timer1.start();
			string strImgSrc = shiftDecrypt(imgBuff2, KEYWORD);
			unsigned char* pChar = (unsigned char*)strImgSrc.c_str();
			unsigned char *pdecBuf = new unsigned char[strImgSrc.length() + 1];
			int nimgNum = base64_decode(pdecBuf, pChar, strImgSrc.length(), 0);
			timer1.stop();
			cout << timer1.elapsed_ms();

			QImage im;
			im.loadFromData(pdecBuf, nimgNum);

			if (!im.isNull())
			{
				cout << "Done!";
			}
			cout << im.height();
			cout << im.width();
			
		}
#endif
	}

	waitKey(1000000);
}