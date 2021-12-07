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


//int main(int argc, char* argv[])
//{
//	ifstream in;
//	in.open("Base.bmp", ios::binary);   //ios::binary是抑制底层系统服务对文件内容进行的自动转换;如果不加这个，系统会自动把'\n'转为'\r\n'; 
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
//		return 1;     //传统意义上，return 1  表示非法结束；return 0  表示正常结束； 
//	}
//
//	while (in)
//	{
//		char ch;
//
//		in.get(ch);
//		ch = ch ^ char(0xFF);    //字符的二进制码为01100000，它与11111111作异或后变为10011111；
//		if (in)   
//			out.put(ch);
//	}
//
//	in.close();
//	out.close();
//	return 0;
//}

//解密
//void main()
//{
//	ifstream in;
//	in.open("Base_tem.bmp", ios::binary);   //ios::binary是抑制底层系统服务对文件内容进行的自动转换;如果不加这个，系统会自动把'\n'转为'\r\n'; 
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
//		return ;     //传统意义上，return 1  表示非法结束；return 0  表示正常结束； 
//	}
//
//	stop_watch timer;
//
//	timer.start();
//	while (in)
//	{
//		char ch;
//		in.get(ch);
//		ch = ch ^ char(0xFF);    //字符的二进制码为01100000，它与11111111作异或后变为10011111；
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