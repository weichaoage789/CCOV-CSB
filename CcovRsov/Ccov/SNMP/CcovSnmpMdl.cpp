#include "AppDefine.h"
#include "CcovSnmpMdl.h"



unsigned short RequestFlameCode(unsigned char *pReqCode, const unsigned char reqType, enum bandNumber bandVarNum, const REQ_ARGV_STRU *preqArgvStru)
{
	unsigned char varNo = 0;
	unsigned char varNum = 0;

	unsigned short strNo = 0;                      /*�ַ�������±�*/

	unsigned char *ptempReqCode = pReqCode;
	unsigned short rtlVal = 0;                    /*������������֡�������ܳ���*/

	unsigned char tempFlameStr[(BANDNUM + 1) * 100] = "";

	unsigned char tempVerStr[6] = "";
	unsigned char tempCommStr[30] = "";

	unsigned char tempReqInfoStr[BANDNUM * 100 + 60] = "";

	unsigned char tempReqIdStr[10] = "";
	unsigned char tempErrStatusStr[5] = "";
	unsigned char tempErrIndexStr[5] = "";

	/*��Ű󶨱���TLV����Ϣ*/
	unsigned char tempVarBindInfoStr[BANDNUM * 100] = "";

	/*���������*/
	unsigned char bindCompStr[BANDNUM][100] = { "","","" };

	unsigned char tempOidStr[BANDNUM][80] = { "","","" };
	unsigned char tempVarStr[BANDNUM][5] = { "","","" };

	/*error Status��error Index*/
	unsigned char ErrStatusStrSize = 0;
	unsigned char ErrIndexStrSize = 0;

	/*֡�����ͼ�����*/
	unsigned char flameType = 0;
	unsigned short flameLen = 0;
	unsigned char flmLenCode[5] = "";
	unsigned char flmLenCodeSize = 0;

	unsigned short flameSize = 0;              /*��������֡�������ܳ���*/

									   /*request IDֵ*/
	unsigned int reqId = 0;

	/*����PDU���ͺ�����PDU�ĳ�����Ϣ*/
	unsigned char requestType = 0;
	unsigned short reqLen = 0;
	unsigned char reqLenCode[5] = "";
	unsigned char reqLenCodeSize = 0;

	unsigned char reqSize = 0;

	/*variable-bindings���������ͺ���Ӧ�ĳ�����Ϣ*/
	unsigned char varBindType = 0;
	unsigned short VarBindLen = 0;
	unsigned char VarBindLenCode[5] = "";
	unsigned char VarBindLenCodeSize = 0;

	unsigned char varBindSize = 0;

	/*variable-bindings��BANDNUM��������������ͺ���Ӧ�ĳ�����Ϣ*/
	unsigned char BindCompType[BANDNUM] = "";
	unsigned short BindCompLen[BANDNUM] = { 0,0,0 };
	unsigned char BindCompLenCode[BANDNUM][5] = { "","","" };
	unsigned char BindCompLenCodeSize[BANDNUM] = "";

	unsigned char BindCompSize[BANDNUM] = "";

	/*variable-bindings��BANDNUM������ı����ַ�����ռ�ֽ���*/
	unsigned char tempVarStrSize[BANDNUM] = "";

	/*�ṹ�嶨��*/
	VER_INFO_STRU verStru;
	COMM_INFO_STRU commStru;
	REQID_INFO_STRU  reqIdStru;

	/*OID��Ϣ�ṹ��*/
	OID_INFO_STRU oidStru[BANDNUM];

	const REQ_ARGV_STRU *ptempReqStru = preqArgvStru;

	if (bandVarNum >= num_Five)
	{
		bandVarNum = num_Four;
	}
	else if (bandVarNum <= num_Zero)
	{
		/*δ����ʵ�ʷ���*/
	}

	varNum = bandVarNum;

	/*�ַ������ʼ��*/
	memset(tempFlameStr, 0, sizeof(tempFlameStr));

	memset(tempVerStr, 0, sizeof(tempVerStr));
	memset(tempCommStr, 0, sizeof(tempCommStr));

	memset(tempReqInfoStr, 0, sizeof(tempReqInfoStr));
	memset((char *)tempReqIdStr, 0, sizeof(tempReqIdStr));
	memset(tempErrStatusStr, 0, sizeof(tempErrStatusStr));
	memset(tempErrIndexStr, 0, sizeof(tempErrIndexStr));

	memset(tempVarBindInfoStr, 0, sizeof(tempVarBindInfoStr));

	/*������BANDNUM������ĳ�ʼ��*/
	memset(bindCompStr, 0, sizeof(bindCompStr));
	memset(tempOidStr, 0, sizeof(tempOidStr));
	memset(tempVarStr, 0, sizeof(tempVarStr));

	memset(BindCompType, 0, sizeof(BindCompType));
	memset(BindCompLen, 0, sizeof(BindCompLen));
	memset(BindCompLenCode, 0, sizeof(BindCompLenCode));
	memset(BindCompLenCodeSize, 0, sizeof(BindCompLenCodeSize));
	memset(BindCompSize, 0, sizeof(BindCompSize));

	memset(flmLenCode, 0, sizeof(flmLenCode));

	/*�ṹ���ʼ��*/
	memset(&verStru, 0, sizeof(verStru));
	memset(&commStru, 0, sizeof(commStru));
	memset(&reqIdStru, 0, sizeof(reqIdStru));

	memset(&oidStru, 0, sizeof(oidStru));

	/*�汾��Ϣ��tlv*/
	verStru.verInfoType = 0x02;                    /*�汾��Ϣ����������*/
	verStru.verInfolen = 1;                        /*�汾��Ϣ����*/
	verStru.verInfo = ptempReqStru->version;
	verStru.verInfoSize = 3;                       /*�����汾��Ϣ��ռ���ֽ���*/

	memcpy(&tempVerStr[0], &verStru.verInfoType, 1);
	memcpy(&tempVerStr[1], &verStru.verInfolen, 1);
	memcpy(&tempVerStr[2], &verStru.verInfo, 1);

	/*Community��Ϣ��tlv*/
	commStru.commInfoType = 0x04;
	commStru.commLen = strlen((char *)ptempReqStru->communityStr);
	commStru.commLenCodeSize = LenCode(commStru.commInfoLenStr, commStru.commLen);
	memcpy(commStru.commInfoStr, ptempReqStru->communityStr, commStru.commLen);

	strNo = 0;
	memcpy(&tempCommStr[strNo], &commStru.commInfoType, 1);
	strNo += 1;
	memcpy(&tempCommStr[strNo], commStru.commInfoLenStr, commStru.commLenCodeSize);
	strNo += commStru.commLenCodeSize;
	memcpy(&tempCommStr[strNo], commStru.commInfoStr, commStru.commLen);

	/*Community��Ϣtlv�����ĳ���*/
	commStru.commInfoSize = strNo + commStru.commLen;

	/*����ID,Ϊ�����,vxWorks���������Χ:[0,0x7fff]*/
	reqId = rand();

	reqIdStru.reqIdInfoType = 0x02;
	reqIdStru.reqIdInfoLen = ReqIdCode(reqIdStru.reqIdInfoStr, reqId);
	reqIdStru.reqIdLenCodeSize = LenCode(reqIdStru.reqIdLenStr, reqIdStru.reqIdInfoLen);

	strNo = 0;
	memcpy(&tempReqIdStr[strNo], &reqIdStru.reqIdInfoType, 1);
	strNo += 1;
	memcpy(&tempReqIdStr[strNo], reqIdStru.reqIdLenStr, reqIdStru.reqIdLenCodeSize);
	strNo += reqIdStru.reqIdLenCodeSize;
	memcpy(&tempReqIdStr[strNo], reqIdStru.reqIdInfoStr, reqIdStru.reqIdInfoLen);

	reqIdStru.InfoSize = strNo + reqIdStru.reqIdInfoLen;

	/*PDU��error status��Ϣ*/
	tempErrStatusStr[0] = 0x02;
	tempErrStatusStr[1] = 0x01;
	tempErrStatusStr[2] = 0x00;

	ErrStatusStrSize = 3;

	/*PDU��error index��Ϣ*/
	tempErrIndexStr[0] = 0x02;
	tempErrIndexStr[1] = 0x01;
	tempErrIndexStr[2] = 0x00;

	ErrIndexStrSize = 3;

	for (varNo = 0; varNo < varNum; varNo++)
	{
		/*variable-bindings�ĵ�1�������OID��Ϣ��tlv*/
		oidStru[varNo].oidInfoType = 0x06;

		/*OID��Ϣ��lv����*/
		oidStru[varNo].oidInfolen = OidCode(oidStru[varNo].oidInfoStr, ptempReqStru->poid[varNo]);
		oidStru[varNo].oidInfoSize = 1 + oidStru[varNo].oidInfolen;

		strNo = 0;
		memcpy(&tempOidStr[varNo][strNo], &oidStru[varNo].oidInfoType, 1);
		strNo += 1;
		memcpy(&tempOidStr[varNo][strNo], oidStru[varNo].oidInfoStr, oidStru[varNo].oidInfolen);

		/*variable-bindings�ĵ�varNo������ı�����tlv*/
		tempVarStr[varNo][0] = 0x05;                /*nullptr����,����ֵ0x05*/
		tempVarStr[varNo][1] = 0x00;

		tempVarStrSize[varNo] = 2;

		/*variable-bindings�ĵ�varNo��������������ͺ���Ӧ�ĳ�����Ϣ*/
		BindCompType[varNo] = 0x30;
		BindCompLen[varNo] = oidStru[varNo].oidInfoSize + tempVarStrSize[varNo];
		BindCompLenCodeSize[varNo] = LenCode(BindCompLenCode[varNo], BindCompLen[varNo]);

		strNo = 0;
		memcpy(&bindCompStr[varNo][strNo], &BindCompType[varNo], 1);
		strNo += 1;
		memcpy(&bindCompStr[varNo][strNo], &BindCompLenCode[varNo], BindCompLenCodeSize[varNo]);
		strNo += BindCompLenCodeSize[varNo];

		/*���OID��tlv*/
		memcpy(&bindCompStr[varNo][strNo], &tempOidStr[varNo], oidStru[varNo].oidInfoSize);
		strNo += oidStru[varNo].oidInfoSize;

		memcpy(&bindCompStr[varNo][strNo], &tempVarStr[varNo], tempVarStrSize[varNo]);

		BindCompSize[varNo] = strNo + tempVarStrSize[varNo];
	}


	/*��ʼ����PDU��variable-bindings�������֡*/
	/*variable-bindings���������ͺ���Ӧ�ĳ�����Ϣ*/
	/*��ǰ֧��BANDNUM��������;*/
	varBindType = 0x30;

	/*����󶨱�����Ϣ���ܳ���*/
	for (varNo = 0; varNo < varNum; varNo++)
	{
		VarBindLen += BindCompSize[varNo];
	}

	VarBindLenCodeSize = LenCode(VarBindLenCode, VarBindLen);

	strNo = 0;
	memcpy(&tempVarBindInfoStr[strNo], &varBindType, 1);
	strNo += 1;
	memcpy(&tempVarBindInfoStr[strNo], VarBindLenCode, VarBindLenCodeSize);
	strNo += VarBindLenCodeSize;

	/*��ӱ�����varNum�����*/
	for (varNo = 0; varNo < varNum; varNo++)
	{
		memcpy(&tempVarBindInfoStr[strNo], &bindCompStr[varNo], BindCompSize[varNo]);
		strNo += BindCompSize[varNo];
	}

	/*varNum������ĳ��Ⱦ����ۼ�,��ʱstrNo���õ���varBindSize*/
	varBindSize = strNo;

	/*��ʼ����PDU����֡*/
	/*request������
	* 0xA0 - GetRequest
	* 0xA1 - GetNextRequest
	* 0xA3 - SetRequest
	*/
	requestType = reqType;
	reqLen = reqIdStru.InfoSize + ErrStatusStrSize + ErrIndexStrSize + varBindSize;
	reqLenCodeSize = LenCode(reqLenCode, reqLen);

	strNo = 0;
	memcpy(&tempReqInfoStr[strNo], &requestType, 1);
	strNo += 1;
	memcpy(&tempReqInfoStr[strNo], reqLenCode, reqLenCodeSize);

	/*�������ID��Ϣ*/
	strNo += reqLenCodeSize;
	memcpy(&tempReqInfoStr[strNo], tempReqIdStr, reqIdStru.InfoSize);

	/*���error Status��Ϣ*/
	strNo += reqIdStru.InfoSize;
	memcpy(&tempReqInfoStr[strNo], tempErrStatusStr, ErrStatusStrSize);

	/*���error Index��Ϣ*/
	strNo += ErrStatusStrSize;
	memcpy(&tempReqInfoStr[strNo], tempErrIndexStr, ErrIndexStrSize);

	/*��Ӱ󶨱���TLV����Ϣ*/
	strNo += ErrIndexStrSize;
	memcpy(&tempReqInfoStr[strNo], tempVarBindInfoStr, varBindSize);

	reqSize = strNo + varBindSize;

	/*��ʼ�������֡����*/
	/*֡�����ͼ�����*/
	flameType = 0x30;
	flameLen = verStru.verInfoSize + commStru.commInfoSize + reqSize;
	flmLenCodeSize = LenCode(flmLenCode, flameLen);

	strNo = 0;
	memcpy(&tempFlameStr[strNo], &flameType, 1);
	strNo += 1;
	memcpy(&tempFlameStr[strNo], flmLenCode, flmLenCodeSize);

	/*��Ӱ汾��Ϣ��tlv*/
	strNo += flmLenCodeSize;
	memcpy(&tempFlameStr[strNo], tempVerStr, verStru.verInfoSize);

	/*���Community��Ϣ��tlv*/
	strNo += verStru.verInfoSize;
	memcpy(&tempFlameStr[strNo], tempCommStr, commStru.commInfoSize);

	/*��������PDU(Э�����ݵ�Ԫ)*/
	strNo += commStru.commInfoSize;
	memcpy(&tempFlameStr[strNo], tempReqInfoStr, reqSize);

	flameSize = strNo + reqSize;

	/*��������֡��Ϣ���*/
	memcpy((void *)ptempReqCode, tempFlameStr, flameSize);

	rtlVal = flameSize;

	/*���س�����Ϣ*/
	return rtlVal;
}


