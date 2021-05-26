#include "AppDefine.h"
#include "CcovSnmpUnpack.h"


unsigned short OidDecode(unsigned char *pOidDecodeStr, const unsigned char *pSrcCodeOidStr, const unsigned short srcCodeOidStrLen)
{
	unsigned short i = 0;
	unsigned short strNo = 0;      /*解码后OID字符串中的下标*/
	unsigned short codeOidStrLen = srcCodeOidStrLen;

	unsigned char highBit = 0;
	unsigned char newCount = 0;    /*记录遍历OID编码字符串时,节点个数*/
	unsigned char oldCount = 0;

	/*值为0x00时,在解码后OID字符串允许添加‘.’;0x01时,不允许添加'.',默认值为0*/
	unsigned char dotKey = 0;

	char fir2DecodeRlt = 0;
	unsigned char OidFirNode = 0;
	unsigned char OidSndNode = 0;

	unsigned short OidDecodeStrLen = 0;
	unsigned short rtnVal = 0;

	unsigned int tempOidNodeVal = 0;

	unsigned char *pDecodeStr = pOidDecodeStr;
	const unsigned char *pSrcCodeOid = pSrcCodeOidStr;


	/*节点个数初始化*/
	newCount = 0;

	/*节点值初始化*/
	tempOidNodeVal = 0;

	/*在OID解码字符串添加‘.’的开关*/
	dotKey = 0;

	for (i = 0; i < codeOidStrLen; i++)
	{
		if (newCount == oldCount)
		{
			tempOidNodeVal = tempOidNodeVal + (pSrcCodeOid[i] & 0x7f);
			highBit = pSrcCodeOid[i] & 0x80;

			/*0x80 == pSrcCodeOid[i] & 0x80,== 级别高于 & 位运算*/
			if ((0 == highBit) || (i == (codeOidStrLen - 1)))
			{
				newCount++;
			}
			else if (0x80 == highBit)
			{
				tempOidNodeVal = tempOidNodeVal << 7;
			}
		}

		if (newCount != oldCount)
		{
			if (1 == newCount)
			{
				fir2DecodeRlt = OidFir2NodeDecode(&OidFirNode, &OidSndNode, tempOidNodeVal);
				if (1 == fir2DecodeRlt)
				{
					strNo = OidNodeValDecode(pDecodeStr, strNo, OidFirNode, dotKey);
					strNo = OidNodeValDecode(pDecodeStr, strNo, OidSndNode, dotKey);
				}
			}
			else
			{
				if (i == (codeOidStrLen - 1))
				{
					/*解码完了OID信息的最后一个字节后,不需添加节点分隔符'.'*/
					dotKey = 1;
				}
				else
				{
					dotKey = 0;
				}
				strNo = OidNodeValDecode(pDecodeStr, strNo, tempOidNodeVal, dotKey);
			}

			/*将资源重新初始化,为下一循环做准备*/
			dotKey = 0;
			tempOidNodeVal = 0;
			oldCount = newCount;
		}
	}

	/*计算解码后的OID信息总共所占的字节数*/
	OidDecodeStrLen = strNo;

	rtnVal = OidDecodeStrLen;
	return rtnVal;
}


char OidFir2NodeDecode(unsigned char *pOidFirNode, unsigned char *pOidSndNode, const unsigned int oidFir2NodeSum)
{
	char rtnVal = 0;

	unsigned char *pFirNode = pOidFirNode;
	unsigned char *pSndNode = pOidSndNode;

	if ((oidFir2NodeSum >= 0) && (oidFir2NodeSum <= 39))
	{
		*pFirNode = 0;
		*pSndNode = oidFir2NodeSum;
		rtnVal = 1;
	}
	else if ((oidFir2NodeSum >= 40) && (oidFir2NodeSum <= 79))
	{
		*pFirNode = 1;
		*pSndNode = oidFir2NodeSum - 40;
		rtnVal = 1;
	}
	else if (oidFir2NodeSum >= 80)
	{
		*pFirNode = 2;
		*pSndNode = oidFir2NodeSum - 40 * 2;
		rtnVal = 1;
	}

	return rtnVal;
}


