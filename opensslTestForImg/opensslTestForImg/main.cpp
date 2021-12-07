#include <QtCore/QCoreApplication>
#include <openssl/aes.h>
#include <iostream>
#include <fstream>

#include <chrono>
#include <windows.h>

using namespace std;

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


//int main(int argc, char* argv[])
//{
//	ifstream in;
//	in.open("Base.bmp", ios::binary);   //ios::binary�����Ƶײ�ϵͳ������ļ����ݽ��е��Զ�ת��;������������ϵͳ���Զ���'\n'תΪ'\r\n'; 
//	if (!in)
//	{
//		cout << "Cannot open file.";
//		return 1;
//	}
//	ofstream out;
//	out.open("Base_tem.bmp", ios::binary);
//	if (!out) 
//	{
//		cout << "Cannot open file.";
//		return 1;     //��ͳ�����ϣ�return 1  ��ʾ�Ƿ�������return 0  ��ʾ���������� 
//	}
//
//	while (in)
//	{
//		char ch;
//
//		in.get(ch);
//		ch = ch ^ char(0xFF);    //�ַ��Ķ�������Ϊ01100000������11111111�������Ϊ10011111��
//		if (in)   
//			out.put(ch);
//	}
//
//	in.close();
//	out.close();
//	return 0;
//}

//����
//void main()
//{
//	ifstream in;
//	in.open("Base_tem.bmp", ios::binary);   //ios::binary�����Ƶײ�ϵͳ������ļ����ݽ��е��Զ�ת��;������������ϵͳ���Զ���'\n'תΪ'\r\n'; 
//	if (!in)
//	{
//		cout << "Cannot open file.";
//		return ;
//	}
//	ofstream out;
//	out.open("Base.bmp", ios::binary);
//	if (!out) {
//
//		cout << "Cannot open file.";
//		return ;     //��ͳ�����ϣ�return 1  ��ʾ�Ƿ�������return 0  ��ʾ���������� 
//	}
//
//	stop_watch timer;
//
//	timer.start();
//	while (in)
//	{
//		char ch;
//		in.get(ch);
//		ch = ch ^ char(0xFF);    //�ַ��Ķ�������Ϊ01100000������11111111�������Ϊ10011111��
//		if (in)   
//			out.put(ch);
//	}
//	timer.stop();
//
//	cout << timer.elapsed_ms();
//
//	in.close();
//	out.close();
//
//
//	FILE* file = fopen("test.txt","w+");
//	std::string strTest = to_string(timer.elapsed_ms());
//
//	fwrite(&strTest,sizeof(strTest),1,file);
//	return ;
//
//}