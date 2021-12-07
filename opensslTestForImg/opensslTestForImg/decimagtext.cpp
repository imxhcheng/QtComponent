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

//��λ����
std::string shiftEncrypt(std::string S, unsigned short Key)
{
	std::string Result, str;
	unsigned int i, j = 0;

	Result = S; // ��ʼ������ַ���
	for (i = 0; i < S.size(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result[i] = S[i] ^ (Key >> 8);
		//	Result.SetAt(i, S.GetAt(i)^(Key>>8)); // ����Կ��λ�����ַ����
		Key = ((unsigned char)Result[i] + Key)*C1 + C2;
		//	Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // ������һ����Կ
	}
	S = Result; // ������
	Result = "";
	//Result.Empty(); // ������
	for (i = 0; i < S.size(); i++) // �Լ��ܽ������ת��
	{
		j = (unsigned char)S[i]; // ��ȡ�ַ�
		// ���ַ�ת��Ϊ������ĸ����
		str = "12"; // ����str����Ϊ2
		str[0] = 65 + j / 26;//���ｫ65�Ĵ���������256�����ľͻ�����룬Ч�����ã���Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����
		str[1] = 65 + j % 26;
		Result += str;
	}
	return Result;
}

//��λ����
std::string shiftDecrypt(std::string S, unsigned short Key)
{
	std::string Result, str;
	unsigned int i, j = 0;

	Result = ""; // ������
	for (i = 0; i < S.size() / 2; i++) // ���ַ���������ĸһ����д���
	{
		j = ((unsigned char)S[2 * i] - 65) * 26;//��Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����

		j += (unsigned char)S[2 * i + 1] - 65;
		str = "1"; // ����str����Ϊ1
		str[0] = j;
		Result += str; // ׷���ַ�����ԭ�ַ���
	}
	S = Result; // �����м���
	for (i = 0; i < S.size(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result[i] = (unsigned char)S[i] ^ (Key >> 8); // ����Կ��λ�����ַ����
		Key = ((unsigned char)S[i] + Key) * C1 + C2; // ������һ����Կ
	}
	return Result;
}

int main()
{
	//���ܲ���
	FILE* file = fopen("test.png","rb");

	if (file != NULL)
	{

#if 0
		//����
		//���ļ�
		fseek(file,0,SEEK_END);
		long filelen = ftell(file);
		byte* imgBuff = new byte[filelen + 1];
		memset(imgBuff, 0x00, filelen + 1);

		//��ȡ�ļ�
		rewind(file);
		fread(imgBuff,filelen,1,file);
		fclose(file);

		//תΪbase64
		stop_watch timer;
		char *pOutBuf = new char[filelen * 2];
		memset(pOutBuf, 0x00, filelen * 2);
		timer.start();
		base64_encode(pOutBuf, imgBuff, filelen, 0);

		delete[]imgBuff;
		imgBuff = NULL;
		//����
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
		//����
		FILE* filemiwen = fopen("img.data", "rb");
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