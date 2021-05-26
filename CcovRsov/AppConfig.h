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

#define PORT_50004			50004			/** < Rsov-Ccov通信端口 */

typedef struct
{
	unsigned char Ip[4];						/** <IP */

	unsigned char    NetType;								/** <表明本CCOV属于红网或蓝网:1为红网,2为蓝网,其他表示无效*/
	unsigned char    WgbIpBuff[4][4];					/** <WGB的IP地址信息*/
	unsigned char    NtpIpBuff[8][4];					/** <向NTP发送报文的IP地址信息*/
	unsigned char    NtpServerCount;					/** < NTP校时服务器的数量 */
	unsigned char    NmsIpBuff[4];						/** <向Nms发送报文的IP地址信息*/
	unsigned short   NmsPort;								/** <NMS的端口信息*/
	unsigned char    CcovFsVer[4];						/** <Ccov配置文件4字节版本号信息*/
	unsigned char    CcovIpVer[4];						/** <Ccov设备IP表4字节版本号信息,用于与Atp进行比较*/
	unsigned char    VersionCompRst;					/** <版本号比较结果信息*/

	unsigned int   CcovToAtpType;						/** <ccov向ATP汇报自身工作状态时所加的内网类型,根据红、蓝网而定*/
	unsigned int   CcovToRsovType;						/** <ccov向RSOV汇报自身工作状态时所加的内网类型,根据红、蓝网而定*/
	unsigned char    RadioTestType;					/** <无线测试报文类型*/
	unsigned char	 CcovToMmiType;					/** <通控发送给MMI版本号类型*/

	unsigned int   CcovSwVer;								/** <软件版本号*/
	unsigned short	 DataLenNum;						/** <记录数据长度值*/
	unsigned char	 ProtclType;							/** <配置所使用的协议类型，1:RSSP-1,2:RSSP-2*/
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