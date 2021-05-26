/**
  ******************************************************************************
  * @copyright	:
  * @file		        : AppConfig.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#include "dfsDataRead.h"
#include "dsuIdmapStruct.h"
#include "Ccov/SNMP/CcovSnmpConfig.h"

#define PORT_50004			50004			/** < Rsov-Ccovͨ�Ŷ˿� */

typedef struct
{
	unsigned char Ip[4];						/** <IP */

	unsigned char    NetType;								/** <������CCOV���ں���������:1Ϊ����,2Ϊ����,������ʾ��Ч*/
	unsigned char    WgbIpBuff[4][4];					/** <WGB��IP��ַ��Ϣ*/
	unsigned char    NtpIpBuff[8][4];					/** <��NTP���ͱ��ĵ�IP��ַ��Ϣ*/
	unsigned char    NtpServerCount;					/** < NTPУʱ������������ */
	unsigned char    NmsIpBuff[4];						/** <��Nms���ͱ��ĵ�IP��ַ��Ϣ*/
	unsigned short   NmsPort;								/** <NMS�Ķ˿���Ϣ*/
	unsigned char    CcovFsVer[4];						/** <Ccov�����ļ�4�ֽڰ汾����Ϣ*/
	unsigned char    CcovIpVer[4];						/** <Ccov�豸IP��4�ֽڰ汾����Ϣ,������Atp���бȽ�*/
	unsigned char    VersionCompRst;					/** <�汾�űȽϽ����Ϣ*/

	unsigned int   CcovToAtpType;						/** <ccov��ATP�㱨������״̬ʱ���ӵ���������,���ݺ졢��������*/
	unsigned int   CcovToRsovType;						/** <ccov��RSOV�㱨������״̬ʱ���ӵ���������,���ݺ졢��������*/
	unsigned char    RadioTestType;					/** <���߲��Ա�������*/
	unsigned char	 CcovToMmiType;					/** <ͨ�ط��͸�MMI�汾������*/

	unsigned int   CcovSwVer;								/** <����汾��*/
	unsigned short	 DataLenNum;						/** <��¼���ݳ���ֵ*/
	unsigned char	 ProtclType;							/** <������ʹ�õ�Э�����ͣ�1:RSSP-1,2:RSSP-2*/
}CCOV_ATTRI;


class CAppCfg
{
private:
	CAppCfg();
	CAppCfg(const CAppCfg&) = delete;
	CAppCfg& operator = (const CAppCfg&) = delete;

public:
	static CAppCfg &GetInstance()
	{
		static CAppCfg instance;
		return instance;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get CCOV/RSOV type.
	  * @return	 :  [type] unsigned int, 0 : RSOV; non-0 : CCOV
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetAppType()
	{
		return _AppType;
	}

	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  get product type.
	  * @return	 :  [type] unsigned int, product type: 510/500/300.
	  *-----------------------------------------------------------------------------
	  */
	unsigned int GetProducetType()
	{
		return _ProductType;
	}


protected:
	/**
	  *-----------------------------------------------------------------------------
	  * @brief		 :  initialize function.
	  * @return	 :  [type] 
	  *-----------------------------------------------------------------------------
	  */
	void init();


public:
	/** @application type */
	unsigned int _AppType;

	/** @product type */
	unsigned int _ProductType;

	/** @local ip address */
	char _LocalIp[32];

	/** @net mask */
	char	_NetMask[32];

	/** @gate way */
	char	_GateWay[32];

	/** @device name */
	unsigned short _DevName;

	/** @ip type */
	unsigned char _IpType;

	/** @CCOV attributes */
	CCOV_ATTRI _Attri;

	/** @LTE/WGB OID info */
	unsigned char _OIDInfo[2][64];

	/** @LTE/WGB REQ info */
	unsigned char _REQInfo;

	/** @LTE/WGB REQ */
	REQ_ARGV_STRU _ReqArgv;

	/** @DQU data storage info */
	DQU_READ_DATA_STOR_STRU _FsStorCfg;
};


#endif /*_APP_CONFIG_H*/