unsigned char OidNodeDecode(unsigned char *pOidNodeStr, const unsigned int oidNodeVal)
{
	unsigned char i = 0;                           /*字符数组下标*/
	unsigned char rtlVal = 0;
	unsigned char oidNodeStrLen = 0;
	unsigned char tempStr[20] = { 0 };
	unsigned char *pTempNodeStr = pOidNodeStr;

	unsigned int rNodeVal = oidNodeVal;         /*循环处理中的oid节点值*/


	if (0 == rNodeVal)
	{
		*pTempNodeStr = rNodeVal + '0';
		oidNodeStrLen = 1;
	}
	else
	{
		while (rNodeVal > 0)
		{
			tempStr[i] = rNodeVal % 10 + '0';
			i++;
			rNodeVal /= 10;
		}

		/*记录OID节点的字符串形式的值的长度*/
		oidNodeStrLen = i;

		for (i = 0; i < oidNodeStrLen; i++)
		{
			*pTempNodeStr = tempStr[oidNodeStrLen - 1 - i];
			pTempNodeStr++;
		}
	}

	rtlVal = oidNodeStrLen;
	return rtlVal;
}


unsigned short OidNodeValDecode(unsigned char *poidDecodeStr, unsigned int decodeStrNo, const unsigned short oidNodeVal, const unsigned char dotKey)
{
	unsigned short rtlVal = 0;
	unsigned int StrNo = 0;

	unsigned char codeOidStr[8] = { 0 };
	unsigned char oidNodeLen = 0;
	unsigned char *pTempOidNode = poidDecodeStr;

	StrNo = decodeStrNo;
	oidNodeLen = OidNodeDecode(codeOidStr, oidNodeVal);
	memcpy((void *)&pTempOidNode[StrNo], codeOidStr, oidNodeLen);
	StrNo += oidNodeLen;

	rtlVal = 1;
	if (0 == dotKey)
	{
		/*在memcpy基础上,可以使用strcat*/
		memcpy((void *)&pTempOidNode[StrNo], ".", 2);
		/*为调试方便,增加‘\0’,但不计入StrNo*/
		StrNo += 1;
	}

	rtlVal = StrNo;
	return rtlVal;
}


unsigned char Uint32ToUchar(unsigned char *pUintStr, const unsigned int uintVal)
{
	/*返回字符串形式的整型数的长度值*/
	return OidNodeDecode(pUintStr, uintVal);
}


unsigned char LenDecode(unsigned short *pLength, const unsigned char *pLenCodeStr)
{
	unsigned char i = 0;                  /*循环用变量*/
	unsigned char lenTag = 0;             /*长度信息高位*/

	unsigned char len = 0;                /*长度本身占有的字节数*/

	unsigned char rtnVal = 0;             /*长度信息使用BER编码后所占字节数*/

	const unsigned char *pLenStr = pLenCodeStr;
	unsigned short *pLenVal = pLength;

	*pLenVal = 0;                 /*做初始化防护*/

	lenTag = *pLenStr & 0x80;     /*取出长度信息高位*/

	if (0 == lenTag)
	{
		*pLenVal = *pLenStr & 0x7f;
		rtnVal = 1;
	}
	else
	{
		len = *pLenStr & 0x7f;
		pLenStr++;                 /*从第二个字节开始计算长度*/

		for (i = 0; i < len; i++)
		{
			*pLenVal = *pLenVal + pLenStr[i];

			/*i == (len - 1)时,是不能进行移位运算的*/
			if (i != (len - 1))
			{
				*pLenVal = (*pLenVal) << 8;
			}
		}

		rtnVal = len + 1;
	}

	return rtnVal;
}


char IntTypeDecode(int *pInteger, const unsigned char *pIntValStr, const unsigned char intStrLen)
{
	char rtlVal = 0;
	unsigned char i = 0;                       /*循环处理中用的变量*/
	unsigned char tempStrLen = 0;
	const unsigned char *pIntStr = pIntValStr;

	char cVal = 0;
	unsigned short i16Val = 0;
	unsigned int i32Val = 0;
	int *pTempInt = pInteger;
	unsigned int tempInt = 0;

	tempStrLen = intStrLen;

	if (0 == intStrLen)
	{
		/*小标为0的字节为指示字节*/
		for (i = 0; i < tempStrLen; i++)
		{
			tempInt = tempInt | pIntStr[i];
			if (i != (tempStrLen - 1))
			{
				tempInt = tempInt << 8;
			}
		}
		rtlVal = 1;

	}
	else if (1 == tempStrLen)
	{
		/*tempInt = pIntStr[0];则得到正值*/
		cVal = pIntStr[0];
		tempInt = cVal;
		rtlVal = 1;
	}
	else if (2 == tempStrLen)
	{
		for (i = 0; i < tempStrLen; i++)
		{
			i16Val = i16Val + pIntStr[i];
			if (i != (tempStrLen - 1))
			{
				i16Val = i16Val << 8;
			}
		}
		tempInt = i16Val;
		rtlVal = 1;
	}
	else if (4 == tempStrLen)
	{
		for (i = 0; i < tempStrLen; i++)
		{
			i32Val = i32Val + pIntStr[i];
			if (i != (tempStrLen - 1))
			{
				i32Val = i32Val << 8;
			}
		}
		tempInt = i32Val;
		rtlVal = 1;
	}

	*pTempInt = tempInt;
	return 	rtlVal;
}


