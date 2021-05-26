/************************************************************************
*@ 文件名	    ： dsuCcovFunc.h
*@ 版权说明	：
*@ 版本号	    ： 1.0
*@ 创建时间	： 2020.4
*@ 作者		    ： HHY
*@ 功能描述	： 冗余层查询函数
*@ 使用注意	：
*@ 修改记录	：

************************************************************************/

#ifndef _DSU_RP_FUNC_H_
#define _DSU_RP_FUNC_H_

#include "common/CommonTypes.h"
#include "dquCbtcType.h"

#define DSURP_LEN_IP_ADDR 4              /*IP地址长度*/

/*FAO，HLT共用，BY LJ,20161210*/
#define ZHUJI_1  '1'    /*逻辑地址，主机1*/
#define ZHUJI_2  '2'    /*逻辑地址，主机2*/
#define ZHUJI_3  '3'    /*逻辑地址，主机3*/
#define ZHUJI_4  '4'    /*逻辑地址，主机4*/
#define ZHUJI_5  '5'    /*逻辑地址，通信控制器1，蓝网*/
#define ZHUJI_6  '6'    /*逻辑地址，通信控制器2，红网*/
#define FTSM_1   '7'    /*逻辑地址，FTSM 1*/
#define FTSM_2   '8'    /*逻辑地址，FTSM 2*/

