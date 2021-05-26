#include "CcovSnmpConfig.h"
#include "CcovSnmpPack.h"


unsigned char LenCode(unsigned char *pLenCodeStr, const unsigned short length)
{
	unsigned char LenStrSize = 0;
	unsigned char OutStrSize = 0;

	/*存放length的大端形式字符串*/
	unsigned char OutLenStr[5]; 
	memset(OutLenStr, 0, sizeof(OutLenStr));

	unsigned char *pLenStr = pLenCodeStr;

	if (length <= 127)
	{
		*pLenStr = length;
		LenStrSize = 1;
	}
	else
	{
		OutStrSize = Uint16ToChar(OutLenStr, length); /*将整数数转存为字符串,大端形式*/

		/*BER编码后的字符串第一个字节为1,后7位表示长度的字节数*/
		*pLenStr = 0x80 | OutStrSize;
		pLenStr++;

		memcpy(pLenStr, OutLenStr, OutStrSize);
		LenStrSize = OutStrSize + 1;
	}

	return LenStrSize;
}


unsigned short OidCode(unsigned char *pOidCodeStr, const unsigned char *pSrcOidStr)
{
	unsigned char i = 0;           /*循环处理中的变量*/
	unsigned char j = 0;           /*OidNumStr数组标号*/
	unsigned char k = 0;           /*tempOidCode数组标号*/

	unsigned char newCount = 0;    /*记录遍历OID字符串时,经历符号‘.’的次数*/
	unsigned char oldCount = 0;

	unsigned char OidFirNode = 0;
	unsigned char OidSndNode = 0;
	unsigned char Fir2NodesSum = 0;

	/*OID字符串节点值的长度*/
	unsigned char NodeStrLen = 0;

	/*OID节点值BER编码后的字符串长度*/
	unsigned char NodeCodeStrLen = 0;

	/*OID字符串经BER编码后的字符串长度*/
	unsigned char CodeStrLen = 0;

	/*OID字符串经BER编码后的字符串长度进行BER编码后占字节数*/
	unsigned char StrLenCodeSize = 0;

	/*OID字符串经BER编码后的字符串长度+长度本身所占的字节数之和*/
	unsigned short CodeStrSize = 0;
	unsigned char srcOidSize = 0;

	const unsigned char *pSrcOid = pSrcOidStr;
	unsigned char *pOidCode = pOidCodeStr;

	/*存放OID字符串形式的节点值*/
	unsigned char OidNumStr[8] = "";

	/*临时存放编码过程中OID字符串*/
	unsigned char tempOidCode[512] = "";

	/*存放OID字符串经BER编码后的字符串的长度BER的编码*/
	unsigned char CodedOidLenStr[5] = "";

	/*临时存放OID节点值的BER编码后的字符串*/
	unsigned char tempOidNodeCode[12] = "";

	int rtlVal = 0;

	/*临时存放OID整型节点值*/
	unsigned int tempOidNodeVal = 0;

	srcOidSize = strlen((char *)pSrcOidStr);

	memset((void *)OidNumStr, 0, sizeof(OidNumStr));
	memset((void *)tempOidCode, 0, sizeof(tempOidCode));
	memset((void *)CodedOidLenStr, 0, sizeof(CodedOidLenStr));
	memset((void *)tempOidNodeCode, 0, sizeof(tempOidNodeCode));

	for (i = 0; i < srcOidSize; i++)
	{
		/*将OID字符串中两个'.'之间的内容保存到OidNumStr数组中,以便于对OID节点值进行BER编码*/
		if (newCount == oldCount)
		{
			if (pSrcOid[i] == '.')
			{
				oldCount = newCount;
				newCount += 1;
			}
			else if (pSrcOid[i] >= '0' && pSrcOid[i] <= '9')
			{
				OidNumStr[j] = pSrcOid[i];
				j++;

				if (j > 10)
				{
					break;
				}
			}
		}

		if ((newCount != oldCount) || (i == (srcOidSize - 1))) /*1 == newCount - oldCount,此时说明从OID字符串中得到节点值*/
		{
			/*进入此分支时,说明得到一个OID字符串形式的节点值,将字符串形式节点值转化为整型数后,进行BER编码*/
			NodeStrLen = strlen((char *)OidNumStr);
			if (0 != NodeStrLen)
			{
				rtlVal = OidStr2Uint(&tempOidNodeVal, OidNumStr);

				if (1 == rtlVal)
				{
					if (1 == newCount)
					{
						OidFirNode = tempOidNodeVal;
					}
					else
					{
						if (2 == newCount)
						{
							OidSndNode = tempOidNodeVal;
							/*计算OID前两个辅助节点整数值之和,使用BER进行编码*/
							Fir2NodesSum = 40 * OidFirNode + OidSndNode;

							/*更新tempOidNodeVal的值*/
							tempOidNodeVal = Fir2NodesSum;
						}

						/*OID节点值BER编码的长度*/
						NodeCodeStrLen = OidUint2Uchar(tempOidNodeCode, tempOidNodeVal);

						/*累加OID节点值BER编码的长度,最后得到OID字符串BER编码后的长度*/
						CodeStrLen += NodeCodeStrLen;

						/*追加OID节点值的BER编码到OID字符串的编码缓冲区*/
						memcpy(&tempOidCode[k], tempOidNodeCode, NodeCodeStrLen);

						/*更新每个OID节点值的BER编码在OID字符串的编码缓冲区的位置标号*/
						k += NodeCodeStrLen;
					}
				}
			}

			oldCount = newCount;
			/*为计算下一个OID节点做准备*/
			tempOidNodeVal = 0;

			memset((void *)OidNumStr, 0, sizeof(OidNumStr));
			memset((void *)tempOidNodeCode, 0, sizeof(tempOidNodeCode));
			j = 0; /*将数组及数组的标号进行复位*/
		}
	}

	/*对OID字符串经BER编码后的字符串长度进行BER编码*/
	StrLenCodeSize = LenCode(CodedOidLenStr, CodeStrLen);

	/*OID字符串经BER编码后的字符串长度+长度本身所占的字节数之和*/
	CodeStrSize = CodeStrLen + StrLenCodeSize;

	/*按照组包规则,将OID的BER编码字符串长度的BER编码字符串放在前面*/
	/*strcpy(&pOidCode[0] , CodedOidLenStr);*/
	memcpy(&pOidCode[0], CodedOidLenStr, StrLenCodeSize);

	/*将OID的BER编码字符串放在长度的BER编码信息的后面*/
	/*strcat(&pOidCode[StrLenCodeSize] , tempOidCode);*/
	memcpy(&pOidCode[StrLenCodeSize], tempOidCode, CodeStrLen);

	return CodeStrSize;
}