unsigned short ResponseDecode(RESPONSE_INFO_NEWSTRU *pRespInfoStru, const unsigned char *pRespStr, const unsigned short recvLen)
{
	unsigned char jOut = 0;                            /*����ṹ�帳ֵʱ��ѭ������*/
	unsigned char initNo = 0;                          /*��ʼ��forѭ������*/
	unsigned char compNo = 0;                          /*�������forѭ������*/

	unsigned char STRno = 0;                           /*��������е�STR��Ϣ�����*/
	unsigned char SNRno = 0;                           /*��������е�STR��Ϣ�����*/
	unsigned char IpStrno = 0;                         /*��������е�STR��Ϣ�����*/

											   /*������Ϣ����*/
	const unsigned char *pTempRespStr = pRespStr;
	unsigned short tempRecvLen = recvLen;
	RESPONSE_INFO_NEWSTRU *pRespStru = pRespInfoStru;

	char i = 0;
	char reqIdRlt = 0;
	char variRlt[VARINUM];

	/*OID�ֶν����ĳ���ֵ,oidDecodeStrLen[i]��ʾ��i��OID�ֶ�,0=<i<BANDNUM*/
	unsigned short oidDecodeStrLen[BANDNUM];

	unsigned short rtnStrNo = 0;                        /*�������ص�strNo*/
	unsigned short strNo = 0;                           /*��Ӧ�����ַ������±�*/
	unsigned short rtnVal = 0;                          /*�����ѽ���ı��ĵĳ���*/

												/*������Ӧ����ʱ,���ֽ��������λ��*/
	unsigned short *errPosi = &(pRespInfoStru->decodeErrPos);

	unsigned char decodeStart = 1;                      /*ֵΪ1ʱ,��ʼ�������,ֵΪ0ʱ,�����������*/

												/*֡�����ͼ�����*/
	unsigned char flameType = 0;
	unsigned short flameLen = 0;
	unsigned char flmLenCodeSize = 0;

	/*��ӦPDU���ͺ���ӦPDU�ĳ�����Ϣ*/
	unsigned char responseType = 0;
	unsigned short respLen = 0;
	unsigned char respLenCodeSize = 0;

	/*request IDֵ*/
	unsigned int reqId = 0;

	/*PDU��error status��Ϣ*/
	unsigned char tempErrStatusStr[5] = "";
	unsigned char errStatusSize = 0;

	/*PDU��error index��Ϣ*/
	unsigned char tempErrIndexStr[5] = "";
	unsigned char errIndexSize = 0;

	/*variable-bindings���������ͺ���Ӧ�ĳ�����Ϣ*/
	unsigned char varBindType = 0;
	unsigned short VarBindLen = 0;
	unsigned char VarBindLenCodeSize = 0;

	unsigned short varBindSize = 0;

	unsigned char oidNum = 0;                            /*��Ӧ�����а���OID�ĸ���*/

												 /*variable-bindings��ȫ��������������ͺ���Ӧ�ĳ�����Ϣ*/
	SEQ_STRU bindCompStru[BANDNUM];

	/*variable���б�����Ϣ*/
	unsigned char variType[BANDNUM] = "";
	unsigned char variLen[BANDNUM] = "";
	unsigned char variLenSize[BANDNUM] = "";

	unsigned char variSize[BANDNUM] = "";

	/*variable������ǿ��Ϣ*/
	int variSTR[BANDNUM];

	/*variable�����������Ϣ*/
	unsigned int variSNR[BANDNUM];

	/*variable����3IP��Ϣ*/
	unsigned char variIpStr[BANDNUM][20];


	/*�ṹ�嶨��*/
	VER_INFO_STRU verStru;
	COMM_INFO_STRU commStru;
	REQID_INFO_STRU  reqIdStru;

	OID_INFO_STRU oidStru[BANDNUM];

	memset(oidDecodeStrLen, 0, sizeof(oidDecodeStrLen));

	memset(tempErrStatusStr, 0, sizeof(tempErrStatusStr));
	memset(tempErrIndexStr, 0, sizeof(tempErrIndexStr));
	memset(variIpStr, 0, sizeof(variIpStr));

	/*�ṹ���ʼ��*/
	memset(&verStru, 0, sizeof(verStru));
	memset(&commStru, 0, sizeof(commStru));
	memset(&reqIdStru, 0, sizeof(reqIdStru));

	/*seqence or sequence of�ṹ���ʼ��*/
	memset(&bindCompStru, 0, sizeof(bindCompStru));

	/*OID�ṹ����г�ʼ��*/
	memset(&oidStru, 0, sizeof(oidStru));

	memset(&variType, 0, sizeof(variType));
	memset(&variLen, 0, sizeof(variLen));
	memset(&variLenSize, 0, sizeof(variLenSize));
	memset(&variSize, 0, sizeof(variSize));

	/*�Է��ַ����鼰��ά�ַ�������г�ʼ��*/
	for (i = 0; i< VARINUM; i++)
	{
		variRlt[i] = 0;
	}

	for (i = 0; i< BANDNUM; i++)
	{
		memset(variIpStr[i], 0, sizeof(variIpStr[i]));
		oidDecodeStrLen[i] = 0;

		variSTR[i] = 0;
		variSNR[i] = 0;
	}

	strNo = 0;

	/*��ѭ��ֻ����һ��,����ѭ��ֻΪ�Ż�����ṹ:����������г��ֽ��������쳣ʱ,
	*���ڽ����������*/
	while (1 == decodeStart)
	{
		if (pTempRespStr[strNo] == 0x30)
		{
			flameType = pTempRespStr[strNo];
			strNo += 1;
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		/*����õ�֡������Ϣ*/
		flmLenCodeSize = LenDecode(&flameLen, &pTempRespStr[strNo]);

		if ((flmLenCodeSize == 0) || (flmLenCodeSize >= 5))
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}
		else
		{
			/*do nothing*/
		}

		pRespStru->flameSize = 1 + flmLenCodeSize + flameLen;

		strNo += flmLenCodeSize;

		if (0x02 == pTempRespStr[strNo])
		{
			verStru.verInfoType = pTempRespStr[strNo];
			strNo += 1;
		}
		else
		{
			*errPosi = strNo;
			/*��������whileѭ��*/
			break;
		}

		if (0x01 == pTempRespStr[strNo])
		{
			verStru.verInfolen = pTempRespStr[strNo];
			strNo += 1;
			verStru.verInfo = pTempRespStr[strNo];
			strNo += 1;
			verStru.verInfoSize = 3;
		}
		else
		{
			*errPosi = strNo;
			/*��������whileѭ��*/
			break;
		}

		commStru.commInfoType = pTempRespStr[strNo];
		strNo += 1;
		commStru.commLenCodeSize = LenDecode((unsigned short *)&commStru.commLen, &pTempRespStr[strNo]);

		/*community���ȹ���,����Ϊ���ֽ����쳣 or community�ṹ�岻����*/
		if ((commStru.commLen == 0) || (commStru.commLen >= 20))
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += commStru.commLenCodeSize;

		/*��ȡcommunity��Ϣ*/
		memcpy(commStru.commInfoStr, &pTempRespStr[strNo], commStru.commLen);
		strNo += commStru.commLen;

		commStru.commInfoSize = 1 + commStru.commLenCodeSize + commStru.commLen;

		/*��ȡ��ӦPDU���ͺͳ�����Ϣ*/
		if (0xa2 == pTempRespStr[strNo])
		{
			responseType = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += 1;
		respLenCodeSize = LenDecode(&respLen, &pTempRespStr[strNo]);

		/*�����ֽڹ���,����Ϊ���ֽ����쳣*/
		if ((respLenCodeSize == 0) || (respLenCodeSize >= 3))
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		pRespStru->respSize = 1 + respLen;
		strNo += respLenCodeSize;

		if (0x02 == pTempRespStr[strNo])
		{
			reqIdStru.reqIdInfoType = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += 1;

		reqIdStru.reqIdLenCodeSize = LenDecode((unsigned short *)&reqIdStru.reqIdInfoLen, &pTempRespStr[strNo]);
		/*����ID�ֽ����Ϊ4,�����ֽڹ���,����Ϊ���ֽ����쳣*/
		if ((reqIdStru.reqIdLenCodeSize == 0) || (reqIdStru.reqIdLenCodeSize >= 5))
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += reqIdStru.reqIdLenCodeSize;
		reqIdStru.InfoSize = 1 + reqIdStru.reqIdLenCodeSize + reqIdStru.reqIdInfoLen;

		reqIdRlt = UintTypeDecode(&reqId, &pTempRespStr[strNo], reqIdStru.reqIdInfoLen);

		if (1 != reqIdRlt)
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}
		strNo += reqIdStru.reqIdInfoLen;

		/*��ȡerror status��Ϣ*/
		if (0x02 == pTempRespStr[strNo])
		{
			tempErrStatusStr[0] = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += 1;

		if (0x01 == pTempRespStr[strNo])
		{
			tempErrStatusStr[1] = pTempRespStr[strNo];
			strNo += 1;
			tempErrStatusStr[2] = pTempRespStr[strNo];
			strNo += 1;
			errStatusSize = 3;
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		/*��ȡerror status��Ϣ*/
		if (0x02 == pTempRespStr[strNo])
		{
			tempErrIndexStr[0] = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += 1;
		if (0x01 == pTempRespStr[strNo])
		{
			tempErrIndexStr[1] = pTempRespStr[strNo];
			strNo += 1;
			tempErrIndexStr[2] = pTempRespStr[strNo];
			strNo += 1;
			errIndexSize = 3;
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		/*����Error Status��Ϣ��Ϊ0ʱ,���ټ�������*/
		if ((tempErrStatusStr[2] > 0) && (tempErrStatusStr[2] <= 5))
		{
			/*��ʾ�����ĳ���*/
			*errPosi = 0xffff;

			/*��������whileѭ��*/
			break;
		}
		else if (tempErrStatusStr[2] > 5)
		{
			/*��ʾ��Ӧ���Ľ������,���ܳ��ִ���ų���5�����*/
			*errPosi = 0x7fff;

			/*��������whileѭ��*/
			break;
		}

		/*variable-bindings����Ϣ*/
		if (0x30 == pTempRespStr[strNo])
		{
			varBindType = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}
		strNo += 1;

		VarBindLenCodeSize = LenDecode(&VarBindLen, &pTempRespStr[strNo]);

		/*VarBind�ֽ����Ϊ4,�����ֽڹ���,����Ϊ���ֽ����쳣*/
		if ((VarBindLenCodeSize == 0) || (VarBindLenCodeSize >= 5))
		{
			*errPosi = strNo;

			/*��������whileѭ��*/
			break;
		}

		strNo += VarBindLenCodeSize;
		varBindSize = 1 + VarBindLenCodeSize + VarBindLen;

		pRespStru->VarBindSize = varBindSize;

#if 1	
		for (compNo = 0; (compNo<BANDNUM) && (strNo<tempRecvLen) && (initNo<VARINUM); compNo++)
		{
			/*variable-bindings�ĵ�compNo��������������ͺ���Ӧ�ĳ�����Ϣ*/
			rtnStrNo = SeqTagDecode(&bindCompStru[compNo], errPosi, strNo, pTempRespStr);

			/*errPosi��ʼֵΪ0,��ʾû�з���������쳣*/
			if (0 == *errPosi)
			{
				/*����strNo*/
				strNo = rtnStrNo;
			}
			else
			{
				/*����forѭ��*/
				break;
			}

			pRespStru->BindCompSize[compNo] = bindCompStru[compNo].seqSize;

			/*Bind��compNo�������OID��Ϣ*/
			if (0x06 == pTempRespStr[strNo])
			{
				oidStru[compNo].oidInfoType = pTempRespStr[strNo];
			}
			else
			{
				*errPosi = strNo;

				/*����forѭ��*/
				break;
			}

			strNo += 1;

			oidStru[compNo].oidInfolenSize = LenDecode(&oidStru[compNo].oidInfolen, &pTempRespStr[strNo]);

			/*��compNo��Bind�����OID��Ϣ�ֽ����ֵ����100ʱ,����Ϊ���ֽ����쳣*/
			if ((oidStru[compNo].oidInfolen == 0) || (oidStru[compNo].oidInfolen >= 100))
			{
				*errPosi = strNo;

				/*����forѭ��*/
				break;
			}

			strNo += oidStru[compNo].oidInfolenSize;

			/*OID�ֶν���,�õ�OID�ֶν����ĳ���ֵ*/
			oidDecodeStrLen[compNo] = OidDecode(oidStru[compNo].oidInfoStr, &pTempRespStr[strNo], oidStru[compNo].oidInfolen);

			/*������OID�ֶγ���ֵ��Ϊ0*/
			if (0 == oidDecodeStrLen[compNo])
			{
				*errPosi = strNo;

				/*����forѭ��*/
				break;
			}

			oidStru[compNo].oidInfoSize = 1 + oidStru[compNo].oidInfolenSize + oidStru[compNo].oidInfolen;
			strNo += oidStru[compNo].oidInfolen;

			switch (pTempRespStr[strNo])
			{
			case TINTEGER:
			{
				/*��compNo��Bind����ĵı�����Ϣip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);

				/*��compNo��Bind����ı����ֽ������Ϊ4,�����ֽڹ���,����Ϊ���ֽ����쳣,δ��ip����6λ�����*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 5))
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}

				strNo += variLenSize[compNo];

				/*variRlt����˽�����,0-��������,1-�ɹ�����*/
				variRlt[initNo] = IntTypeDecode(&variSTR[STRno], &pTempRespStr[strNo], variLen[compNo]);

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*��������ṹ����Ϣ*/
				pRespStru->intTypeStru[STRno].intVal = variSTR[STRno];
				pRespStru->intTypeStru[STRno].oidNo = compNo;

				/*׼����¼��һ��STR��Ϣ*/
				STRno += 1;
				initNo += 1;

				/*����switch*/
				break;
			}

			case TGauge:
			{
				/*��compNo��Bind����ĵı�����Ϣip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);

				/*��compNo��Bind����ı����ֽ������Ϊ4,�����ֽڹ���,����Ϊ���ֽ����쳣,δ��ip����6λ�����*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 5))
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLenSize[compNo];

				/*variRlt����˽�����,0-��������,1-�ɹ�����*/
				variRlt[initNo] = UintTypeDecode(&variSNR[SNRno], &pTempRespStr[strNo], variLen[compNo]);

#ifdef SNMP_ResponseDecode_DEBUG
				logMsg("variRlt:0-error,1-normal\n", 0, 0, 0, 0, 0, 0);
				logMsg("variRlt:%d,variSNR[%d]:%d\n", variRlt[initNo], SNRno, variSNR[SNRno], 0, 0, 0);
#endif

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*��������ṹ����Ϣ*/
				pRespStru->uintTypeStru[SNRno].uintVal = variSNR[SNRno];
				pRespStru->uintTypeStru[SNRno].oidNo = compNo;

				/*׼����¼��һ��SNR��Ϣ*/
				SNRno += 1;
				initNo += 1;

				/*����switch*/
				break;
			}

			case TCOTET_STRING:
			{
				/*��compNo��Bind����ĵı�����Ϣip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);
				/*��compNo��Bind����ı����ֽ������Ϊ6,�����ֽڹ���,����Ϊ���ֽ����쳣,δ��ip����6λ�����*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 16))
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLenSize[compNo];

				/*variRlt����˽����ip��Ϣռ���ֽ���*/
				/*variRlt[initNo] = IpInfoDecode( variIpStr[IpStrno] , &pTempRespStr[strNo] , variLen[compNo] );  */
				memcpy(variIpStr[IpStrno], &pTempRespStr[strNo], variLen[compNo]);
				variRlt[initNo] = variLen[compNo];

#ifdef SNMP_ResponseDecode_DEBUG
				logMsg("variLen:%d,variIpStr[%d]:%s\n", variRlt[initNo], IpStrno, (int)variIpStr[IpStrno], 0, 0, 0);
#endif

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*��������ṹ����Ϣ*/
				pRespStru->strTypeStru[IpStrno].oidNo = compNo;
				pRespStru->strTypeStru[IpStrno].strLen = variRlt[initNo];
				memcpy(pRespStru->strTypeStru[IpStrno].str, variIpStr[IpStrno], variRlt[initNo]);

				/*׼����¼��һ��IP��Ϣ*/
				IpStrno += 1;
				initNo += 1;

				/*����switch*/
				break;
			}
			case TIpAddress:
			{
				/*��compNo��Bind����ĵı�����Ϣip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);
				/*��compNo��Bind����ı����ֽ������Ϊ6,�����ֽڹ���,����Ϊ���ֽ����쳣,δ��ip����6λ�����*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 7))
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLenSize[compNo];

				/*variRlt����˽����ip��Ϣռ���ֽ���*/
				variRlt[initNo] = IpInfoDecode(variIpStr[IpStrno], &pTempRespStr[strNo], variLen[compNo]);

#ifdef SNMP_ResponseDecode_DEBUG
				logMsg("variLen:%d,variIpStr[%d]:%s\n", variRlt[initNo], IpStrno, (int)variIpStr[IpStrno], 0, 0, 0);
#endif

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*����switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*��������ṹ����Ϣ*/
				pRespStru->strTypeStru[IpStrno].oidNo = compNo;
				pRespStru->strTypeStru[IpStrno].strLen = variRlt[initNo];
				memcpy(pRespStru->strTypeStru[IpStrno].str, variIpStr[IpStrno], variRlt[initNo]);

				/*׼����¼��һ��IP��Ϣ*/
				IpStrno += 1;
				initNo += 1;

				/*����switch*/
				break;
			}
			default:
			{
				*errPosi = strNo;

				/*��¼δ֪����������*/
				pRespStru->errVariType = pTempRespStr[strNo];

				/*����switch*/
				break;
			}

			}

			/*errPosi��ʼֵΪ0,��ʾû�з���������쳣,������strNoӦ�õ���tempRecvLen*/
			if (0 != *errPosi)
			{
				/*��������whileѭ��*/
				break;
			}
			else
			{
				/*do nothing*/
			}
		}

		/*��������ṹ����Ϣ*/
		oidNum = compNo;
		pRespStru->oidNum = oidNum;

		pRespStru->intStruSize = STRno;
		pRespStru->uintStruSize = SNRno;
		pRespStru->strStruSize = IpStrno;

		for (jOut = 0; jOut<pRespStru->oidNum; jOut++)
		{
			pRespStru->oidStrLen[jOut] = oidDecodeStrLen[jOut];
			strcpy((char *)pRespStru->oidStr[jOut], (char *)oidStru[jOut].oidInfoStr);
		}


		/*errPosi��ʼֵΪ0,��ʾû�з���������쳣*/
		if (0 != *errPosi)
		{
			/*��������whileѭ��*/
			break;
		}
		else
		{
			/*do nothing*/
		}

#endif        
		pRespStru->version = verStru.verInfo;
		memcpy(pRespStru->communityStr, commStru.commInfoStr, commStru.commLen);
		pRespStru->cmdType = responseType;

		pRespStru->reqId = reqId;
		pRespStru->errStatus = tempErrStatusStr[2];
		pRespStru->errIndex = tempErrIndexStr[2];


		/*�������ݵĸ�ֵ���ڴ���ִ�й��������:
		pRespStru->flameSize;
		pRespStru->respSize;
		pRespStru->VarBindSize;

		pRespStru->BindCompSize[BANDNUM];
		pRespStru->decodeErrPos;
		**/

		decodeStart = 0;
		break;
	}

	rtnVal = strNo;

	/*�����ѽ���ı��ĵĳ���*/
	return rtnVal;
}