char UintTypeDecode(unsigned int *puInteger, const unsigned char *puIntValStr, const unsigned char uintStrLen)
{
	char rtlVal = 0;
	unsigned char i = 0;                       /*循环处理中用的变量*/
	unsigned char tempStrLen = 0;

	unsigned int *pTempUint = puInteger;
	unsigned int tempUint = 0;

	tempStrLen = uintStrLen;

	if (0 == puIntValStr[0])
	{
		for (i = 1; i < tempStrLen; i++)
		{
			tempUint = tempUint | puIntValStr[i];
			if (i != (tempStrLen - 1))
			{
				tempUint = tempUint << 8;
			}
		}
		rtlVal = 1;
	}
	else
	{
		for (i = 0; i < tempStrLen; i++)
		{
			tempUint = tempUint | puIntValStr[i];
			if (i != (tempStrLen - 1))
			{
				tempUint = tempUint << 8;
			}
		}
		rtlVal = 1;
	}

	*pTempUint = tempUint;
	return rtlVal;
}


unsigned short SeqTagDecode(SEQ_STRU *pSeqStru, unsigned short *pErrPosition, const unsigned short strNo, const unsigned char *pResponsStr)
{
	unsigned char seqDecodeStart = 1;
	unsigned short rtlVal = 0;
	unsigned short *pErrPosi = pErrPosition;
	unsigned short num = strNo;

	const unsigned char *pRespStr = pResponsStr;
	SEQ_STRU *pSequStru = pSeqStru;
	memset(pSequStru, 0, sizeof(pSequStru));

	/*此循环只运行一次,采用循环只为优化编码结构:当解码过程中出现解码错误或异常时,
	*便于结束解码操作*/
	while (1 == seqDecodeStart)
	{
		if (TSEQUENCE == pRespStr[num])
		{
			pSequStru->seqType = pRespStr[num];
		}
		else
		{
			*pErrPosi = num;
			break;
		}

		num += 1;
		pSequStru->seqLenCodeSize = LenDecode(&pSequStru->seqLen, &pRespStr[num]);

		/*Bind组件2字节最大为4,长度字节过大,则视为出现解码异常*/
		if ((pSequStru->seqLenCodeSize == 0) | (pSequStru->seqLenCodeSize >= 5))
		{
			*pErrPosi = num;
			break;
		}

		num += pSequStru->seqLenCodeSize;
		pSequStru->seqSize = 1 + pSequStru->seqLenCodeSize + pSequStru->seqLen;

		seqDecodeStart = 0;
		break;
	}

	rtlVal = num;
	return rtlVal;
}


unsigned char IpInfoDecode(unsigned char *pIpInfoStr, const unsigned char *pIpCodeStr, const unsigned char ipCodeLen)
{
	unsigned char integStrLen = 0;
	unsigned char rtnVal = 0;              /*函数返回值*/
	unsigned char i = 0;                  /*循环处理中的变量*/
	unsigned char j = 0;

	/*参数传递*/
	unsigned char ipTempLen = ipCodeLen;
	unsigned char *pTempIpStr = pIpInfoStr;
	const unsigned char *pTempIpCodeStr = pIpCodeStr;

	if ((ipTempLen >= 4) && (ipTempLen <= 16))
	{
		for (i = 0; i < ipTempLen; i++)
		{
			/*将IP节点'.'之间的值转化为字符串形式*/
			integStrLen = OidNodeDecode(&pTempIpStr[j], (unsigned int)pTempIpCodeStr[i]);
			j += integStrLen;

			if (i != (ipTempLen - 1))
			{
				pTempIpStr[j] = '.';
				j++;
			}
		}

		rtnVal = j + 1;
	}

	/*解码后ip信息占的字节数*/
	return rtnVal;
}