#include "CcovSnmpConfig.h"
#include "CcovSnmpPack.h"


unsigned char LenCode(unsigned char *pLenCodeStr, const unsigned short length)
{
	unsigned char LenStrSize = 0;
	unsigned char OutStrSize = 0;

	/*���length�Ĵ����ʽ�ַ���*/
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
		OutStrSize = Uint16ToChar(OutLenStr, length); /*��������ת��Ϊ�ַ���,�����ʽ*/

		/*BER�������ַ�����һ���ֽ�Ϊ1,��7λ��ʾ���ȵ��ֽ���*/
		*pLenStr = 0x80 | OutStrSize;
		pLenStr++;

		memcpy(pLenStr, OutLenStr, OutStrSize);
		LenStrSize = OutStrSize + 1;
	}

	return LenStrSize;
}


unsigned short OidCode(unsigned char *pOidCodeStr, const unsigned char *pSrcOidStr)
{
	unsigned char i = 0;           /*ѭ�������еı���*/
	unsigned char j = 0;           /*OidNumStr������*/
	unsigned char k = 0;           /*tempOidCode������*/

	unsigned char newCount = 0;    /*��¼����OID�ַ���ʱ,�������š�.���Ĵ���*/
	unsigned char oldCount = 0;

	unsigned char OidFirNode = 0;
	unsigned char OidSndNode = 0;
	unsigned char Fir2NodesSum = 0;

	/*OID�ַ����ڵ�ֵ�ĳ���*/
	unsigned char NodeStrLen = 0;

	/*OID�ڵ�ֵBER�������ַ�������*/
	unsigned char NodeCodeStrLen = 0;

	/*OID�ַ�����BER�������ַ�������*/
	unsigned char CodeStrLen = 0;

	/*OID�ַ�����BER�������ַ������Ƚ���BER�����ռ�ֽ���*/
	unsigned char StrLenCodeSize = 0;

	/*OID�ַ�����BER�������ַ�������+���ȱ�����ռ���ֽ���֮��*/
	unsigned short CodeStrSize = 0;
	unsigned char srcOidSize = 0;

	const unsigned char *pSrcOid = pSrcOidStr;
	unsigned char *pOidCode = pOidCodeStr;

	/*���OID�ַ�����ʽ�Ľڵ�ֵ*/
	unsigned char OidNumStr[8] = "";

	/*��ʱ��ű��������OID�ַ���*/
	unsigned char tempOidCode[512] = "";

	/*���OID�ַ�����BER�������ַ����ĳ���BER�ı���*/
	unsigned char CodedOidLenStr[5] = "";

	/*��ʱ���OID�ڵ�ֵ��BER�������ַ���*/
	unsigned char tempOidNodeCode[12] = "";

	int rtlVal = 0;

	/*��ʱ���OID���ͽڵ�ֵ*/
	unsigned int tempOidNodeVal = 0;

	srcOidSize = strlen((char *)pSrcOidStr);

	memset((void *)OidNumStr, 0, sizeof(OidNumStr));
	memset((void *)tempOidCode, 0, sizeof(tempOidCode));
	memset((void *)CodedOidLenStr, 0, sizeof(CodedOidLenStr));
	memset((void *)tempOidNodeCode, 0, sizeof(tempOidNodeCode));

	for (i = 0; i < srcOidSize; i++)
	{
		/*��OID�ַ���������'.'֮������ݱ��浽OidNumStr������,�Ա��ڶ�OID�ڵ�ֵ����BER����*/
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

		if ((newCount != oldCount) || (i == (srcOidSize - 1))) /*1 == newCount - oldCount,��ʱ˵����OID�ַ����еõ��ڵ�ֵ*/
		{
			/*����˷�֧ʱ,˵���õ�һ��OID�ַ�����ʽ�Ľڵ�ֵ,���ַ�����ʽ�ڵ�ֵת��Ϊ��������,����BER����*/
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
							/*����OIDǰ���������ڵ�����ֵ֮��,ʹ��BER���б���*/
							Fir2NodesSum = 40 * OidFirNode + OidSndNode;

							/*����tempOidNodeVal��ֵ*/
							tempOidNodeVal = Fir2NodesSum;
						}

						/*OID�ڵ�ֵBER����ĳ���*/
						NodeCodeStrLen = OidUint2Uchar(tempOidNodeCode, tempOidNodeVal);

						/*�ۼ�OID�ڵ�ֵBER����ĳ���,���õ�OID�ַ���BER�����ĳ���*/
						CodeStrLen += NodeCodeStrLen;

						/*׷��OID�ڵ�ֵ��BER���뵽OID�ַ����ı��뻺����*/
						memcpy(&tempOidCode[k], tempOidNodeCode, NodeCodeStrLen);

						/*����ÿ��OID�ڵ�ֵ��BER������OID�ַ����ı��뻺������λ�ñ��*/
						k += NodeCodeStrLen;
					}
				}
			}

			oldCount = newCount;
			/*Ϊ������һ��OID�ڵ���׼��*/
			tempOidNodeVal = 0;

			memset((void *)OidNumStr, 0, sizeof(OidNumStr));
			memset((void *)tempOidNodeCode, 0, sizeof(tempOidNodeCode));
			j = 0; /*�����鼰����ı�Ž��и�λ*/
		}
	}

	/*��OID�ַ�����BER�������ַ������Ƚ���BER����*/
	StrLenCodeSize = LenCode(CodedOidLenStr, CodeStrLen);

	/*OID�ַ�����BER�������ַ�������+���ȱ�����ռ���ֽ���֮��*/
	CodeStrSize = CodeStrLen + StrLenCodeSize;

	/*�����������,��OID��BER�����ַ������ȵ�BER�����ַ�������ǰ��*/
	/*strcpy(&pOidCode[0] , CodedOidLenStr);*/
	memcpy(&pOidCode[0], CodedOidLenStr, StrLenCodeSize);

	/*��OID��BER�����ַ������ڳ��ȵ�BER������Ϣ�ĺ���*/
	/*strcat(&pOidCode[StrLenCodeSize] , tempOidCode);*/
	memcpy(&pOidCode[StrLenCodeSize], tempOidCode, CodeStrLen);

	return CodeStrSize;
}