unsigned char OidUint2Uchar(unsigned char *pOidNumCodeStr, const unsigned int oidNum)
{
	unsigned char i = 0;
	unsigned char NumStrSize = 0; /*OID节点值使用BER编码后的的字符串所占字节数,不含'\0'*/

	unsigned char tempOidStr[10] = { 0 };
	unsigned char *pOidNumStr = pOidNumCodeStr;

	unsigned int tempNum = oidNum;
	unsigned int rNum = oidNum;

	if (tempNum <= 127)
	{
		tempOidStr[i] = tempNum & 0x7f;
		NumStrSize = 1;
	}
	else
	{
		while (rNum > 127)
		{

			if (0 == i)
			{
				tempOidStr[i] = rNum & 0x7f;
			}
			else
			{
				/*后续字节,从第2个字节开始*/
				tempOidStr[i] = 0x80 | (rNum & 0x7f);/**/
			}

			rNum = rNum >> 7;
			i++;
		}

		/*跳出while循环后,rNum通常为非0数*/
		tempOidStr[i] = 0x80 | (rNum & 0x7f);
		NumStrSize = i + 1;
	}

	/*BER编码后的的字符串的排列顺序进行恢复,以大端形式保存*/
	for (i = 0; i < NumStrSize; i++)
	{
		*pOidNumStr = tempOidStr[NumStrSize - 1 - i];
		pOidNumStr++;
	}
	*pOidNumStr = '\0';

	return NumStrSize;
}


