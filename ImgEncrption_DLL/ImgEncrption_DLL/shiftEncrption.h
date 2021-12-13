#pragma once
#include "pch.h"
#define C1 52845
#define C2 22719

#include <iostream>

#define DLL_SHIFT_API extern "C" _declspec(dllexport)

/*********************************************************
 @brief:    移位加密
 @params:   【IN】 待加密的密文
 @params:   【IN】 密钥 
 @ret:      加密后的密文
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftEncrypt(std::string S, unsigned short Key);


/*********************************************************
 @brief:    移位加密
 @params:   【IN】 待加密密文数组首地址
 @params:   【IN】 待加密密文长度
 @params:   【IN】 加密后密文数组首地址
 @params:   【OUT】加密后密文长度
 @params:   【IN】 密钥
 @ret:		 bool
 @birth:     created by imxhcheng
*********************************************************/
DLL_SHIFT_API bool shiftEncrypt2(char* pSrc,int nSrcStringNum,char* pDst,int& nDstStringNum, unsigned short Key);


/*********************************************************
 @brief:    移位解密
 @params:   加密后的密文
 @params:   密钥
 @ret:      解密后的原数据
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API std::string shiftDecrypt(std::string S, unsigned short Key);

/*********************************************************
 @brief:    移位解密
 @params:   【IN】 密文数组首地址
 @params:   【IN】 密文长度
 @params:   【OUT】解密后字符数组首地址
 @params:   【IN】 解密后字符长度
 @params:   【IN】 密钥
 @ret:      bool
 @birth:    created by imxhcheng
*********************************************************/
DLL_SHIFT_API bool shiftDecrypt2(char* pSrc, int nSrcStringNum, char* pDst, int& nDstStringNum, unsigned short Key);
