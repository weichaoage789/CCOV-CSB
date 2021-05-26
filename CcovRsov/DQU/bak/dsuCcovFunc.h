/************************************************************************
*@ �ļ���	    �� dsuCcovFunc.h
*@ ��Ȩ˵��	��
*@ �汾��	    �� 1.0
*@ ����ʱ��	�� 2020.4
*@ ����		    �� 
*@ ��������	�� DSUΪCCOV�ṩ�Ĳ�ѯ����
*@ ʹ��ע��	��
*@ �޸ļ�¼	��

************************************************************************/

#ifndef _DSU_CCOV_FUNC_H
#define _DSU_CCOV_FUNC_H

#include "GlobalDefine.h"
#include "common/CommonTypes.h"
#include "dsuRpFunc.h"

/*CCOV������IP�궨��*/
#define CCOV_RED_IP  0x01
#define CCOV_BLUE_IP 0x02
#define CCOV_UNKNOWN_IP	0xFF


	/*�豸���ر����ݽṹ��*/
	typedef struct
	{
		unsigned short  Index;	                                   /*2	���*/
		unsigned short  DeviceName;	                           /*2	�豸����*/
		unsigned char   IpAddrGatewayBlue[DSURP_LEN_IP_ADDR];      /*4	�豸��������*/
		unsigned char   IpAddrGatewayRed[DSURP_LEN_IP_ADDR];       /*4	�豸��������*/
		unsigned char   IpAddrSubnetMask[DSURP_LEN_IP_ADDR];       /*4	��������*/
	}DSU_GATEWAY_INFO_STRU;

	/*�豸���ر���Ϣ*/
	typedef struct
	{
		unsigned short GatewayInfoLen;
		DSU_GATEWAY_INFO_STRU* pGatewayInfoStru;

	}DSU_GATEWAY_SHEET_STRU;


	/* ��־�ӿ� */
	void dquLogMsg(int level, const char* fmt, ...);

	/*DSU�ṩ�Ĳ�ѯCCOV���������Եĺ���
	*�������: CcovIpBuf ͨ�ſ�������IP��ַ
	*����ֵ��  1-����; 2-����; 0xff-��Ч
	*/
	unsigned char dsuGetCcovRedOrBlue(const unsigned char CcovIpBuf[]);

	/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
	*�������: CcovIpBuf ͨ�ſ�������IP��ַ
	*		 WgbBufSize ͨ�ſ�����IP��ַ��������С
	*���������WgbIpBuf  WGB��IP��ַ
	*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
	*/
	unsigned char dsuWgbIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char WgbIpBuf[][4], unsigned short WgbBufSize);

	/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
	*�������: CcovIpBuf ͨ�ſ�������IP��ַ
	*        size ͨ�ſ�������IP��ַ��������С
	*���������NtpServerIpBuf  NTP��������IP��ַ
	*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
	*/
	unsigned char dsuNtpServerIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NtpServerIpBuf[][4], unsigned short size);

	/*DSU�ṩ����CCOV�����ӵ�WGB��IP��ַ��ѯ����
	*�������: CcovIpBuf ͨ�ſ�������IP��ַ
	*���������NmsIpBuf  ����������ϵͳ��IP��ַ
	unsigned short* NmsPort���˿ں�
	*����ֵ��  0 ��ѯʧ��; 1��ѯ�ɹ�
	*/
	unsigned char dsuNmsIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NmsIpBuf[], unsigned short* NmsPort);

	/*ͨ��ip��ַ��ѯ�豸���ƺͺ�������Ϣ
	*�������: unsigned char CcovIpBuf[]  �豸ip��ַ
	*���������unsigned short* DevName �豸����
	*����ֵ��  1-����; 2-����; 0xff-��Ч
	*/
	unsigned char dsuGetDevNameByIp(const unsigned char DevIpBuf[], unsigned short* DevName);

	/*ͨ���豸���ƺͺ�������ѯ�豸ip��ַ
	*�������: unsigned short DevName �豸����
	unsigned char RedOrBlue 1-����; 2-����; 0xff-��Ч
	*���������unsigned char CcovIpBuf[]  �豸ip��ַ
	*����ֵ��  1-�ɹ���0-ʧ��
	*/
	unsigned char dsuGetDevIpByName(unsigned char DevIpBuf[], unsigned short DevName, unsigned char RedOrBlue);

	/*ͨ���豸���ƺͺ�������ѯ�豸IP��ַ�Ͷ˿ں�
	*�������: unsigned short DevName �豸����
	*			    int nIPMaxCount:IP��ַ���������
	*���������ipInfo �豸IP��ַ��Ϣ����
	*����ֵ��  �ҵ���IP����
	*/
	unsigned char dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, unsigned short DevName);

	/*
	* �������������ݺ�����IP��ַ�Ͷ˿ںŻ�ȡ�豸����
	* ���������unsigned short port �˿ں�
	*           unsigned char* ipAdd  IP��ַ
	* ���������devName �豸����  unsigned short*
	* �� �� ֵ��1:�������� 0�����󷵻�
	*/
	unsigned char dsuGetRedDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName);

	/*
	* ��������������������IP��ַ�Ͷ˿ںŻ�ȡ�豸����
	* ���������unsigned short port �˿ں�
	*           unsigned char* ipAdd  IP��ַ
	* ���������devName �豸����  unsigned short*
	* �� �� ֵ��1:�������� 0�����󷵻�
	*/
	unsigned char dsuGetBlueDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName);

	/*ͨ���豸���ƺͺ�������ѯ�豸���е�ip��ַ
	*�������: unsigned short DevName �豸����
	unsigned char RedOrBlue 1-����; 2-����; 0xff-��Ч
	* unsigned short BufSize  �豸IP��ַ��������С
	*���������unsigned char CcovIpBuf[]  �豸ip��ַ
	*����ֵ��  �ҵ���IP��ַ������
	*/
	unsigned char dsuGetAllDevIpByName(unsigned char DevIpBuf[][4], unsigned short BufSize, unsigned short DevName, unsigned char RedOrBlue);

	unsigned char dsuGetGateWayByName(unsigned char DevGateWayBuf[], unsigned short DeviceName, unsigned char RedOrBlue);

	unsigned char dsuGatewayInit(char*FSName);


#endif

