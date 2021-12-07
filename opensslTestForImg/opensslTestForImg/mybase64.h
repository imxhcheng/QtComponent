#ifndef __MYBASE64_H__
#define __MYBASE64_H__

#ifdef __cplusplus
extern "C" {
#endif

	/*************************************************
	Function:       base64_to_ascii
	Description:    decodes string from base64 to ascii
	Input:          const char *in: the input string (NIL-terminated)
	int inlen: length of input string
	int maxlen: the output buffer size limit, 0 to ignore
	Output:         unsigned char *out: decoded output string
	Return:         length of output string on successful
	less than 0 on error occur, then the output is invalid
	Others:
	*************************************************/
	int base64_decode(unsigned char *out, unsigned char *in, int inlen, int maxlen);	//解密

	/*************************************************
	Function:       ascii_to_base64
	Description:    encodes string from ascii to base64
	Input:          const char *in: the input string (NIL-terminated)
	int inlen: length of input string
	int maxlen: the output buffer size limit, 0 to ignore
	Output:         unsigned char *out: decoded output string
	Return:         length of output string on successful
	less than 0 on error occur, then the output is invalid
	Others:
	*************************************************/
	int base64_encode(char *out, const unsigned char *in, int inlen, int maxlen);	//加密

#ifdef __cplusplus
}
#endif



#endif