/*源自FAO由15改为30，BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO 30 /*zcdsu通信对象最大数量*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM 15     /*zcdsu通信对象最大数量*/


	/*RpGetLnkInfo函数使用的结构体*/
	typedef struct {
		unsigned char DestIPRed[DSURP_LEN_IP_ADDR];/* 接受方红网IP地址*/
		unsigned short DestPortRed;/* 接受方红网端口*/
		unsigned char DestIPBlue[DSURP_LEN_IP_ADDR];/* 接受方蓝网IP地址*/
		unsigned short DestPortBlue;/* 接受方蓝网端口*/
		unsigned char LocalIPRed[DSURP_LEN_IP_ADDR];/* 发送方红网IP地址*/
		unsigned short LocalPortRed;/* 发送方红网端口*/
		unsigned char LocalIPBlue[DSURP_LEN_IP_ADDR];/* 发送方蓝网IP地址*/
		unsigned short LocalPortBlue;/* 发送方蓝网端口*/
	} DSU_RP_LINK_INFO_STRU;

	/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/
	/*ip地址结构体*/
	typedef struct
	{
		unsigned short DevName;        /*设备标识符*/

		unsigned char IpAddrBlue[DSURP_LEN_IP_ADDR];      /*蓝网ip地址*/
		unsigned char IpAddrRed[DSURP_LEN_IP_ADDR];       /*红网ip地址*/

		unsigned short SfpBluePort;    /*Sfp本地蓝网端口号*/
		unsigned short SfpRedPort;     /*Sfp本地红网端口号*/

		unsigned short RpBluePort;     /*Rp本地蓝网端口号*/
		unsigned short RpRedPort;     /*Rp本地红网端口号*/

		unsigned short RsspBluePort;     /*Rssp本地蓝网端口号*/
		unsigned short RsspRedPort;     /*Rssp本地红网端口号*/


	}DSU_IP_INFO_STRU;

	/*设备类型与通信协议关系表*/
	typedef struct
	{
		unsigned char EmitterType;        /*发送设备类型*/
		unsigned char ReceptorType;       /*接受设备类型*/

		unsigned char CommuType;          /*所使用的通信协议，1为Sfp，2为Rp，3为Rssp*/
	}DSU_COMM_PROTCL_STRU;

	/*CCOV通信关系配置表信息*/
	typedef struct
	{
		unsigned short DeviceName_VOBC;	/*2	VOBC设备类型+设备ID*/
		unsigned short 	DeviceName_WGB;	/*2	WGB设备类型+设备ID*/
		unsigned short 	DeviceName_TimeServer;	/*2	信号系统时钟服务器设备类型+设备ID*/
		unsigned short 	DeviceName_NMS;	/*2	NMS设备类型+设备ID*/
		unsigned short 	NMS_BluePort;	/*2	CCOV与NMS通信蓝网端口*/
		unsigned short 	NMS_RedPort;	/*2	CCOV与NMS通信红网端口*/

	}DSU_CCOV_COMM_INFO_STRU;

	/*ZCDSU通信关系配置表信息*/
	typedef struct
	{
		unsigned short LocalDevName;    /*本设备编号	2	包含设备类型与ID*/
		unsigned short CommObjNum;	    /*通信对象数量	2	暂定最大数量为15*/
		unsigned short CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO];	/*通信对象1编号	2	包含设备类型与ID*/
	}DSU_ZCDSU_COMM_INFO_STRU;

	typedef struct
	{
		/*Ip地址和端口号信息数据长度和数据*/
		unsigned short IpInfoStruLen;
		DSU_IP_INFO_STRU *pIpInfoStru;

		/*设备类型与通信协议关系表*/
		unsigned short CommProtclStruLen;
		DSU_COMM_PROTCL_STRU *pCommProtclStru;

		/*CCOV通信关系配置表信息*/
		unsigned short CcovCommInfoLen;
		DSU_CCOV_COMM_INFO_STRU* pCcovCommInfoStru;

		/*ZCDSU通信关系配置表信息*/
		unsigned short ZcdsuCommInfoLen;
		DSU_ZCDSU_COMM_INFO_STRU* pZcdsuCommInfoStru;

	}DSU_BOOK_IP_INFO_STRU;

	extern DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;


	/*
	* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
	* 输入参数：pDsuIpInfoStru 字节数据头指针
	* 输出参数：pDsuIpInfoStru 初始化后的数据
	* 返 回 值：1，成功
	*           0，失败，数据有错误
	*/
	unsigned char dsuIpInit(char* FSName);

	/*
	* 功能描述：该函数用于获取冗余层通信节点的数据信息。该函数专用于冗余层。
	* 输入参数：unsigned short LocalDevName, 发送方设备标识（Type在前，ID在后）
	*           unsigned short DestDevName,  接受方设备标识（Type在前，ID在后）
	* 输出参数：DSU_RP_LINK_INFO_STRU* pRpLnkInfo,冗余层所需的通信节点信息,所有参数都应输出
	* 返 回 值：1，成功
	*           0，失败
	*/
	unsigned char dsuRpGetLnkInfo(unsigned short LocalName, unsigned short DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo);

	/*
	* 功能描述：获取设备类型
	* 输入参数：unsigned short DevName 设备名
	* 输出参数：
	* 返 回 值：设备类型
	*/
	unsigned char dsuRpGetDevType(unsigned short DevName);

	/*
	* 功能描述：查询设备IP
	* 输入参数：unsigned short wLocalID，设备id
	unsigned char chLocalType，设备类型
	* 输出参数：unsigned int * RedIPInfo，红网ip
	unsigned int * BlueIPInfo，蓝网ip
	* 返 回 值：查询成功，返回1， 	查询失败，返回0。
	*/
	unsigned char CheckIPInfoOfObject(unsigned short wLocalID, unsigned char chLocalType, unsigned int * RedIPInfo, unsigned int * BlueIPInfo);

	/*
	* 功能描述：输入本ZC的ID，输出本ZC地面通信对象信息，包括：ZC、DSU、ATS、CI、时钟服务器等。
	* 输入参数：unsigned short wLocalID，本设备id
	unsigned char chLocalType，设备类型
	* 输出参数：unsigned int * wObjectNum, ?通信对象数量
	unsigned short *pwObjectInfo,通信对象编号信息，包括ID信息和类型信息
	* 返 回 值：查询成功，返回1， 	查询失败，返回0。
	*/
	unsigned char CheckCommObjectInfo(unsigned short wLocalID, unsigned char chLocalType, unsigned int * wObjectNum, unsigned short *pwObjectInfo);

	void dsuRpTypeId2DevName(unsigned char Type, unsigned char Id, unsigned short* pDevName);

	unsigned char dsurRpGetProtclType(unsigned short LocalName, unsigned short DestName, unsigned char* protclTye);


#endif
