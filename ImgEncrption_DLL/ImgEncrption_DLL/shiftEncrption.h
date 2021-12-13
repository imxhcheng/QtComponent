#pragma once
#include "pch.h"
#define C1 52845
#define C2 22719

#include <iostream>

#define DLL_SHIFT_API extern "C" _declspec(dllexport)

/*********************************************************
 @brief:    ��λ����
 @params:   ��IN�� �����ܵ�����
 @params:   ��IN�� ��Կ 
 @ret:      ���ܺ������
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftEncrypt(std::string S, unsigned short Key);


/*********************************************************
 @brief:    ��λ����
 @params:   ��IN�� ���������������׵�ַ
 @params:   ��IN�� ���������ĳ���
 @params:   ��IN�� ���ܺ����������׵�ַ
 @params:   ��OUT�����ܺ����ĳ���
 @params:   ��IN�� ��Կ
 @ret:		 bool
 @birth:     created by imxhcheng
*********************************************************/
DLL_SHIFT_API bool shiftEncrypt2(char* pSrc,int nSrcStringNum,char* pDst,int& nDstStringNum, unsigned short Key);


/*********************************************************
 @brief:    ��λ����
 @params:   ���ܺ������
 @params:   ��Կ
 @ret:      ���ܺ��ԭ����
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftDecrypt(std::string S, unsigned short Key);

/*********************************************************
 @brief:    ��λ����
 @params:   ��IN�� ���������׵�ַ
 @params:   ��IN�� ���ĳ���
 @params:   ��OUT�����ܺ��ַ������׵�ַ
 @params:   ��IN�� ���ܺ��ַ�����
 @params:   ��IN�� ��Կ
 @ret:      bool
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API bool shiftDecrypt2(char* pSrc, int nSrcStringNum, char* pDst, int& nDstStringNum, unsigned short Key);
