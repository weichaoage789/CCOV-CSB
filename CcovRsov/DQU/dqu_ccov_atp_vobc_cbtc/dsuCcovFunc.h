
/************************************************************************
* �ļ���	�� dsuCcovFunc.h
* ��Ȩ˵��	��	
* �汾��	�� 1.0  
* ����ʱ��	�� 2009.9.11
* ����		�� ������Э�鲿
* ��������	�� DSUΪCCOV�ṩ�Ĳ�ѯ���� 
* ʹ��ע��	�� 
* �޸ļ�¼	�� 

************************************************************************/
#ifndef _DSU_CCOV_FUNC_H
#define _DSU_CCOV_FUNC_H

#include "CommonTypes.h"
#include "IntegrationConfig.h"
#include "CommonTypes.h"
#include "dquCbtcType.h"

#define DSURP_LEN_IP_ADDR 4              /*IP��ַ����*/

 /*Դ��FAO��15��Ϊ30��BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO 30 /*zcdsuͨ�Ŷ����������*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM 15     /*zcdsuͨ�Ŷ����������*/

/*CCOV������IP�궨��*/
#define CCOV_RED_IP  0x01
#define CCOV_BLUE_IP 0x02
#define CCOV_UNKNOWN_IP	0xFF

#ifdef __cplusplus
extern "C" {
#endif

/*RpGetLnkInfo����ʹ�õĽṹ��*/
typedef struct {
	UINT8 DestIPRed[DSURP_LEN_IP_ADDR];/* ���ܷ�����IP��ַ*/
	UINT16 DestPortRed;/* ���ܷ������˿�*/
	UINT8 DestIPBlue[DSURP_LEN_IP_ADDR];/* ���ܷ�����IP��ַ*/
	UINT16 DestPortBlue;/* ���ܷ������˿�*/
	UINT8 LocalIPRed[DSURP_LEN_IP_ADDR];/* ���ͷ�����IP��ַ*/
	UINT16 LocalPortRed;/* ���ͷ������˿�*/
	UINT8 LocalIPBlue[DSURP_LEN_IP_ADDR];/* ���ͷ�����IP��ַ*/
	UINT16 LocalPortBlue;/* ���ͷ������˿�*/
} DSU_RP_LINK_INFO_STRU;

/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
/*ip��ַ�ṹ��*/
#pragma pack(2)  /*ָ����2�ֽڶ���*/

typedef struct
{
	UINT16 DevName;        /*�豸��ʶ��*/
	UINT32 HLHTId; 			/*������ͨID*/	
	UINT8 IpAddrBlue[DSURP_LEN_IP_ADDR];      /*����ip��ַ*/
	UINT8 IpAddrRed[DSURP_LEN_IP_ADDR];       /*����ip��ַ*/

	UINT16 SfpBluePort;    /*Sfp���������˿ں�*/
	UINT16 SfpRedPort;     /*Sfp���غ����˿ں�*/

	UINT16 RpBluePort;     /*Rp���������˿ں�*/
	UINT16 RpRedPort;     /*Rp���غ����˿ں�*/

	UINT16 RsspBluePort;     /*Rssp���������˿ں�*/
	UINT16 RsspRedPort;     /*Rssp���غ����˿ں�*/

	UINT16 BlueNetSourcePort;  /*����UDP����Դ�˿�*/
	UINT16 RedNetSourcePort;   /*����UDP����Դ�˿�*/

}DSU_IP_INFO_STRU;
#pragma pack()   /*ȡ��ָ�����룬�ָ�ȱʡ����*/


/*�豸������ͨ��Э���ϵ��*/
typedef struct  
{
	UINT8 EmitterType;        /*�����豸����*/
	UINT8 ReceptorType;       /*�����豸����*/

	UINT8 CommuType;          /*��ʹ�õ�ͨ��Э�飬1ΪSfp��2ΪRp��3ΪRssp*/
}DSU_COMM_PROTCL_STRU;


/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
typedef struct  
{
	UINT16 DeviceName_VOBC;	/*2	VOBC�豸����+�豸ID*/
	UINT16 	DeviceName_WGB;	/*2	WGB�豸����+�豸ID*/
	UINT16 	DeviceName_TimeServer;	/*2	�ź�ϵͳʱ�ӷ������豸����+�豸ID*/
	UINT16 	DeviceName_NMS;	/*2	NMS�豸����+�豸ID*/
	UINT16 	NMS_BluePort;	/*2	CCOV��NMSͨ�������˿�*/
	UINT16 	NMS_RedPort;	/*2	CCOV��NMSͨ�ź����˿�*/

}DSU_CCOV_COMM_INFO_STRU;

/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
typedef struct  
{
	UINT16 LocalDevName;    /*���豸���	2	�����豸������ID*/
	UINT16 CommObjNum;	    /*ͨ�Ŷ�������	2	�ݶ��������Ϊ15*/
	UINT16 CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO];	/*ͨ�Ŷ���1���	2	�����豸������ID*/
}DSU_ZCDSU_COMM_INFO_STRU;

/*�豸���ر����ݽṹ��*/
typedef struct  
{
	UINT16  Index;	                                   /*2	���*/
	UINT16  DeviceName;	                           		/*2	�豸����*/
	UINT8   IpAddrGatewayBlue[DSURP_LEN_IP_ADDR];      /*4	�豸��������*/
	UINT8   IpAddrGatewayRed[DSURP_LEN_IP_ADDR];       /*4	�豸��������*/
	UINT8   IpAddrSubnetMask[DSURP_LEN_IP_ADDR];       /*4	��������*/
}DSU_GATEWAY_INFO_STRU;


