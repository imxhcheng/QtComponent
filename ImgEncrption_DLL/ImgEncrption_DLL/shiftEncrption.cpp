#include "pch.h"
#include "shiftEncrption.h"

/*********************************************************
 @brief:    ��λ����
 @params:   ��IN�� �����ܵ�����
 @params:   ��IN�� ��Կ
 @ret:      ���ܺ������
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftEncrypt(std::string S, unsigned short Key)
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

/*********************************************************
 @brief:    ��λ����
 @params:   ��IN�� ���������������׵�ַ
 @params:   ��IN�� ���������ĳ���
 @params:   ��IN�� ���ܺ����������׵�ַ
 @params:   ��OUT�����ܺ����ĳ���
 @params:   ��IN�� ��Կ
 @ret:		 bool
 @birth:     created by imxhcheng
 @Ps:		 pDst���ڴ���DLL�з��䣬ʹ��֮����Ҫ�Լ��ͷ�  delete[] pDst
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
	for (i = 0; i < nSrcStringNum; i++) // ���ζ��ַ����и��ַ����в���
	{
		Result[i] = pSrc[i] ^ (Key >> 8);
		//	Result.SetAt(i, S.GetAt(i)^(Key>>8)); // ����Կ��λ�����ַ����
		Key = ((unsigned char)Result[i] + Key)*C1 + C2;
		//	Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // ������һ����Կ
	}

	std::string strTmp = Result; // ������
	Result = "";
	for (i = 0; i < strTmp.size(); i++) // �Լ��ܽ������ת��
	{
		j = (unsigned char)strTmp[i]; // ��ȡ�ַ�
		// ���ַ�ת��Ϊ������ĸ����
		str = "12"; // ����str����Ϊ2
		str[0] = 65 + j / 26;//���ｫ65�Ĵ���������256�����ľͻ�����룬Ч�����ã���Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����
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
 @brief:    ��λ����
 @params:   ���ܺ������
 @params:   ��Կ
 @ret:      ���ܺ��ԭ����
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftDecrypt(std::string S, unsigned short Key)
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

/*********************************************************
 @brief:    ��λ����
 @params:   ��IN�� ���������׵�ַ
 @params:   ��IN�� ���ĳ���
 @params:   ��OUT�����ܺ��ַ������׵�ַ
 @params:   ��IN�� ���ܺ��ַ�����
 @params:   ��IN�� ��Կ
 @ret:      bool
 @birth:    created by imxhcheng
 @Ps:		pDst���ڴ���DLL�з��䣬ʹ��֮����Ҫ�Լ��ͷ�  delete[] pDst
*********************************************************/
DLL_SHIFT_API bool shiftDecrypt2(char* pSrc, int nSrcStringNum, char* pDst, int& nDstStringNum, unsigned short Key)
{

	if (pSrc == nullptr || pDst == nullptr)
	{
		return false;
	}

	std::string Result, str;
	unsigned int i, j = 0;

	Result = ""; // ������
	for (i = 0; i < nSrcStringNum / 2; i++) // ���ַ���������ĸһ����д���
	{
		j = ((unsigned char)pSrc[2 * i] - 65) * 26;//��Ӧ�ģ����ܴ�Ҫ��Ϊ��ͬ����

		j += (unsigned char)pSrc[2 * i + 1] - 65;
		str = "1"; // ����str����Ϊ1
		str[0] = j;
		Result += str; // ׷���ַ�����ԭ�ַ���
	}

	std::string strTmp = Result;

	for (i = 0; i < strTmp.size(); i++) // ���ζ��ַ����и��ַ����в���
	{
		Result[i] = (unsigned char)strTmp[i] ^ (Key >> 8); // ����Կ��λ�����ַ����
		Key = ((unsigned char)strTmp[i] + Key) * C1 + C2; // ������һ����Կ
	}

	for (int i = 0; i < Result.length(); i++)
	{
		pDst[i] = Result[i];
	}

	nDstStringNum = Result.length();
	return true;
}
