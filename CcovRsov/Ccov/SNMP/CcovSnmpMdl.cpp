#include "AppDefine.h"
#include "CcovSnmpMdl.h"



unsigned short RequestFlameCode(unsigned char *pReqCode, const unsigned char reqType, enum bandNumber bandVarNum, const REQ_ARGV_STRU *preqArgvStru)
{
	unsigned char varNo = 0;
	unsigned char varNum = 0;

	unsigned short strNo = 0;                      /*字符数组的下标*/

	unsigned char *ptempReqCode = pReqCode;
	unsigned short rtlVal = 0;                    /*返回整个请求帧编码后的总长度*/

	unsigned char tempFlameStr[(BANDNUM + 1) * 100] = "";

	unsigned char tempVerStr[6] = "";
	unsigned char tempCommStr[30] = "";

	unsigned char tempReqInfoStr[BANDNUM * 100 + 60] = "";

	unsigned char tempReqIdStr[10] = "";
	unsigned char tempErrStatusStr[5] = "";
	unsigned char tempErrIndexStr[5] = "";

	/*存放绑定变量TLV组信息*/
	unsigned char tempVarBindInfoStr[BANDNUM * 100] = "";

	/*变量绑定组件*/
	unsigned char bindCompStr[BANDNUM][100] = { "","","" };

	unsigned char tempOidStr[BANDNUM][80] = { "","","" };
	unsigned char tempVarStr[BANDNUM][5] = { "","","" };

	/*error Status和error Index*/
	unsigned char ErrStatusStrSize = 0;
	unsigned char ErrIndexStrSize = 0;

	/*帧的类型及长度*/
	unsigned char flameType = 0;
	unsigned short flameLen = 0;
	unsigned char flmLenCode[5] = "";
	unsigned char flmLenCodeSize = 0;

	unsigned short flameSize = 0;              /*整个请求帧编码后的总长度*/

									   /*request ID值*/
	unsigned int reqId = 0;

	/*请求PDU类型和请求PDU的长度信息*/
	unsigned char requestType = 0;
	unsigned short reqLen = 0;
	unsigned char reqLenCode[5] = "";
	unsigned char reqLenCodeSize = 0;

	unsigned char reqSize = 0;

	/*variable-bindings的数据类型和相应的长度信息*/
	unsigned char varBindType = 0;
	unsigned short VarBindLen = 0;
	unsigned char VarBindLenCode[5] = "";
	unsigned char VarBindLenCodeSize = 0;

	unsigned char varBindSize = 0;

	/*variable-bindings的BANDNUM个组件的数据类型和相应的长度信息*/
	unsigned char BindCompType[BANDNUM] = "";
	unsigned short BindCompLen[BANDNUM] = { 0,0,0 };
	unsigned char BindCompLenCode[BANDNUM][5] = { "","","" };
	unsigned char BindCompLenCodeSize[BANDNUM] = "";

	unsigned char BindCompSize[BANDNUM] = "";

	/*variable-bindings的BANDNUM个组件的变量字符串的占字节数*/
	unsigned char tempVarStrSize[BANDNUM] = "";

	/*结构体定义*/
	VER_INFO_STRU verStru;
	COMM_INFO_STRU commStru;
	REQID_INFO_STRU  reqIdStru;

	/*OID信息结构体*/
	OID_INFO_STRU oidStru[BANDNUM];

	const REQ_ARGV_STRU *ptempReqStru = preqArgvStru;

	if (bandVarNum >= num_Five)
	{
		bandVarNum = num_Four;
	}
	else if (bandVarNum <= num_Zero)
	{
		/*未进行实质防护*/
	}

	varNum = bandVarNum;

	/*字符数组初始化*/
	memset(tempFlameStr, 0, sizeof(tempFlameStr));

	memset(tempVerStr, 0, sizeof(tempVerStr));
	memset(tempCommStr, 0, sizeof(tempCommStr));

	memset(tempReqInfoStr, 0, sizeof(tempReqInfoStr));
	memset((char *)tempReqIdStr, 0, sizeof(tempReqIdStr));
	memset(tempErrStatusStr, 0, sizeof(tempErrStatusStr));
	memset(tempErrIndexStr, 0, sizeof(tempErrIndexStr));

	memset(tempVarBindInfoStr, 0, sizeof(tempVarBindInfoStr));

	/*变量绑定BANDNUM个组件的初始化*/
	memset(bindCompStr, 0, sizeof(bindCompStr));
	memset(tempOidStr, 0, sizeof(tempOidStr));
	memset(tempVarStr, 0, sizeof(tempVarStr));

	memset(BindCompType, 0, sizeof(BindCompType));
	memset(BindCompLen, 0, sizeof(BindCompLen));
	memset(BindCompLenCode, 0, sizeof(BindCompLenCode));
	memset(BindCompLenCodeSize, 0, sizeof(BindCompLenCodeSize));
	memset(BindCompSize, 0, sizeof(BindCompSize));

	memset(flmLenCode, 0, sizeof(flmLenCode));

	/*结构体初始化*/
	memset(&verStru, 0, sizeof(verStru));
	memset(&commStru, 0, sizeof(commStru));
	memset(&reqIdStru, 0, sizeof(reqIdStru));

	memset(&oidStru, 0, sizeof(oidStru));

	/*版本信息的tlv*/
	verStru.verInfoType = 0x02;                    /*版本信息的数据类型*/
	verStru.verInfolen = 1;                        /*版本信息长度*/
	verStru.verInfo = ptempReqStru->version;
	verStru.verInfoSize = 3;                       /*整个版本信息的占的字节数*/

	memcpy(&tempVerStr[0], &verStru.verInfoType, 1);
	memcpy(&tempVerStr[1], &verStru.verInfolen, 1);
	memcpy(&tempVerStr[2], &verStru.verInfo, 1);

	/*Community信息的tlv*/
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

	/*Community信息tlv编码后的长度*/
	commStru.commInfoSize = strNo + commStru.commLen;

	/*请求ID,为随机数,vxWorks下随机数范围:[0,0x7fff]*/
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

	/*PDU的error status信息*/
	tempErrStatusStr[0] = 0x02;
	tempErrStatusStr[1] = 0x01;
	tempErrStatusStr[2] = 0x00;

	ErrStatusStrSize = 3;

	/*PDU的error index信息*/
	tempErrIndexStr[0] = 0x02;
	tempErrIndexStr[1] = 0x01;
	tempErrIndexStr[2] = 0x00;

	ErrIndexStrSize = 3;

	for (varNo = 0; varNo < varNum; varNo++)
	{
		/*variable-bindings的第1个组件的OID信息的tlv*/
		oidStru[varNo].oidInfoType = 0x06;

		/*OID信息的lv编码*/
		oidStru[varNo].oidInfolen = OidCode(oidStru[varNo].oidInfoStr, ptempReqStru->poid[varNo]);
		oidStru[varNo].oidInfoSize = 1 + oidStru[varNo].oidInfolen;

		strNo = 0;
		memcpy(&tempOidStr[varNo][strNo], &oidStru[varNo].oidInfoType, 1);
		strNo += 1;
		memcpy(&tempOidStr[varNo][strNo], oidStru[varNo].oidInfoStr, oidStru[varNo].oidInfolen);

		/*variable-bindings的第varNo个组件的变量的tlv*/
		tempVarStr[varNo][0] = 0x05;                /*nullptr类型,编码值0x05*/
		tempVarStr[varNo][1] = 0x00;

		tempVarStrSize[varNo] = 2;

		/*variable-bindings的第varNo个组件的数据类型和相应的长度信息*/
		BindCompType[varNo] = 0x30;
		BindCompLen[varNo] = oidStru[varNo].oidInfoSize + tempVarStrSize[varNo];
		BindCompLenCodeSize[varNo] = LenCode(BindCompLenCode[varNo], BindCompLen[varNo]);

		strNo = 0;
		memcpy(&bindCompStr[varNo][strNo], &BindCompType[varNo], 1);
		strNo += 1;
		memcpy(&bindCompStr[varNo][strNo], &BindCompLenCode[varNo], BindCompLenCodeSize[varNo]);
		strNo += BindCompLenCodeSize[varNo];

		/*添加OID的tlv*/
		memcpy(&bindCompStr[varNo][strNo], &tempOidStr[varNo], oidStru[varNo].oidInfoSize);
		strNo += oidStru[varNo].oidInfoSize;

		memcpy(&bindCompStr[varNo][strNo], &tempVarStr[varNo], tempVarStrSize[varNo]);

		BindCompSize[varNo] = strNo + tempVarStrSize[varNo];
	}


	/*开始请求PDU的variable-bindings组件的组帧*/
	/*variable-bindings的数据类型和相应的长度信息*/
	/*当前支持BANDNUM个变量绑定;*/
	varBindType = 0x30;

	/*计算绑定变量信息的总长度*/
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

	/*添加变量绑定varNum个组件*/
	for (varNo = 0; varNo < varNum; varNo++)
	{
		memcpy(&tempVarBindInfoStr[strNo], &bindCompStr[varNo], BindCompSize[varNo]);
		strNo += BindCompSize[varNo];
	}

	/*varNum个组件的长度均已累加,此时strNo正好等于varBindSize*/
	varBindSize = strNo;

	/*开始请求PDU的组帧*/
	/*request的类型
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

	/*添加请求ID信息*/
	strNo += reqLenCodeSize;
	memcpy(&tempReqInfoStr[strNo], tempReqIdStr, reqIdStru.InfoSize);

	/*添加error Status信息*/
	strNo += reqIdStru.InfoSize;
	memcpy(&tempReqInfoStr[strNo], tempErrStatusStr, ErrStatusStrSize);

	/*添加error Index信息*/
	strNo += ErrStatusStrSize;
	memcpy(&tempReqInfoStr[strNo], tempErrIndexStr, ErrIndexStrSize);

	/*添加绑定变量TLV组信息*/
	strNo += ErrIndexStrSize;
	memcpy(&tempReqInfoStr[strNo], tempVarBindInfoStr, varBindSize);

	reqSize = strNo + varBindSize;

	/*开始请求的组帧操作*/
	/*帧的类型及长度*/
	flameType = 0x30;
	flameLen = verStru.verInfoSize + commStru.commInfoSize + reqSize;
	flmLenCodeSize = LenCode(flmLenCode, flameLen);

	strNo = 0;
	memcpy(&tempFlameStr[strNo], &flameType, 1);
	strNo += 1;
	memcpy(&tempFlameStr[strNo], flmLenCode, flmLenCodeSize);

	/*添加版本信息的tlv*/
	strNo += flmLenCodeSize;
	memcpy(&tempFlameStr[strNo], tempVerStr, verStru.verInfoSize);

	/*添加Community信息的tlv*/
	strNo += verStru.verInfoSize;
	memcpy(&tempFlameStr[strNo], tempCommStr, commStru.commInfoSize);

	/*添加请求的PDU(协议数据单元)*/
	strNo += commStru.commInfoSize;
	memcpy(&tempFlameStr[strNo], tempReqInfoStr, reqSize);

	flameSize = strNo + reqSize;

	/*将编码后的帧信息输出*/
	memcpy((void *)ptempReqCode, tempFlameStr, flameSize);

	rtlVal = flameSize;

	/*返回长度信息*/
	return rtlVal;
}