typedef struct
{
	/*Ip��ַ�Ͷ˿ں���Ϣ���ݳ��Ⱥ�����*/
	UINT16 IpInfoStruLen;      
	DSU_IP_INFO_STRU *pIpInfoStru;

    /*�豸������ͨ��Э���ϵ��*/
	UINT16 CommProtclStruLen;
	DSU_COMM_PROTCL_STRU *pCommProtclStru;

	/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	UINT16 CcovCommInfoLen;
	DSU_CCOV_COMM_INFO_STRU* pCcovCommInfoStru;

	/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	UINT16 ZcdsuCommInfoLen;
	DSU_ZCDSU_COMM_INFO_STRU* pZcdsuCommInfoStru;
	
	/*�豸���ر���Ϣ*/
	UINT16 GatewayInfoLen;
	DSU_GATEWAY_INFO_STRU* pGatewayInfoStru;

}DSU_BOOK_IP_INFO_STRU;

extern DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     


/* ��־�ӿ� */
void dquLogMsg(INT32 level, const CHAR* fmt, ...);

/*DSU�ṩ�Ĳ�ѯCCOV���������Եĺ���
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*����ֵ��  1-����; 2-����; 0xff-��Ч
*/
UINT8 dsuGetCcovRedOrBlue(const UINT8 CcovIpBuf[]);

/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*		 WgbBufSize ͨ�ſ�����IP��ַ��������С
*���������WgbIpBuf  WGB��IP��ַ
*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
*/
UINT8 dsuWgbIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 WgbIpBuf[][4], UINT16 WgbBufSize);

/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*        size ͨ�ſ�������IP��ַ��������С
*���������NtpServerIpBuf  NTP��������IP��ַ
*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
*/
UINT8 dsuNtpServerIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NtpServerIpBuf[][4], UINT16 size);

/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
*�������: CcovIpBuf ͨ�ſ�������IP��ַ
*���������NmsIpBuf  ����������ϵͳ��IP��ַ
UINT16* NmsPort���˿ں�
*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
*/
UINT8 dsuNmsIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NmsIpBuf[],UINT16* NmsPort);

/*ͨ��ip��ַ��ѯ�豸���ƺͺ�������Ϣ
*�������: UINT8 CcovIpBuf[]  �豸ip��ַ
*���������UINT16* DevName �豸����
*����ֵ��  1-����; 2-����; 0xff-��Ч
*/
UINT8 dsuGetDevNameByIp(const UINT8 DevIpBuf[],UINT16* DevName);

/*ͨ���豸���ƺͺ�������ѯ�豸ip��ַ
*�������:  UINT16 DevName �豸����
			      UINT8 RedOrBlue 1-����; 2-����; 0xff-��Ч
*���������UINT8 CcovIpBuf[]  �豸ip��ַ
*����ֵ��  1-��ѯ�ɹ�;  0-��ѯʧ��
*�޸ļ�¼: �޸�QC689,�Ƴ�,20180816;
*/
UINT8 dsuGetDevIpByName(UINT8 DevIpBuf[], UINT16 DevName, UINT8 RedOrBlue);

/*ͨ���豸���ƺͺ�������ѯ�豸IP��ַ�Ͷ˿ں�
*�������: UINT16 DevName �豸����
*			    int nIPMaxCount:IP��ַ���������
*���������ipInfo �豸IP��ַ��Ϣ����
*����ֵ��  �ҵ���IP����
*/
UINT8 dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, UINT16 DevName);

/*
* �������������ݺ�����IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
UINT8 dsuGetRedDevName(UINT8* ipAdd,UINT16 port,UINT16* devName) ;

/*
* ��������������������IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
UINT8 dsuGetBlueDevName(UINT8* ipAdd,UINT16 port,UINT16* devName) ;

/*ͨ���豸���ƺͺ�������ѯ�豸���е�ip��ַ
*�������: UINT16 DevName �豸����
UINT8 RedOrBlue 1-����; 2-����; 0xff-��Ч
* UINT16 BufSize  �豸IP��ַ��������С
*���������UINT8 CcovIpBuf[]  �豸ip��ַ
*����ֵ��  �ҵ���IP��ַ������
*/
UINT8 dsuGetAllDevIpByName(UINT8 DevIpBuf[][4], UINT16 BufSize, UINT16 DevName, UINT8 RedOrBlue);

UINT8 dsuGetGateWayByName(UINT8 DevGateWayBuf[],UINT16 DeviceName,UINT8 RedOrBlue);

/*ͨ���豸���Ʋ�ѯ����UDP����Դ�˿ں�
*�������: UINT16 DevName �豸����
*���������UINT16* SourcePort :UDP����Դ�˿ں�
*����ֵ����ѯ�ɹ�����1�����򷵻�0
*/
UINT8 dsuGetRedSourcePortByName(UINT16 DevName,UINT16* SourcePort);

/*ͨ���豸���Ʋ�ѯ����UDP����Դ�˿ں�
*�������: UINT16 DevName �豸����
*���������UINT16* SourcePort :UDP����Դ�˿ں�
*����ֵ����ѯ�ɹ�����1�����򷵻�0
*/
UINT8 dsuGetBlueSourcePortByName(UINT16 DevName,UINT16* SourcePort);

/*��������: ���ʽӿ�
* �� �� ֵ��1���ɹ�
*           0��ʧ��
* ��������:201701
* ������:zlj
*/
UINT8 dsuCcovIpInit(UINT8*FSName);


#ifdef __cplusplus
}
#endif

#endif

