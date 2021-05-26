#include "GlobalDefine.h"

#include "dsuRssp2Func.h"
#include "string.h"
#include "Utilities/Convert/ConvertWork.h"
using namespace Convert;
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"
#include "dquIdmapInit.h"


DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /*���ڱ���RSSP2����Ϣ�Ľṹ�����*/
DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* ��ǰ���ڲ��������ݿ� */

/*VOBC�ڲ����ת4�ֽڻ�����ͨ���*/
unsigned char dquVobcName2OutDevId(unsigned short VobcName, unsigned char EndId, unsigned int *pOutDevId);
/*4�ֽ�ETCSIDתVOBC�ڲ����*/
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
* ������������ȫͨ��Э��RSSP2Э���ѯ������ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
ʹ��Э���ѯ����֮ǰ����һ��
* ���������
FSName �ļ����������ݵ�ַ
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
************************************************************************/
unsigned char dsuRssp2Init(char* FSName)
{
	unsigned char chReturnValue = 1; /*����ֵ*/
	unsigned char TempRet;
	unsigned char* pData = nullptr;       /*���ڶ�ȡ���ݵ�ָ��*/
	unsigned int dataLen = 0;        /*���ݳ���*/
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int IbookLen = 0;

	if (nullptr != g_dsuRssp2Stru)
	{
		chReturnValue = 1;
	}
	else
	{
#if 1  /* TO BE DONE �����ϵ��ʼ��ģʽ����ʱ�汾��׮��Э���ʼ���ľ��巽ʽ���� */
		g_dsuRssp2Stru = &dsuRssp2InfoStru;
		chReturnValue = 1;
#else
		chReturnValue = 0;
#endif
	}

	/******************************************��7*****************************************/
	/*��ʼ��rssp2 Э��ջ��乲������ */
	/*��ȡ����*/
	TempRet = dquGetConfigData((char *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen%DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE)))
	{
		IbookLen = (unsigned int)(dataLen / DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE);  /*�����������ݵĸ���*/
		if (0 < IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru = (DSU_RSSP2_WS_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen); /*Ϊ�ṹ������ռ�*/
			if (nullptr == g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru)
			{
				chReturnValue = 0;
			}
		}
		if (1 == chReturnValue)
		{
			memset(g_dsuRssp2Stru->pDsuRssp2WsTypeCfgStru, 0, sizeof(DSU_RSSP2_WS_TYPE_CFG_STRU)*IbookLen);
			/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
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
		/* ��ȡ�����ļ�ʧ�� */
		chReturnValue = 0;
	}

	/******************************************��8*****************************************/
	/*��ʼ��rssp2 ��Ե㰲ȫ�������ñ� */
	/*��ȡ����*/
	TempRet = dquGetConfigData((char *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (0 == (dataLen % DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE)))
	{
		IbookLen = (unsigned int)(dataLen / DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE);  /*�����������ݵĸ���*/

		if (0<IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru = (DSU_RSSP2_LOC_TYPE_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen); /*Ϊ�ṹ������ռ�*/
			if (nullptr == g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru)
			{
				chReturnValue = 0;
			}
		}
		if (1 == chReturnValue)
		{
			memset(g_dsuRssp2Stru->pDsuRssp2LocTypeCfgStru, 0, sizeof(DSU_RSSP2_LOC_TYPE_CFG_STRU)*IbookLen);
			/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
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
		/* ��ȡ�����ļ�ʧ�� */
		chReturnValue = 0;
	}

	/******************************************��9*****************************************/
	/* ��ʼ��rssp2 ��ǰTCP����ͨ�����ñ� */
	/* ��ȡ���� */
	TempRet = dquGetConfigData((char *)FSName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PRTCL_RSSP2_DEV_CFG, &pData, &dataLen);
	if ((1 == chReturnValue) && (1 == TempRet) && (dataLen > 0))
	{
		IbookLen = (unsigned int)(dataLen / DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE);  /*�����������ݵĸ���*/
		if (0 < IbookLen)
		{
			g_dsuRssp2Stru->pDsuRssp2DevCfgStru = (DSU_RSSP2_DEV_CFG_STRU*)MyNew(sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen); /*Ϊ�ṹ������ռ�*/
			if (nullptr == g_dsuRssp2Stru->pDsuRssp2DevCfgStru)
			{
				chReturnValue = 0;
			}
		}

		if (1 == chReturnValue)
		{
			memset(g_dsuRssp2Stru->pDsuRssp2DevCfgStru, 0, sizeof(DSU_RSSP2_DEV_CFG_STRU)*IbookLen);
			/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
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
		/* ��ȡ�����ļ�ʧ�� */
		chReturnValue = 0;
	}

	return chReturnValue;
}

/************************************************************************
���ܣ��ڴ��ͷź���
���룺��
�������
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ�2�ֽ��ڲ���ź�2�ֽ��߼����ת4�ֽ�ETCSID
���룺
IN unsigned short DevName     2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
IN unsigned short LogId       vobcʱ��ʹ�ö˱�� 1-TC1 2-TC2 0-2IP������β���೵��
�����豸ʱ������
�����
OUT unsigned int *pEtcsId    4�ֽ�ETCSID
����ֵ��unsigned char 1�ɹ� 0ʧ��
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

	/*��ڼ��*/
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

	/* ���һ�����ͨ��� */
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
			if ((((hlhtId) >> 29) & 0x00000007) == 0x01) /* CQ HOTFIX TH���� */
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

	/*���Ҷ�Ӧ��ETCSID*/
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
���ܣ�4�ֽ�ETCSIDת2�ֽ��ڲ���ź�2�ֽ��߼����
���룺
IN unsigned int EtcsId    ETCSID
�����
OUT unsigned short *pDevId     2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
OUT unsigned short *pLogicId   vobcʱ��ʹ�ö˱�� 1-TC1 2-TC2 0-2IP������β���೵��
�����豸ʱ������
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
		case DEVICE_TYPE_ZC:/* ��ZC��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ZC_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ZC * 256;
			break;
		case DEVICE_TYPE_ATP:/* ��VOBC��ű� */
			ret = dquEtcsId2VobcName(EtcsId, &devName, &logId);
			break;
		case DEVICE_TYPE_ATS:/* ��ATS��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_ATS_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_ATS * 256;
			break;
		case DEVICE_TYPE_DSU:/* ��DSU��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_DSU_ID, EtcsId, ETCSID_MASK, &devName);
			devName += DEVTYPE_NDSU * 256;
			break;
		case DEVICE_TYPE_CI:/* ��CI��ű� */
			ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId, ETCSID_MASK, &devName);
			if (1 != ret)  /* CQ HOTFIX TH���� */
			{
				ret = dquQueryDevBits2Index(QFUN_IDMAP_CI_ID, EtcsId - 1, ETCSID_MASK, &devName);
				if (1 == ret)
				{
					logId = 2;
				}
			}
			devName += DEVTYPE_CI * 256;
			break;
		case DEVICE_TYPE_AOM:/* ��AOM��ű� */
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
���ܣ�vobc�ڲ����ת4�ֽڻ�����ͨ���
���룺
IN unsigned short VobcName      2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
IN unsigned char EndId        vobcʹ�ö˱�ű�� 1-TC1�ˣ�2-TC2�� 0-2IP������β���೵��
�����
OUT unsigned int *pOutDevId    4�ֽڻ�����ͨ���
����ֵ��unsigned char 1�ɹ� 0ʧ��
************************************************************************/
unsigned char dquVobcName2OutDevId(unsigned short VobcName, unsigned char EndId, unsigned int *pOutDevId)
{
	unsigned char ret = 0;
	unsigned int hlhtId = 0;/*������ͨ���*/
	unsigned int vobcId[2] = { 0,0 };
	unsigned char devType = 0;
	unsigned char devId = 0;

	/* ������ */
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

	/* ��ѯVOBC������ͨ��� */
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
		case VOBC_ENDID_REDUN:/*2IP������β���೵��ʹ��"VOBC���1"*/
			hlhtId = vobcId[0];
			break;
		default:
			/* ���ɴ��֧ */
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
���ܣ�ETCSIDתvobc�ڲ����
���룺
IN unsigned int EtcsId    ETCSID
�����
OUT unsigned short *pDevName     2�ֽ��ڲ���ţ���λ�豸���� ��λ�豸������)
OUT unsigned short *pEndId		vobcʹ�ö˱�� 1-TC1 2-TC2 0-2IP������β���೵��
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ�����ETCSID��ȡRSSP2Э��ʹ�õ�IP�˿�
���룺
IN unsigned int EtcsId    ETCSID
�����
OUT unsigned char *pNum,    TCP����ͨ������(һ�Ժ�����IP�˿�Ϊһ�飬��4IPϵͳΪ2��)
OUT DSU_IP_PORT *pRedIpPort,   ����IP�˿�����(�˿ں�65535��ʾ����˿�)
OUT DSU_IP_PORT *pBlueIpPort   ����IP�˿�����(�����IP�˿�������ͬ�Ǳ��Ԫ�����һ������ͨ��)
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ�����RSSP-2Э��ʹ�õ�IP�˿ڻ�ȡETCSID
����������������������ö˿ں�Ϊ65535����ʾ����˿ڣ���У�鴫��Ķ˿ںţ�
ֻҪIPƥ��������Ӧ��ETCSID
���룺
IN DSU_IP_PORT IpPort,   IP�˿�
�����
OUT unsigned int *pEtcsId,    ETCSID
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ����ݹ����豸����(ETCS����)��ȡЭ��ջ��乲������
���룺
IN unsigned char WsEtcsType �����豸����(ETCS����)
�����
OUT DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg Э��ջ��������
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ�����ETCS�豸���ͻ�ȡ�����豸������������
���룺
IN unsigned char LocalEtcsType ������ͨ�豸���(4�ֽ��ⲿ���)
�����
OUT DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg RSSP2�豸���Ͳ���
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ�����ETCSID��ȡ�豸��������
���룺
IN unsigned int EtcsId ETCSID
�����
OUT DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg ��Ե㰲ȫ��������
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ�����ETCSID��ȡETCS����
���룺
IN unsigned int EtcsId, ETCSID
�����
OUT RSSP2_DEVICE_TYPE *pDevType  ETCS����
����ֵ��unsigned char 1�ɹ� 0ʧ��
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
���ܣ����ݱ���ETCSID��ȡ������ͨ�Ž�ɫ
���룺
IN unsigned int etcsId_local,  �����豸ETCSID
�����
OUT unsigned char *pRole  ����ͨ�Ž�ɫ 0����(�ͻ���) 1���շ�(�����)
����ֵ��unsigned char 1�ɹ� 0ʧ��
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