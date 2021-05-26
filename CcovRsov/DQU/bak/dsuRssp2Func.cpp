#include "GlobalDefine.h"

#include "dsuRssp2Func.h"
#include "string.h"
#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "dquIdmapInit.h"


DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /*用于保存RSSP2表信息的结构体对象*/
DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* 当前正在操作的数据库 */

/*VOBC内部编号转4字节互联互通编号*/
unsigned char dquVobcName2OutDevId(unsigned short VobcName, unsigned char EndId, unsigned int *pOutDevId);
/*4字节ETCSID转VOBC内部编号*/
unsigned char dquEtcsId2VobcName(unsigned int EtcsId, unsigned short *pDevName, unsigned short *pEndId);

unsigned char dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru)
{
	unsigned char ret = 0;
	if (nullptr != pDsuRssp2Stru)
	{
		g_dsuRssp2Stru = pDsuRssp2Stru;
		ret = 1;
	}
	else
	{
		g_dsuRssp2Stru = nullptr;
		ret = 0;
	}
	return ret;
}
/************************************************************************
* 功能描述：安全通信协议RSSP2协议查询函数初始化函数，用于检查数据正确性，并将数据结构化。
使用协议查询函数之前调用一次
* 输入参数：
FSName 文件名或者数据地址
* 返 回 值：1，成功
*           0，失败，数据有错误
************************************************************************/
unsigned char dsuRssp2Init(char* FSName)
{
	unsigned char chReturnValue = 1; /*返回值*/
	unsigned char TempRet;
	unsigned char* pData = nullptr;       /*用于读取数据的指针*/
	unsigned int dataLen = 0;        /*数据长度*/
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int IbookLen = 0;

	if (nullptr != g_dsuRssp2Stru)
	{
		chReturnValue = 1;
	}
	else
	{
#if 1  /* TO BE DONE 仅限上电初始化模式的临时版本插桩，协议初始化的具体方式待定 */
		g_dsuRssp2Stru = &dsuRssp2InfoStru;
		chReturnValue = 1;
#else
		chReturnValue = 0;
#endif
	}

	/******************************************表7*****************************************/
	/*初始化rssp2 协议栈层间共享配置 */
	/*读取数据*/
	TempRet = dquGetConfigData((char *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen%DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE)))
	{
		IbookLen = (unsigned int)(dataLen / DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);  /*计算表格中数据的个数*/
		if (0 < IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = (DSU_RSSP2_WS_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen); /*为结构体申请空间*/
			if (nullptr == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
			{
				chReturnValue = 0;
			}
		}
		if (1 == chReturnValue)
		{
			memset(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru, 0, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen);
			/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->RemoteEtcsType = *(pData + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAITsyn = char_to_long(pData + 1 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECRtnPeriod = char_to_long(pData + 5 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAISNDiffMax = char_to_short(pData + 9 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECAlarm = char_to_long(pData + 11 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIECRtnValue = char_to_long(pData + 15 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIMaxErrValue = char_to_long(pData + 19 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->SAIUpdFailCountMax = char_to_long(pData + 23 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->MASLTsynTimeOut = char_to_long(pData + 27 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETsynTimeOut = char_to_long(pData + 31 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETconTimeOut = char_to_long(pData + 35 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
				(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru + i)->ALETSNCheckMode = *(pData + 39 + i*DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);
			}/* end for i */
			g_dsuRssp2Stru->dsuRssp2WsTypeCfgLen = IbookLen;
		}
	}
	else
	{
		/* 读取配置文件失败 */
		chReturnValue = 0;
	}

	/******************************************表8*****************************************/
	/*初始化rssp2 点对点安全连接配置表 */
	/*读取数据*/
	TempRet = dquGetConfigData((char *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen % DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE)))
	{
		IbookLen = (unsigned int)(dataLen / DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE);  /*计算表格中数据的个数*/

		if (0<IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = (DSU_RSSP2_LOC_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen); /*为结构体申请空间*/
			if (nullptr == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
			{
				chReturnValue = 0;
			}
		}
		if (1 == chReturnValue)
		{
			memset(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru, 0, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen);
			/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->LocalEtcsType = *(pData + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->SAI_Tupd = char_to_long(pData + 1 + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru + i)->SAI_EcPeriod = char_to_short(pData + 5 + DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE*i);
			}
			g_dsuRssp2Stru->dsuRssp2LocTypeCfgLen = IbookLen;
		}
	}
	else
	{
		/* 读取配置文件失败 */
		chReturnValue = 0;
	}

	/******************************************表9*****************************************/
	/* 初始化rssp2 当前TCP连接通道配置表 */
	/* 读取数据 */
	TempRet = dquGetConfigData((char *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_DEV_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (dataLen > 0))
	{
		IbookLen = (unsigned int)(dataLen / DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE);  /*计算表格中数据的个数*/
		if (0 < IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = (DSU_RSSP2_DEV_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen); /*为结构体申请空间*/
			if (nullptr == g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
			{
				chReturnValue = 0;
			}
		}

		if (1 == chReturnValue)
		{
			memset(g_dsuRssp2Stru->pDsuRssp2DevCfgStru, 0, sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen);
			/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
			for (i = 0; i < IbookLen; i++)
			{
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->EtcsId = char_to_long(pData + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
				memcpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->Key, (pData + 4 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 24);
				memcpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->BlueIpPort.Ip, (pData + 28 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 4);
				memcpy((g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->RedIpPort.Ip, (pData + 32 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i), 4);
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->BlueIpPort.Port = char_to_short(pData + 36 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
				(g_dsuRssp2Stru->pDsuRssp2DevCfgStru + i)->RedIpPort.Port = char_to_short(pData + 38 + DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE*i);
			}
			g_dsuRssp2Stru->dsuRssp2DevCfgLen = IbookLen;
		}
	}
	else
	{
		/* 读取配置文件失败 */
		chReturnValue = 0;
	}

	return chReturnValue;
}

/************************************************************************
功能：内存释放函数
输入：无
输出：无
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dsuRsspIIFreeSpace(void)
{
	unsigned char rt = 0;

	if (nullptr != g_dsuRssp2Stru)
	{
		if (nullptr != g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru);
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = nullptr;
		}

		if (nullptr != g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru);
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = nullptr;
		}

		if (nullptr != g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
		{
			free((void*)g_dsuRssp2Stru->pDsuRssp2DevCfgStru);
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = nullptr;
		}
		rt = 1;
	}
	return rt;
}

/************************************************************************
功能：2字节内部编号和2字节逻辑编号转4字节ETCSID
输入：
IN unsigned short DevName     2字节内部编号（高位设备类型 低位设备索引号)
IN unsigned short LogId       vobc时是使用端编号 1-TC1 2-TC2 0-2IP车（首尾冗余车）
其它设备时无意义
输出：
OUT unsigned int *pEtcsId    4字节ETCSID
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquDevName2EtcsId(unsigned short DevName, unsigned short LogId, unsigned int *pEtcsId)
{
	unsigned char ret = 0;
	unsigned int hlhtId = 0;
	unsigned int etcsId = 0;
	unsigned char devType = 0;
	unsigned char devId = 0;
	unsigned int i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = nullptr;

	devType = (unsigned char)(DevName / 256);
	devId = (unsigned char)(DevName & 0x00FF);

	/*入口检查*/
	if (nullptr == pEtcsId)
	{
		ret = 0;
	}
	else
	{
		*pEtcsId = 0;
		ret = 1;
	}
	if (nullptr == g_dsuRssp2Stru)
	{
		ret = 0;
	}

	/* 查找互联互通编号 */
	if (1 == ret)
	{
		switch (devType)
		{
		case DEVTYPE_ZC:
			ret = dquQueryOutDevId(QFUN_IDMAP_ZC_ID, devId, &hlhtId);
			break;
		case DEVTYPE_ATS:
			ret = dquQueryOutDevId(QFUN_IDMAP_ATS_ID, devId, &hlhtId);
			break;
		case DEVTYPE_CI:
			ret = dquQueryOutDevId(QFUN_IDMAP_CI_ID, devId, &hlhtId);
			if ((((hlhtId) >> 29) & 0x00000007) == 0x01) /* CQ HOTFIX TH联锁 */
			{
				if (LogId == 1 || LogId == 2)
				{
					hlhtId += LogId - 1;
				}
			}
			break;
		case DEVTYPE_VOBC:
			ret = dquVobcName2OutDevId(DevName, (unsigned char)LogId, &hlhtId);
			break;
		case DEVTYPE_NDSU:
			ret = dquQueryOutDevId(QFUN_IDMAP_DSU_ID, devId, &hlhtId);
			break;
		case DEVTYPE_AOM:
			ret = dquVobcName2OutDevId(DevName, (unsigned char)LogId, &hlhtId);
			break;
		default:
			ret = 0;
			break;
		}
	}

	/*查找对应的ETCSID*/
	if (1 == ret)
	{
		ret = 0;
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((hlhtId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				etcsId = pDevCfg->EtcsId;
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}

	if (1 == ret)
	{
		*pEtcsId = etcsId;
	}

	return ret;
}

/************************************************************************
功能：4字节ETCSID转2字节内部编号和2字节逻辑编号
输入：
IN unsigned int EtcsId    ETCSID
输出：
OUT unsigned short *pDevId     2字节内部编号（高位设备类型 低位设备索引号)
OUT unsigned short *pLogicId   vobc时是使用端编号 1-TC1 2-TC2 0-2IP车（首尾冗余车）
其它设备时无意义
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquEtcsId2DevName(unsigned int EtcsId, unsigned short *pDevName, unsigned short *pLogId)
{
	unsigned char ret = 0;
	unsigned short devName = 0;
	unsigned short logId = 0;
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;

	if ((nullptr != pDevName) && (nullptr != pLogId))
	{
		GetEtcsTypeFromEtcsId(EtcsId, &type);
		switch (type)
		{
		case DEVICE_TYPE_ZC:/* 查ZC编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ZC_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ZC * 256;
			break;
		case DEVICE_TYPE_ATP:/* 查VOBC编号表 */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		case DEVICE_TYPE_ATS:/* 查ATS编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ATS_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ATS * 256;
			break;
		case DEVICE_TYPE_DSU:/* 查DSU编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_DSU_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_NDSU * 256;
			break;
		case DEVICE_TYPE_CI:/* 查CI编号表 */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId, ETCSID_MASK, &devName);
			if (1 != ret)  /* CQ HOTFIX TH联锁 */
			{
				ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId - 1, ETCSID_MASK, &devName);
				if (1 == ret)
				{
					logId = 2;
				}
			}
			devName += DEVTYPE_CI * 256;
			break;
		case DEVICE_TYPE_AOM:/* 查AOM编号表 */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		ret = 0;
	}
	if (1 == ret)
	{
		*pDevName = devName;
		*pLogId = logId;
	}
	return ret;
}


/************************************************************************
功能：vobc内部编号转4字节互联互通编号
输入：
IN unsigned short VobcName      2字节内部编号（高位设备类型 低位设备索引号)
IN unsigned char EndId        vobc使用端编号编号 1-TC1端，2-TC2端 0-2IP车（首尾冗余车）
输出：
OUT unsigned int *pOutDevId    4字节互联互通编号
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquVobcName2OutDevId(unsigned short VobcName, unsigned char EndId, unsigned int *pOutDevId)
{
	unsigned char ret = 0;
	unsigned int hlhtId = 0;/*互联互通编号*/
	unsigned int vobcId[2] = { 0,0 };
	unsigned char devType = 0;
	unsigned char devId = 0;

	/* 输入检查 */
	if (nullptr != pOutDevId)
	{
		*pOutDevId = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	if (nullptr == g_dsuRssp2Stru)
	{
		ret = 0;
	}
	if ((VOBC_ENDID_TC1 != EndId) && (VOBC_ENDID_TC2 != EndId) && (VOBC_ENDID_REDUN != EndId))
	{
		ret = 0;
	}
	devType = (unsigned char)(VobcName / 256);
	if ((DEVTYPE_VOBC != devType) && (DEVTYPE_AOM != devType))
	{
		ret = 0;
	}

	/* 查询VOBC互联互通编号 */
	if (1 == ret)
	{
		devId = (unsigned char)(VobcName & 0x00FF);
		if (DEVTYPE_VOBC == devType)
		{
			ret = dquQueryOutVobcDevId(devId, vobcId);
		}
		else
		{
			ret = dquQueryOutAOMDevId(devId, vobcId);
		}
		switch (EndId)
		{
		case VOBC_ENDID_TC1:
			hlhtId = vobcId[0];
			break;
		case VOBC_ENDID_TC2:
			hlhtId = vobcId[1];
			break;
		case VOBC_ENDID_REDUN:/*2IP车（首尾冗余车）使用"VOBC编号1"*/
			hlhtId = vobcId[0];
			break;
		default:
			/* 不可达分支 */
			ret = 0;
			break;
		}
	}

	if (1 == ret)
	{
		*pOutDevId = hlhtId;
	}

	return ret;
}

/************************************************************************
功能：ETCSID转vobc内部编号
输入：
IN unsigned int EtcsId    ETCSID
输出：
OUT unsigned short *pDevName     2字节内部编号（高位设备类型 低位设备索引号)
OUT unsigned short *pEndId		vobc使用端编号 1-TC1 2-TC2 0-2IP车（首尾冗余车）
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquEtcsId2VobcName(unsigned int EtcsId, unsigned short *pDevName, unsigned short *pEndId)
{
	unsigned char ret = 0;
	unsigned char endId = 0;
	unsigned short index = 0;
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;

	if ((nullptr != pDevName) && (nullptr != pEndId))
	{
		*pDevName = 0;
		*pEndId = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		endId = GET_VOBC_END_ID(EtcsId);
		switch (endId)
		{
		case 1:
			*pEndId = VOBC_ENDID_TC1;
			break;
		case 2:
			*pEndId = VOBC_ENDID_TC2;
			break;
		case 3:
			*pEndId = VOBC_ENDID_REDUN;
			break;
		default:
			ret = 0;
			break;
		}
	}

	GetEtcsTypeFromEtcsId(EtcsId, &type);
	if ((DEVICE_TYPE_ATP != type) && (DEVICE_TYPE_AOM != type))
	{
		ret = 0;
	}

	if (1 == ret)
	{
		if (DEVICE_TYPE_ATP == type)
		{
			ret = dquQueryVobcDevBits2Index(EtcsId, ETCSID_MASK, &index);
		}
		else
		{
			ret = dquQueryAOMDevBits2Index(EtcsId, ETCSID_MASK, &index);
		}
	}

	if (1 == ret)
	{
		if (DEVICE_TYPE_ATP == type)
		{
			*pDevName = DEVTYPE_VOBC * 256 + index;
		}
		else
		{
			*pDevName = DEVTYPE_AOM * 256 + index;
		}
	}
	return ret;
}

/************************************************************************
功能：根据ETCSID获取RSSP2协议使用的IP端口
输入：
IN unsigned int EtcsId    ETCSID
输出：
OUT unsigned char *pNum,    TCP冗余通道组数(一对红蓝网IP端口为一组，即4IP系统为2组)
OUT DSU_IP_PORT *pRedIpPort,   红网IP端口数组(端口号65535表示随机端口)
OUT DSU_IP_PORT *pBlueIpPort   篮网IP端口数组(与红网IP端口数组相同角标的元素组成一对冗余通道)
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquGetIpPortByEtcsId(unsigned int EtcsId, unsigned char *pNum, DSU_IP_PORT *pRedIpPort, DSU_IP_PORT *pBlueIpPort)
{
	unsigned char ret = 0;
	unsigned int i = 0;
	unsigned char j = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = nullptr;
	if ((nullptr != pNum) && (nullptr != pRedIpPort) && (nullptr != pBlueIpPort) && (nullptr != g_dsuRssp2Stru))
	{
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((EtcsId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				memcpy(pRedIpPort[j].Ip, pDevCfg->RedIpPort.Ip, 4);
				pRedIpPort[j].Port = pDevCfg->RedIpPort.Port;
				memcpy(pBlueIpPort[j].Ip, pDevCfg->BlueIpPort.Ip, 4);
				pBlueIpPort[j].Port = pDevCfg->BlueIpPort.Port;
				j++;
				ret = 1;
			}
			pDevCfg++;
		}
		*pNum = j;
	}
	return ret;
}

/************************************************************************
功能：根据RSSP-2协议使用的IP端口获取ETCSID
描述：如果配置数据中配置端口号为65535（表示随机端口）则不校验传入的端口号，
只要IP匹配就输出对应的ETCSID
输入：
IN DSU_IP_PORT IpPort,   IP端口
输出：
OUT unsigned int *pEtcsId,    ETCSID
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquGetEtcsIdByIpPort(DSU_IP_PORT IpPort, unsigned int *pEtcsId)
{
	unsigned char ret = 0;
	unsigned int i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = nullptr;
	int rslt1 = 0;
	int rslt2 = 0;
	if ((nullptr != pEtcsId) && (nullptr != g_dsuRssp2Stru))
	{
		*pEtcsId = 0;
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			rslt1 = memcmp(IpPort.Ip, pDevCfg->RedIpPort.Ip, 4);
			rslt2 = memcmp(IpPort.Ip, pDevCfg->BlueIpPort.Ip, 4);
			if (((0 == rslt1) && ((IpPort.Port == pDevCfg->RedIpPort.Port) || (0xFFFF == pDevCfg->RedIpPort.Port)))
				|| ((0 == rslt2) && ((IpPort.Port == pDevCfg->BlueIpPort.Port) || (0xFFFF == pDevCfg->BlueIpPort.Port))))
			{
				*pEtcsId = pDevCfg->EtcsId;
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}
	return ret;
}

/************************************************************************
功能：根据轨旁设备类型(ETCS类型)获取协议栈层间共享配置
输入：
IN unsigned char WsEtcsType 轨旁设备类型(ETCS类型)
输出：
OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg 协议栈共享配置
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquRssp2GetWaysideTypeCfg(unsigned char WsEtcsType, DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg)
{
	unsigned char ret = 0;
	unsigned char tmpRet = 0;
	unsigned int i = 0;

	if ((nullptr != pRssp2RmtTypeCfg) && (nullptr != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}

	if (1 == tmpRet)
	{
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2WsTypeCfgLen; i++)
		{
			if (WsEtcsType == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru[i].RemoteEtcsType)
			{
				memcpy((unsigned char*)pRssp2RmtTypeCfg,  (unsigned char*)&g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru[i], sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU));
				ret = 1;
				break;
			}
		}
	}
	return ret;
}

/************************************************************************
功能：根据ETCS设备类型获取本地设备类型配置数据
输入：
IN unsigned char LocalEtcsType 互联互通设备编号(4字节外部编号)
输出：
OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg RSSP2设备类型参数
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquRssp2GetLocalTypeCfg(unsigned char LocalEtcsType, DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg)
{
	unsigned char ret = 0;
	unsigned char tmpRet = 0;
	unsigned int i = 0;
	DSU_RSSP2_LOC_TYPE_CFG_STRU *pLocTypeCfg = nullptr;
	if ((nullptr != pRssp2LocTypeCfg) && (nullptr != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}
	if (1 == tmpRet)
	{
		pLocTypeCfg = g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2LocTypeCfgLen; i++)
		{
			if (LocalEtcsType == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru[i].LocalEtcsType)
			{
				memcpy((unsigned char*)pRssp2LocTypeCfg,  (unsigned char*)pLocTypeCfg, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU));
				ret = 1;
				break;
			}
			pLocTypeCfg++;
		}
	}
	return ret;
}


/************************************************************************
功能：根据ETCSID获取设备配置数据
输入：
IN unsigned int EtcsId ETCSID
输出：
OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg 点对点安全连接配置
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquRssp2GetDevCfg(unsigned int EtcsId, DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg)
{
	unsigned char ret = 0;
	unsigned char tmpRet = 0;
	unsigned int i = 0;
	DSU_RSSP2_DEV_CFG_STRU *pDevCfg = nullptr;

	if ((nullptr != pRssp2DevCfg) && (nullptr != g_dsuRssp2Stru))
	{
		tmpRet = 1;
	}
	else
	{
		tmpRet = 0;
	}
	if (1 == tmpRet)
	{
		pDevCfg = g_dsuRssp2Stru->pDsuRssp2DevCfgStru;
		for (i = 0; i < g_dsuRssp2Stru->dsuRssp2DevCfgLen; i++)
		{
			if ((EtcsId&ETCSID_MASK) == (pDevCfg->EtcsId&ETCSID_MASK))
			{
				memcpy((unsigned char*)pRssp2DevCfg, (unsigned char*)pDevCfg, sizeof(DSU_RSSP2_DEV_CFG_STRU));
				ret = 1;
				break;
			}
			pDevCfg++;
		}
	}
	return ret;
}

/************************************************************************
功能：根据ETCSID获取ETCS类型
输入：
IN unsigned int EtcsId, ETCSID
输出：
OUT RSSP2_DEVICE_TYPE *pDevType  ETCS类型
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char GetEtcsTypeFromEtcsId(unsigned int EtcsId, RSSP2_DEVICE_TYPE *pDevType)
{
	unsigned char ret = 0;
	unsigned char devType = 0;

	devType = (EtcsId / 256 / 256 / 256) & 0x1F;

	if (nullptr != pDevType)
	{
		*pDevType = DEVICE_TYPE_UNKNOWN;
		if ((8 > devType) && (0 < devType))
		{
			*pDevType = RSSP2_DEVICE_TYPE(devType);
			ret = 1;
		}
	}
	return ret;
}

/************************************************************************
功能：根据本方ETCSID获取本方的通信角色
输入：
IN unsigned int etcsId_local,  本地设备ETCSID
输出：
OUT unsigned char *pRole  本方通信角色 0发起方(客户端) 1接收方(服务端)
返回值：unsigned char 1成功 0失败
************************************************************************/
unsigned char dquRssp2GetRole(unsigned int etcsId_local, unsigned char *pRole)
{
	RSSP2_DEVICE_TYPE type = DEVICE_TYPE_UNKNOWN;
	unsigned char role = 0;
	unsigned char ret = 0;
	GetEtcsTypeFromEtcsId(etcsId_local, &type);
	if (nullptr == pRole)
	{
		ret = 0;
	}
	else if ((DEVICE_TYPE_ZC == type) || (DEVICE_TYPE_CI == type) || (DEVICE_TYPE_ATS == type) || (DEVICE_TYPE_DSU == type))
	{
		role = 1;
		ret = 1;
	}
	else if ((DEVICE_TYPE_ATP == type) || (DEVICE_TYPE_AOM == type))
	{
		role = 0;
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	if (1 == ret)
	{
		*pRole = role;
	}
	return ret;
}