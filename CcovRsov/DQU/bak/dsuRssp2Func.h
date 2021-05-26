/************************************************************************
*@ �ļ���	    �� dsuRsspDataStru.h
*@ ��Ȩ˵��	��
*@ �汾��	    �� 1.0
*@ ����ʱ��	�� 2020.4
*@ ����		    �� HHY
*@ ��������	�� RSSP2Э���ѯ����
*@ ʹ��ע��	�� RSSP2��ѯ������Ҫʹ�ñ�Ŷ��ձ�����Ӧ����Ҫ�ֱ��ʼ��RSSP2��ѯ�����ͱ�Ŷ��ձ����Ⱥ����Ҫ�󣩣��������ֶ���ʼ����ɺ�ſ���ʹ��RSSP2Э���ѯ����
�ȼ���ʱ��RSSP2��ѯ�����ͱ�Ŷ��ձ�������µ�����Դ�����Ⱥ����Ҫ�󣩲ſ���ʹ��RSSP2Э���ѯ����
*@ �޸ļ�¼	��

************************************************************************/

#ifndef _DSU_RSSP2_FUNC_H_
#define _DSU_RSSP2_FUNC_H_

#include "common/CommonTypes.h"
#include "dsuRssp2DataStru.h"
#include "dsuIdmapVar.h"
#include "dsuEmapDefine.h"


#define GET_VOBC_END_ID(EtcsId) (unsigned char)((EtcsId)&0x03)
#define ETCSID_MASK (0x7FFFFFF)

	extern DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /* ���ڱ���RSSP2����Ϣ�Ľṹ�����*/
	extern DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* ��ǰ���ڲ��������ݿ� */

													 /* ����RSSP2�������ݾ�̬�ṹ�� */
	unsigned char dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru);

	/*RSSP2Э���ѯ������ʼ��*/
	unsigned char dsuRssp2Init(char* FSName);

	/*2�ֽ��ڲ����ת4�ֽ�ETCSID*/
	unsigned char dquDevName2EtcsId(unsigned short DevName, unsigned short LogId, unsigned int *pEtcsId);

	/*4�ֽ�ETCSIDת2�ֽ��ڲ����*/
	unsigned char dquEtcsId2DevName(unsigned int EtcsId, unsigned short *pDevName, unsigned short *pLogId);

	/*����ETCSID��ȡRSSP-2Э��ʹ�õ�IP�˿�*/
	unsigned char dquGetIpPortByEtcsId(unsigned int EtcsId, unsigned char *pNum, DSU_IP_PORT *pRedIpPort, DSU_IP_PORT *pBlueIpPort);

	/*����RSSP-2Э��ʹ�õ�IP�˿ڻ�ȡETCSID*/
	unsigned char dquGetEtcsIdByIpPort(DSU_IP_PORT IpPort, unsigned int *pEtcsId);

	/*��ȡ�����豸������������*/
	unsigned char dquRssp2GetWaysideTypeCfg(unsigned char WsEtcsType, DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg);

	/*��ȡ�����豸������������*/
	unsigned char dquRssp2GetLocalTypeCfg(unsigned char LocalEtcsType, DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg);

	/*��ȡ�豸��������*/
	unsigned char dquRssp2GetDevCfg(unsigned int EtcsId, DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg);

	/* ���ݻ�����ͨ�豸��Ż�ȡ������ͨ�豸���� */
	unsigned char GetEtcsTypeFromEtcsId(unsigned int EtcsId, RSSP2_DEVICE_TYPE *pDevType);

	/*���ݱ����豸��Ż�ȡ������ͨ�Ž�ɫ*/
	unsigned char dquRssp2GetRole(unsigned int etcsId_local, unsigned char *pRole);

	/*�ڴ��ͷź���*/
	unsigned char dsuRsspIIFreeSpace(void);


#endif


