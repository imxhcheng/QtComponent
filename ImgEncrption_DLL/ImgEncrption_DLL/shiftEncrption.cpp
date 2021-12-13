#include "pch.h"
#include "shiftEncrption.h"

/*********************************************************
 @brief:    移位加密
 @params:   【IN】 待加密的密文
 @params:   【IN】 密钥
 @ret:      加密后的密文
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftEncrypt(std::string S, unsigned short Key)
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

/*********************************************************
 @brief:    移位加密
 @params:   【IN】 待加密密文数组首地址
 @params:   【IN】 待加密密文长度
 @params:   【IN】 加密后密文数组首地址
 @params:   【OUT】加密后密文长度
 @params:   【IN】 密钥
 @ret:		 bool
 @birth:     created by imxhcheng
 @Ps:		 pDst的内存在DLL中分配，使用之后需要自己释放  delete[] pDst
*********************************************************/

DLL_SHIFT_API bool shiftEncrypt2(char* pSrc, int nSrcStringNum, char* pDst, int& nDstStringNum, unsigned short Key)
{
	if (pSrc == nullptr || pDst == nullptr)
	{
		return false;
	}

	std::string str;
	std::string Result(pSrc,pSrc+nSrcStringNum);

	unsigned int i, j = 0;
	for (i = 0; i < nSrcStringNum; i++) // 依次对字符串中各字符进行操作
	{
		Result[i] = pSrc[i] ^ (Key >> 8);
		//	Result.SetAt(i, S.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((unsigned char)Result[i] + Key)*C1 + C2;
		//	Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}

	std::string strTmp = Result; // 保存结果
	Result = "";
	for (i = 0; i < strTmp.size(); i++) // 对加密结果进行转换
	{
		j = (unsigned char)strTmp[i]; // 提取字符
		// 将字符转换为两个字母保存
		str = "12"; // 设置str长度为2
		str[0] = 65 + j / 26;//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		str[1] = 65 + j % 26;
		Result += str;
	}

	for (int i = 0; i < Result.length(); i++)
	{
		pDst[i] = Result[i];
	}

	nDstStringNum = Result.length();
	return true;
}

/*********************************************************
 @brief:    移位解密
 @params:   加密后的密文
 @params:   密钥
 @ret:      解密后的原数据
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftDecrypt(std::string S, unsigned short Key)
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

/*********************************************************
 @brief:    移位解密
 @params:   【IN】 密文数组首地址
 @params:   【IN】 密文长度
 @params:   【OUT】解密后字符数组首地址
 @params:   【IN】 解密后字符长度
 @params:   【IN】 密钥
 @ret:      bool
 @birth:    created by imxhcheng
 @Ps:		pDst的内存在DLL中分配，使用之后需要自己释放  delete[] pDst
*********************************************************/
DLL_SHIFT_API bool shiftDecrypt2(char* pSrc, int nSrcStringNum, char* pDst, int& nDstStringNum, unsigned short Key)
{

	if (pSrc == nullptr || pDst == nullptr)
	{
		return false;
	}

	std::string Result, str;
	unsigned int i, j = 0;

	Result = ""; // 清除结果
	for (i = 0; i < nSrcStringNum / 2; i++) // 将字符串两个字母一组进行处理
	{
		j = ((unsigned char)pSrc[2 * i] - 65) * 26;//相应的，解密处要改为相同的数

		j += (unsigned char)pSrc[2 * i + 1] - 65;
		str = "1"; // 设置str长度为1
		str[0] = j;
		Result += str; // 追加字符，还原字符串
	}

	std::string strTmp = Result;

	for (i = 0; i < strTmp.size(); i++) // 依次对字符串中各字符进行操作
	{
		Result[i] = (unsigned char)strTmp[i] ^ (Key >> 8); // 将密钥移位后与字符异或
		Key = ((unsigned char)strTmp[i] + Key) * C1 + C2; // 产生下一个密钥
	}

	for (int i = 0; i < Result.length(); i++)
	{
		pDst[i] = Result[i];
	}

	nDstStringNum = Result.length();
	return true;
}
