/************************************************************************
*@ �ļ���	    �� dsuRsspDataStru.h
*@ ��Ȩ˵��	��
*@ �汾��	    �� 1.0
*@ ����ʱ��	�� 2020.4
*@ ����		    �� HHY
*@ ��������	�� ��RSRЭ����ص�dsu��ѯ�ṹ�嶨�壬�ڲ�ʹ��
*@ ʹ��ע��	��
*@ �޸ļ�¼	��

************************************************************************/

#ifndef _DSU_RSSP2_DATA_STRU_H
#define _DSU_RSSP2_DATA_STRU_H


/*������󳤶����ڴ洢���ݿ�������*/
#define DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE  (40)
#define DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE  (7) 
#define DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE  (40) 

#define DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG		 (7)
#define DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG (8)
#define DSU_DATA_PRTCL_RSSP2_DEV_CFG	 (9)

/* VOBCʹ�ö˱�� */
#define VOBC_ENDID_TC1     1    /* TC1�� */
#define VOBC_ENDID_TC2     2    /* TC2��*/
#define VOBC_ENDID_REDUN   0    /* 2IP������β���೵�� */

#undef RSSP2_2IP_MODE /*������*/

typedef enum
{
	DEVICE_TYPE_ZC = 0x01, /* ZC */
	DEVICE_TYPE_ATP = 0x02, /* ����ATP */
	DEVICE_TYPE_AOM = 0x03, /* ����AOM */
	DEVICE_TYPE_ATS = 0x04, /* ATS */
	DEVICE_TYPE_DSU = 0x05, /* DSU */
	DEVICE_TYPE_CI = 0x06, /* CI */
	DEVICE_TYPE_MSS = 0x07, /* MSS */
	DEVICE_TYPE_UNKNOWN = 0xFF  /* δ֪ */
}RSSP2_DEVICE_TYPE;


	/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
	/*rssp2 ��7 Э��ջ�㹲��������Ϣ�ṹ��*/
	typedef struct
	{
		unsigned char RemoteEtcsType;			/*�Է�ETCS����*/
		unsigned int SAITsyn;					/* SAI��Tsyn��ʱ����ʱֵ*/
		unsigned int SAIECRtnPeriod;			/* SAI�㳤���ڻص��������� */
		unsigned short SAISNDiffMax;            /* SAI���������ֵ */
		unsigned int SAIECAlarm;				/* SAI��EC����״ֵ̬*/
		unsigned int SAIECRtnValue;			/* SAI��EC�������ص�ֵ TO BE DONE */
		unsigned int SAIMaxErrValue;			/*SAI�������������ֵ */
		unsigned int SAIUpdFailCountMax;      /* SAI����¹��������������ֵ */
		unsigned int MASLTsynTimeOut;			/* MASL��Testab��ʱ����ʱֵ */
		unsigned int ALETsynTimeOut;			/* ALE��Tsyn��ʱ����ʱֵ */
		unsigned int ALETconTimeOut;			/*ALE��Tcon��ʱ����ʱֵ(ͨ��ʹ��,Э������㲻ʹ��) */
		unsigned char ALETSNCheckMode;          /* ALE��TSN���ģʽ */
	}DSU_RSSP2_WS_TYPE_CFG_STRU;  /*%RELAX<MISRA_8_9>*/

								  /*rssp2 ��8 RSSP2�豸���Ͳ�����ṹ��*/
	typedef struct
	{
		unsigned char LocalEtcsType;			/* ����ETCS����*/
		unsigned int SAI_Tupd; 		        /* Tupd��ʱ����ʱֵ*/
		unsigned short SAI_EcPeriod;    		/* SAI��EC������ֵ */
	}DSU_RSSP2_LOC_TYPE_CFG_STRU;/*%RELAX<MISRA_8_9>*/

	typedef struct
	{
		unsigned char Ip[4];
		unsigned short Port;
	}DSU_IP_PORT;  /*%RELAX<MISRA_5_3>*/

				   /*rssp2 ��9 RSSP2�豸ͨ�Ų�����ṹ��*/
	typedef struct
	{
		unsigned int EtcsId;				/*�豸ETCS���*/
		unsigned char Key[24];              /*������Կ*/
		DSU_IP_PORT BlueIpPort;
		DSU_IP_PORT RedIpPort;
	}DSU_RSSP2_DEV_CFG_STRU;/*%RELAX<MISRA_8_9>*/

	typedef struct
	{
		unsigned int dsuRssp2WsTypeCfgLen;
		DSU_RSSP2_WS_TYPE_CFG_STRU *pDsuRssp2WsTypeCfgStru;/* ��7 */

		unsigned int dsuRssp2LocTypeCfgLen;
		DSU_RSSP2_LOC_TYPE_CFG_STRU *pDsuRssp2LocTypeCfgStru;/* ��8 */

		unsigned int dsuRssp2DevCfgLen;
		DSU_RSSP2_DEV_CFG_STRU *pDsuRssp2DevCfgStru;/* ��9 */

	}DSU_RSSP2_INFO_STRU, *LPDSU_RSSP2_INFO_STRU;/*%RELAX<MISRA_8_9>*/


#endif