unsigned char OidFir2NodesCode(unsigned char *pOidNodesSumStr, const unsigned int oidFirNode, const unsigned int oidSndNode)
{
	/*根据BER的编码规则,传输第一个整数X、第二个整数Y时,由公式40X+Y得到的整数表示*/
	unsigned int NodeSum = 40 * oidFirNode + oidSndNode;
	return  OidUint2Uchar(pOidNodesSumStr, NodeSum);
}


int OidStr2Uint(unsigned int *pOidNum, const unsigned char *pOidNumStr)
{
	int i = 0;
	const unsigned char *pNumStr = pOidNumStr;

	unsigned int *ptempNum = pOidNum;

	/*对输出做初始化*/
	*ptempNum = 0;

	int cmpRlt = strcmp((const char *)pNumStr, "4294967295");
	int strLen = strlen((const char *)pNumStr);

	/*此处只简单防护,关键是使用说明*/
	if (((strLen > 0) && (strLen < 10)) || ((10 == strLen) && (cmpRlt <= 0)))
	{
		for (i = 0; i < strLen; i++)
		{
			*ptempNum = *ptempNum * 10 + pNumStr[i] - '0';
		}
		return 1;
	}

	return 0;
}


unsigned char Uint16ToChar(unsigned char *pOutput, const unsigned short Input)
{
	if (Input < 256)
	{
		*pOutput = Input & 0xff;
		return 1;
	}
	else
	{
		*pOutput = (Input >> 8) & 0xff;
		*(pOutput + 1) = Input & 0xff;
		return 2;
	}

	return 0;
}


unsigned char ReqIdCode(unsigned char * pReqIdStr, const unsigned int reqId)
{
	unsigned char *ptempReqId = pReqIdStr;
	unsigned char reqIdSize = 0;
	unsigned int tempReqId = 0;

	tempReqId = reqId;

	if (tempReqId <= RAND_MAX)
	{
		if (tempReqId <= 0xff) /*1个字节*/
		{
			if (0 == (tempReqId & 0x80))
			{
				*ptempReqId = tempReqId;
				reqIdSize = 1;
			}
			else /*字节最高位为1*/
			{
				*ptempReqId = 0x00;
				*(ptempReqId + 1) = tempReqId;
				reqIdSize = 2;
			}
		}
		else /*(0xff,RAND_MAX],2个字节*/
		{
			if (0 == (tempReqId & 0x8000))
			{
				*ptempReqId = (tempReqId >> 8) & 0xff;
				*(ptempReqId + 1) = tempReqId & 0xff;
				reqIdSize = 2;
			}
			else /*字节最高位为1*/
			{
				*ptempReqId = 0x00;
				*(ptempReqId + 1) = (tempReqId >> 8) & 0xff;
				*(ptempReqId + 2) = tempReqId & 0xff;
				reqIdSize = 3;
			}
		}
	}

	return reqIdSize;
}


unsigned char u64ReqIdCode(unsigned char * pReqIdStr, const UINT64 reqId)
{
	unsigned char i = 0;
	unsigned char reqIdSize = 0;   /*Request ID经BER编码后的字符串的长度*/

	unsigned char *ptempReqId = pReqIdStr;
	unsigned char tempReqIdStr[15] = { 0 };

	UINT64 tempReqId = reqId;
	UINT64 rReqId = reqId;

	if (tempReqId <= REQID_MAX)
	{
		while (rReqId > 255)
		{
			tempReqIdStr[i] = rReqId & 0xff;/**/
			rReqId = rReqId >> 8;
			i++;
		}
		/*输入整型数,最高位为1时,添加0x00字段*/
		if (0x80 == (rReqId & 0x80))
		{
			tempReqIdStr[i] = rReqId;
			tempReqIdStr[i + 1] = 0x00;
			reqIdSize = i + 2;        /*更新长度值*/
		}
		else  /*0 == (rReqId & 0x80)*/
		{
			tempReqIdStr[i] = rReqId;
			reqIdSize = i + 1;        /*更新长度值*/
		}
	}

	/*BER编码后的的字符串的排列顺序进行恢复,以大端形式保存*/
	for (i = 0; i < reqIdSize; i++)
	{
		*ptempReqId = tempReqIdStr[reqIdSize - 1 - i];
		ptempReqId++;
	}
	*ptempReqId = '\0';

	/*返回Request ID字符串的长度*/
	return reqIdSize;
}