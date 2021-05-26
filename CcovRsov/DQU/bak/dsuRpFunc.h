/************************************************************************
*@ �ļ���	    �� dsuCcovFunc.h
*@ ��Ȩ˵��	��
*@ �汾��	    �� 1.0
*@ ����ʱ��	�� 2020.4
*@ ����		    �� HHY
*@ ��������	�� ������ѯ����
*@ ʹ��ע��	��
*@ �޸ļ�¼	��

************************************************************************/

#ifndef _DSU_RP_FUNC_H_
#define _DSU_RP_FUNC_H_

#include "common/CommonTypes.h"
#include "dquCbtcType.h"

#define DSURP_LEN_IP_ADDR 4              /*IP��ַ����*/

/*FAO��HLT���ã�BY LJ,20161210*/
#define ZHUJI_1  '1'    /*�߼���ַ������1*/
#define ZHUJI_2  '2'    /*�߼���ַ������2*/
#define ZHUJI_3  '3'    /*�߼���ַ������3*/
#define ZHUJI_4  '4'    /*�߼���ַ������4*/
#define ZHUJI_5  '5'    /*�߼���ַ��ͨ�ſ�����1������*/
#define ZHUJI_6  '6'    /*�߼���ַ��ͨ�ſ�����2������*/
#define FTSM_1   '7'    /*�߼���ַ��FTSM 1*/
#define FTSM_2   '8'    /*�߼���ַ��FTSM 2*/