unsigned short ResponseDecode(RESPONSE_INFO_NEWSTRU *pRespInfoStru, const unsigned char *pRespStr, const unsigned short recvLen)
{
	unsigned char jOut = 0;                            /*输出结构体赋值时的循环变量*/
	unsigned char initNo = 0;                          /*初始化for循环变量*/
	unsigned char compNo = 0;                          /*组件解码for循环变量*/

	unsigned char STRno = 0;                           /*解码过程中的STR信息的序号*/
	unsigned char SNRno = 0;                           /*解码过程中的STR信息的序号*/
	unsigned char IpStrno = 0;                         /*解码过程中的STR信息的序号*/

											   /*参数信息传递*/
	const unsigned char *pTempRespStr = pRespStr;
	unsigned short tempRecvLen = recvLen;
	RESPONSE_INFO_NEWSTRU *pRespStru = pRespInfoStru;

	char i = 0;
	char reqIdRlt = 0;
	char variRlt[VARINUM];

	/*OID字段解码后的长度值,oidDecodeStrLen[i]表示第i个OID字段,0=<i<BANDNUM*/
	unsigned short oidDecodeStrLen[BANDNUM];

	unsigned short rtnStrNo = 0;                        /*函数返回的strNo*/
	unsigned short strNo = 0;                           /*响应报文字符串的下标*/
	unsigned short rtnVal = 0;                          /*返回已解码的报文的长度*/

												/*解析响应报文时,出现解析错误的位置*/
	unsigned short *errPosi = &(pRespInfoStru->decodeErrPos);

	unsigned char decodeStart = 1;                      /*值为1时,开始解码操作,值为0时,结束解码操作*/

												/*帧的类型及长度*/
	unsigned char flameType = 0;
	unsigned short flameLen = 0;
	unsigned char flmLenCodeSize = 0;

	/*响应PDU类型和响应PDU的长度信息*/
	unsigned char responseType = 0;
	unsigned short respLen = 0;
	unsigned char respLenCodeSize = 0;

	/*request ID值*/
	unsigned int reqId = 0;

	/*PDU的error status信息*/
	unsigned char tempErrStatusStr[5] = "";
	unsigned char errStatusSize = 0;

	/*PDU的error index信息*/
	unsigned char tempErrIndexStr[5] = "";
	unsigned char errIndexSize = 0;

	/*variable-bindings的数据类型和相应的长度信息*/
	unsigned char varBindType = 0;
	unsigned short VarBindLen = 0;
	unsigned char VarBindLenCodeSize = 0;

	unsigned short varBindSize = 0;

	unsigned char oidNum = 0;                            /*响应报文中包含OID的个数*/

												 /*variable-bindings的全部组件的数据类型和相应的长度信息*/
	SEQ_STRU bindCompStru[BANDNUM];

	/*variable所有变量信息*/
	unsigned char variType[BANDNUM] = "";
	unsigned char variLen[BANDNUM] = "";
	unsigned char variLenSize[BANDNUM] = "";

	unsigned char variSize[BANDNUM] = "";

	/*variable变量场强信息*/
	int variSTR[BANDNUM];

	/*variable变量信噪比信息*/
	unsigned int variSNR[BANDNUM];

	/*variable变量3IP信息*/
	unsigned char variIpStr[BANDNUM][20];


	/*结构体定义*/
	VER_INFO_STRU verStru;
	COMM_INFO_STRU commStru;
	REQID_INFO_STRU  reqIdStru;

	OID_INFO_STRU oidStru[BANDNUM];

	memset(oidDecodeStrLen, 0, sizeof(oidDecodeStrLen));

	memset(tempErrStatusStr, 0, sizeof(tempErrStatusStr));
	memset(tempErrIndexStr, 0, sizeof(tempErrIndexStr));
	memset(variIpStr, 0, sizeof(variIpStr));

	/*结构体初始化*/
	memset(&verStru, 0, sizeof(verStru));
	memset(&commStru, 0, sizeof(commStru));
	memset(&reqIdStru, 0, sizeof(reqIdStru));

	/*seqence or sequence of结构体初始化*/
	memset(&bindCompStru, 0, sizeof(bindCompStru));

	/*OID结构体进行初始化*/
	memset(&oidStru, 0, sizeof(oidStru));

	memset(&variType, 0, sizeof(variType));
	memset(&variLen, 0, sizeof(variLen));
	memset(&variLenSize, 0, sizeof(variLenSize));
	memset(&variSize, 0, sizeof(variSize));

	/*对非字符数组及二维字符数组进行初始化*/
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

	/*此循环只运行一次,采用循环只为优化编码结构:当解码过程中出现解码错误或异常时,
	*便于结束解码操作*/
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

			/*跳出整个while循环*/
			break;
		}

		/*解码得到帧长度信息*/
		flmLenCodeSize = LenDecode(&flameLen, &pTempRespStr[strNo]);

		if ((flmLenCodeSize == 0) || (flmLenCodeSize >= 5))
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
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
			/*跳出整个while循环*/
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
			/*跳出整个while循环*/
			break;
		}

		commStru.commInfoType = pTempRespStr[strNo];
		strNo += 1;
		commStru.commLenCodeSize = LenDecode((unsigned short *)&commStru.commLen, &pTempRespStr[strNo]);

		/*community长度过大,则视为出现解码异常 or community结构体不合理*/
		if ((commStru.commLen == 0) || (commStru.commLen >= 20))
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
			break;
		}

		strNo += commStru.commLenCodeSize;

		/*获取community信息*/
		memcpy(commStru.commInfoStr, &pTempRespStr[strNo], commStru.commLen);
		strNo += commStru.commLen;

		commStru.commInfoSize = 1 + commStru.commLenCodeSize + commStru.commLen;

		/*获取响应PDU类型和长度信息*/
		if (0xa2 == pTempRespStr[strNo])
		{
			responseType = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
			break;
		}

		strNo += 1;
		respLenCodeSize = LenDecode(&respLen, &pTempRespStr[strNo]);

		/*长度字节过大,则视为出现解码异常*/
		if ((respLenCodeSize == 0) || (respLenCodeSize >= 3))
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
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

			/*跳出整个while循环*/
			break;
		}

		strNo += 1;

		reqIdStru.reqIdLenCodeSize = LenDecode((unsigned short *)&reqIdStru.reqIdInfoLen, &pTempRespStr[strNo]);
		/*请求ID字节最大为4,长度字节过大,则视为出现解码异常*/
		if ((reqIdStru.reqIdLenCodeSize == 0) || (reqIdStru.reqIdLenCodeSize >= 5))
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
			break;
		}

		strNo += reqIdStru.reqIdLenCodeSize;
		reqIdStru.InfoSize = 1 + reqIdStru.reqIdLenCodeSize + reqIdStru.reqIdInfoLen;

		reqIdRlt = UintTypeDecode(&reqId, &pTempRespStr[strNo], reqIdStru.reqIdInfoLen);

		if (1 != reqIdRlt)
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
			break;
		}
		strNo += reqIdStru.reqIdInfoLen;

		/*获取error status信息*/
		if (0x02 == pTempRespStr[strNo])
		{
			tempErrStatusStr[0] = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
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

			/*跳出整个while循环*/
			break;
		}

		/*获取error status信息*/
		if (0x02 == pTempRespStr[strNo])
		{
			tempErrIndexStr[0] = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
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

			/*跳出整个while循环*/
			break;
		}

		/*出现Error Status信息不为0时,不再继续解码*/
		if ((tempErrStatusStr[2] > 0) && (tempErrStatusStr[2] <= 5))
		{
			/*表示请求报文出错*/
			*errPosi = 0xffff;

			/*跳出整个while循环*/
			break;
		}
		else if (tempErrStatusStr[2] > 5)
		{
			/*表示响应报文解码错误,不能出现错误号超过5的情况*/
			*errPosi = 0x7fff;

			/*跳出整个while循环*/
			break;
		}

		/*variable-bindings的信息*/
		if (0x30 == pTempRespStr[strNo])
		{
			varBindType = pTempRespStr[strNo];
		}
		else
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
			break;
		}
		strNo += 1;

		VarBindLenCodeSize = LenDecode(&VarBindLen, &pTempRespStr[strNo]);

		/*VarBind字节最大为4,长度字节过大,则视为出现解码异常*/
		if ((VarBindLenCodeSize == 0) || (VarBindLenCodeSize >= 5))
		{
			*errPosi = strNo;

			/*跳出整个while循环*/
			break;
		}

		strNo += VarBindLenCodeSize;
		varBindSize = 1 + VarBindLenCodeSize + VarBindLen;

		pRespStru->VarBindSize = varBindSize;