unsigned char OidUint2Uchar(unsigned char *pOidNumCodeStr, const unsigned int oidNum)
{
	unsigned char i = 0;
	unsigned char NumStrSize = 0; /*OID�ڵ�ֵʹ��BER�����ĵ��ַ�����ռ�ֽ���,����'\0'*/

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
				/*�����ֽ�,�ӵ�2���ֽڿ�ʼ*/
				tempOidStr[i] = 0x80 | (rNum & 0x7f);/**/
			}

			rNum = rNum >> 7;
			i++;
		}

		/*����whileѭ����,rNumͨ��Ϊ��0��*/
		tempOidStr[i] = 0x80 | (rNum & 0x7f);
		NumStrSize = i + 1;
	}

	/*BER�����ĵ��ַ���������˳����лָ�,�Դ����ʽ����*/
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
	/*����BER�ı������,�����һ������X���ڶ�������Yʱ,�ɹ�ʽ40X+Y�õ���������ʾ*/
	unsigned int NodeSum = 40 * oidFirNode + oidSndNode;
	return  OidUint2Uchar(pOidNodesSumStr, NodeSum);
}


int OidStr2Uint(unsigned int *pOidNum, const unsigned char *pOidNumStr)
{
	int i = 0;
	const unsigned char *pNumStr = pOidNumStr;

	unsigned int *ptempNum = pOidNum;

	/*���������ʼ��*/
	*ptempNum = 0;

	int cmpRlt = strcmp((const char *)pNumStr, "4294967295");
	int strLen = strlen((const char *)pNumStr);

	/*�˴�ֻ�򵥷���,�ؼ���ʹ��˵��*/
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
		if (tempReqId <= 0xff) /*1���ֽ�*/
		{
			if (0 == (tempReqId & 0x80))
			{
				*ptempReqId = tempReqId;
				reqIdSize = 1;
			}
			else /*�ֽ����λΪ1*/
			{
				*ptempReqId = 0x00;
				*(ptempReqId + 1) = tempReqId;
				reqIdSize = 2;
			}
		}
		else /*(0xff,RAND_MAX],2���ֽ�*/
		{
			if (0 == (tempReqId & 0x8000))
			{
				*ptempReqId = (tempReqId >> 8) & 0xff;
				*(ptempReqId + 1) = tempReqId & 0xff;
				reqIdSize = 2;
			}
			else /*�ֽ����λΪ1*/
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
	unsigned char reqIdSize = 0;   /*Request ID��BER�������ַ����ĳ���*/

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
		/*����������,���λΪ1ʱ,���0x00�ֶ�*/
		if (0x80 == (rReqId & 0x80))
		{
			tempReqIdStr[i] = rReqId;
			tempReqIdStr[i + 1] = 0x00;
			reqIdSize = i + 2;        /*���³���ֵ*/
		}
		else  /*0 == (rReqId & 0x80)*/
		{
			tempReqIdStr[i] = rReqId;
			reqIdSize = i + 1;        /*���³���ֵ*/
		}
	}

	/*BER�����ĵ��ַ���������˳����лָ�,�Դ����ʽ����*/
	for (i = 0; i < reqIdSize; i++)
	{
		*ptempReqId = tempReqIdStr[reqIdSize - 1 - i];
		ptempReqId++;
	}
	*ptempReqId = '\0';

	/*����Request ID�ַ����ĳ���*/
	return reqIdSize;
}