/*Դ��FAO��15��Ϊ30��BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO 30 /*zcdsuͨ�Ŷ����������*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM 15     /*zcdsuͨ�Ŷ����������*/


	/*RpGetLnkInfo����ʹ�õĽṹ��*/
	typedef struct {
		unsigned char DestIPRed[DSURP_LEN_IP_ADDR];/* ���ܷ�����IP��ַ*/
		unsigned short DestPortRed;/* ���ܷ������˿�*/
		unsigned char DestIPBlue[DSURP_LEN_IP_ADDR];/* ���ܷ�����IP��ַ*/
		unsigned short DestPortBlue;/* ���ܷ������˿�*/
		unsigned char LocalIPRed[DSURP_LEN_IP_ADDR];/* ���ͷ�����IP��ַ*/
		unsigned short LocalPortRed;/* ���ͷ������˿�*/
		unsigned char LocalIPBlue[DSURP_LEN_IP_ADDR];/* ���ͷ�����IP��ַ*/
		unsigned short LocalPortBlue;/* ���ͷ������˿�*/
	} DSU_RP_LINK_INFO_STRU;

	/*���½ṹ�嶨������dsu���ݿ��е����ݴ洢�������ⷢ��*/
	/*ip��ַ�ṹ��*/
	typedef struct
	{
		unsigned short DevName;        /*�豸��ʶ��*/

		unsigned char IpAddrBlue[DSURP_LEN_IP_ADDR];      /*����ip��ַ*/
		unsigned char IpAddrRed[DSURP_LEN_IP_ADDR];       /*����ip��ַ*/

		unsigned short SfpBluePort;    /*Sfp���������˿ں�*/
		unsigned short SfpRedPort;     /*Sfp���غ����˿ں�*/

		unsigned short RpBluePort;     /*Rp���������˿ں�*/
		unsigned short RpRedPort;     /*Rp���غ����˿ں�*/

		unsigned short RsspBluePort;     /*Rssp���������˿ں�*/
		unsigned short RsspRedPort;     /*Rssp���غ����˿ں�*/


	}DSU_IP_INFO_STRU;

	/*�豸������ͨ��Э���ϵ��*/
	typedef struct
	{
		unsigned char EmitterType;        /*�����豸����*/
		unsigned char ReceptorType;       /*�����豸����*/

		unsigned char CommuType;          /*��ʹ�õ�ͨ��Э�飬1ΪSfp��2ΪRp��3ΪRssp*/
	}DSU_COMM_PROTCL_STRU;

	/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	typedef struct
	{
		unsigned short DeviceName_VOBC;	/*2	VOBC�豸����+�豸ID*/
		unsigned short 	DeviceName_WGB;	/*2	WGB�豸����+�豸ID*/
		unsigned short 	DeviceName_TimeServer;	/*2	�ź�ϵͳʱ�ӷ������豸����+�豸ID*/
		unsigned short 	DeviceName_NMS;	/*2	NMS�豸����+�豸ID*/
		unsigned short 	NMS_BluePort;	/*2	CCOV��NMSͨ�������˿�*/
		unsigned short 	NMS_RedPort;	/*2	CCOV��NMSͨ�ź����˿�*/

	}DSU_CCOV_COMM_INFO_STRU;

	/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	typedef struct
	{
		unsigned short LocalDevName;    /*���豸���	2	�����豸������ID*/
		unsigned short CommObjNum;	    /*ͨ�Ŷ�������	2	�ݶ��������Ϊ15*/
		unsigned short CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO];	/*ͨ�Ŷ���1���	2	�����豸������ID*/
	}DSU_ZCDSU_COMM_INFO_STRU;

	typedef struct
	{
		/*Ip��ַ�Ͷ˿ں���Ϣ���ݳ��Ⱥ�����*/
		unsigned short IpInfoStruLen;
		DSU_IP_INFO_STRU *pIpInfoStru;

		/*�豸������ͨ��Э���ϵ��*/
		unsigned short CommProtclStruLen;
		DSU_COMM_PROTCL_STRU *pCommProtclStru;

		/*CCOVͨ�Ź�ϵ���ñ���Ϣ*/
		unsigned short CcovCommInfoLen;
		DSU_CCOV_COMM_INFO_STRU* pCcovCommInfoStru;

		/*ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
		unsigned short ZcdsuCommInfoLen;
		DSU_ZCDSU_COMM_INFO_STRU* pZcdsuCommInfoStru;

	}DSU_BOOK_IP_INFO_STRU;

	extern DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;


	/*
	* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
	* ���������pDsuIpInfoStru �ֽ�����ͷָ��
	* ���������pDsuIpInfoStru ��ʼ���������
	* �� �� ֵ��1���ɹ�
	*           0��ʧ�ܣ������д���
	*/
	unsigned char dsuIpInit(char* FSName);

	/*
	* �����������ú������ڻ�ȡ�����ͨ�Žڵ��������Ϣ���ú���ר��������㡣
	* ���������unsigned short LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
	*           unsigned short DestDevName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
	* ���������DSU_RP_LINK_INFO_STRU* pRpLnkInfo,����������ͨ�Žڵ���Ϣ,���в�����Ӧ���
	* �� �� ֵ��1���ɹ�
	*           0��ʧ��
	*/
	unsigned char dsuRpGetLnkInfo(unsigned short LocalName, unsigned short DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo);

	/*
	* ������������ȡ�豸����
	* ���������unsigned short DevName �豸��
	* ���������
	* �� �� ֵ���豸����
	*/
	unsigned char dsuRpGetDevType(unsigned short DevName);

	/*
	* ������������ѯ�豸IP
	* ���������unsigned short wLocalID���豸id
	unsigned char chLocalType���豸����
	* ���������unsigned int * RedIPInfo������ip
	unsigned int * BlueIPInfo������ip
	* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
	*/
	unsigned char CheckIPInfoOfObject(unsigned short wLocalID, unsigned char chLocalType, unsigned int * RedIPInfo, unsigned int * BlueIPInfo);

	/*
	* �������������뱾ZC��ID�������ZC����ͨ�Ŷ�����Ϣ��������ZC��DSU��ATS��CI��ʱ�ӷ������ȡ�
	* ���������unsigned short wLocalID�����豸id
	unsigned char chLocalType���豸����
	* ���������unsigned int * wObjectNum, ?ͨ�Ŷ�������
	unsigned short *pwObjectInfo,ͨ�Ŷ�������Ϣ������ID��Ϣ��������Ϣ
	* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
	*/
	unsigned char CheckCommObjectInfo(unsigned short wLocalID, unsigned char chLocalType, unsigned int * wObjectNum, unsigned short *pwObjectInfo);

	void dsuRpTypeId2DevName(unsigned char Type, unsigned char Id, unsigned short* pDevName);

	unsigned char dsurRpGetProtclType(unsigned short LocalName, unsigned short DestName, unsigned char* protclTye);


#endif