#if 1	
		for (compNo = 0; (compNo<BANDNUM) && (strNo<tempRecvLen) && (initNo<VARINUM); compNo++)
		{
			/*variable-bindings的第compNo个组件的数据类型和相应的长度信息*/
			rtnStrNo = SeqTagDecode(&bindCompStru[compNo], errPosi, strNo, pTempRespStr);

			/*errPosi初始值为0,表示没有发生错误或异常*/
			if (0 == *errPosi)
			{
				/*更新strNo*/
				strNo = rtnStrNo;
			}
			else
			{
				/*跳出for循环*/
				break;
			}

			pRespStru->BindCompSize[compNo] = bindCompStru[compNo].seqSize;

			/*Bind第compNo个组件的OID信息*/
			if (0x06 == pTempRespStr[strNo])
			{
				oidStru[compNo].oidInfoType = pTempRespStr[strNo];
			}
			else
			{
				*errPosi = strNo;

				/*跳出for循环*/
				break;
			}

			strNo += 1;

			oidStru[compNo].oidInfolenSize = LenDecode(&oidStru[compNo].oidInfolen, &pTempRespStr[strNo]);

			/*第compNo个Bind组件的OID信息字节最大值超过100时,则视为出现解码异常*/
			if ((oidStru[compNo].oidInfolen == 0) || (oidStru[compNo].oidInfolen >= 100))
			{
				*errPosi = strNo;

				/*跳出for循环*/
				break;
			}

			strNo += oidStru[compNo].oidInfolenSize;

			/*OID字段解码,得到OID字段解码后的长度值*/
			oidDecodeStrLen[compNo] = OidDecode(oidStru[compNo].oidInfoStr, &pTempRespStr[strNo], oidStru[compNo].oidInfolen);

			/*解码后的OID字段长度值不为0*/
			if (0 == oidDecodeStrLen[compNo])
			{
				*errPosi = strNo;

				/*跳出for循环*/
				break;
			}

			oidStru[compNo].oidInfoSize = 1 + oidStru[compNo].oidInfolenSize + oidStru[compNo].oidInfolen;
			strNo += oidStru[compNo].oidInfolen;

			switch (pTempRespStr[strNo])
			{
			case TINTEGER:
			{
				/*第compNo个Bind组件的的变量信息ip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);

				/*第compNo个Bind组件的变量字节数最大为4,长度字节过大,则视为出现解码异常,未来ip存在6位的情况*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 5))
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}

				strNo += variLenSize[compNo];

				/*variRlt存放了解码结果,0-发生错误,1-成功解码*/
				variRlt[initNo] = IntTypeDecode(&variSTR[STRno], &pTempRespStr[strNo], variLen[compNo]);

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*保存输出结构体信息*/
				pRespStru->intTypeStru[STRno].intVal = variSTR[STRno];
				pRespStru->intTypeStru[STRno].oidNo = compNo;

				/*准备记录下一个STR信息*/
				STRno += 1;
				initNo += 1;

				/*跳出switch*/
				break;
			}

			case TGauge:
			{
				/*第compNo个Bind组件的的变量信息ip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);

				/*第compNo个Bind组件的变量字节数最大为4,长度字节过大,则视为出现解码异常,未来ip存在6位的情况*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 5))
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLenSize[compNo];

				/*variRlt存放了解码结果,0-发生错误,1-成功解码*/
				variRlt[initNo] = UintTypeDecode(&variSNR[SNRno], &pTempRespStr[strNo], variLen[compNo]);

#ifdef SNMP_ResponseDecode_DEBUG
				logMsg("variRlt:0-error,1-normal\n", 0, 0, 0, 0, 0, 0);
				logMsg("variRlt:%d,variSNR[%d]:%d\n", variRlt[initNo], SNRno, variSNR[SNRno], 0, 0, 0);
#endif

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*保存输出结构体信息*/
				pRespStru->uintTypeStru[SNRno].uintVal = variSNR[SNRno];
				pRespStru->uintTypeStru[SNRno].oidNo = compNo;

				/*准备记录下一个SNR信息*/
				SNRno += 1;
				initNo += 1;

				/*跳出switch*/
				break;
			}

			case TCOTET_STRING:
			{
				/*第compNo个Bind组件的的变量信息ip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);
				/*第compNo个Bind组件的变量字节数最大为6,长度字节过大,则视为出现解码异常,未来ip存在6位的情况*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 16))
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLenSize[compNo];

				/*variRlt存放了解码后ip信息占的字节数*/
				/*variRlt[initNo] = IpInfoDecode( variIpStr[IpStrno] , &pTempRespStr[strNo] , variLen[compNo] );  */
				memcpy(variIpStr[IpStrno], &pTempRespStr[strNo], variLen[compNo]);
				variRlt[initNo] = variLen[compNo];

#ifdef SNMP_ResponseDecode_DEBUG
				logMsg("variLen:%d,variIpStr[%d]:%s\n", variRlt[initNo], IpStrno, (int)variIpStr[IpStrno], 0, 0, 0);
#endif

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*保存输出结构体信息*/
				pRespStru->strTypeStru[IpStrno].oidNo = compNo;
				pRespStru->strTypeStru[IpStrno].strLen = variRlt[initNo];
				memcpy(pRespStru->strTypeStru[IpStrno].str, variIpStr[IpStrno], variRlt[initNo]);

				/*准备记录下一个IP信息*/
				IpStrno += 1;
				initNo += 1;

				/*跳出switch*/
				break;
			}
			case TIpAddress:
			{
				/*第compNo个Bind组件的的变量信息ip*/
				variType[compNo] = pTempRespStr[strNo];
				strNo += 1;

				variLenSize[compNo] = LenDecode((unsigned short *)&variLen[compNo], &pTempRespStr[strNo]);
				/*第compNo个Bind组件的变量字节数最大为6,长度字节过大,则视为出现解码异常,未来ip存在6位的情况*/
				if ((variLenSize[compNo] == 0) || (variLenSize[compNo] >= 7))
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLenSize[compNo];

				/*variRlt存放了解码后ip信息占的字节数*/
				variRlt[initNo] = IpInfoDecode(variIpStr[IpStrno], &pTempRespStr[strNo], variLen[compNo]);

#ifdef SNMP_ResponseDecode_DEBUG
				logMsg("variLen:%d,variIpStr[%d]:%s\n", variRlt[initNo], IpStrno, (int)variIpStr[IpStrno], 0, 0, 0);
#endif

				if (0 == variRlt[initNo])
				{
					*errPosi = strNo;

					/*跳出switch*/
					break;
				}
				else
				{
					/*do nothing*/
				}

				strNo += variLen[compNo];
				variSize[compNo] = 1 + variLenSize[compNo] + variLen[compNo];

				/*保存输出结构体信息*/
				pRespStru->strTypeStru[IpStrno].oidNo = compNo;
				pRespStru->strTypeStru[IpStrno].strLen = variRlt[initNo];
				memcpy(pRespStru->strTypeStru[IpStrno].str, variIpStr[IpStrno], variRlt[initNo]);

				/*准备记录下一个IP信息*/
				IpStrno += 1;
				initNo += 1;

				/*跳出switch*/
				break;
			}
			default:
			{
				*errPosi = strNo;

				/*记录未知的数据类型*/
				pRespStru->errVariType = pTempRespStr[strNo];

				/*跳出switch*/
				break;
			}

			}

			/*errPosi初始值为0,表示没有发生错误或异常,解码后的strNo应该等于tempRecvLen*/
			if (0 != *errPosi)
			{
				/*跳出整个while循环*/
				break;
			}
			else
			{
				/*do nothing*/
			}
		}

		/*保存输出结构体信息*/
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


		/*errPosi初始值为0,表示没有发生错误或异常*/
		if (0 != *errPosi)
		{
			/*跳出整个while循环*/
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


		/*以下内容的赋值已在代码执行过程中完成:
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

	/*返回已解码的报文的长度*/
	return rtnVal;
}