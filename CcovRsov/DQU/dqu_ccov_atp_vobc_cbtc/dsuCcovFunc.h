
/************************************************************************
* 文件名	： dsuCcovFunc.h
* 版权说明	：	
* 版本号	： 1.0  
* 创建时间	： 2009.9.11
* 作者		： 车载与协议部
* 功能描述	： DSU为CCOV提供的查询函数 
* 使用注意	： 
* 修改记录	： 

************************************************************************/
#ifndef _DSU_CCOV_FUNC_H
#define _DSU_CCOV_FUNC_H

#include "CommonTypes.h"
#include "IntegrationConfig.h"
#include "CommonTypes.h"
#include "dquCbtcType.h"

#define DSURP_LEN_IP_ADDR 4              /*IP地址长度*/

 /*源自FAO由15改为30，BY LJ,20170109*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO 30 /*zcdsu通信对象最大数量*/
#define DSU_MAX_ZCDSU_COMM_OBJ_NUM 15     /*zcdsu通信对象最大数量*/

/*CCOV红蓝网IP宏定义*/
#define CCOV_RED_IP  0x01
#define CCOV_BLUE_IP 0x02
#define CCOV_UNKNOWN_IP	0xFF

#ifdef __cplusplus
extern "C" {
#endif

/*RpGetLnkInfo函数使用的结构体*/
typedef struct {
	UINT8 DestIPRed[DSURP_LEN_IP_ADDR];/* 接受方红网IP地址*/
	UINT16 DestPortRed;/* 接受方红网端口*/
	UINT8 DestIPBlue[DSURP_LEN_IP_ADDR];/* 接受方蓝网IP地址*/
	UINT16 DestPortBlue;/* 接受方蓝网端口*/
	UINT8 LocalIPRed[DSURP_LEN_IP_ADDR];/* 发送方红网IP地址*/
	UINT16 LocalPortRed;/* 发送方红网端口*/
	UINT8 LocalIPBlue[DSURP_LEN_IP_ADDR];/* 发送方蓝网IP地址*/
	UINT16 LocalPortBlue;/* 发送方蓝网端口*/
} DSU_RP_LINK_INFO_STRU;

/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/
/*ip地址结构体*/
#pragma pack(2)  /*指定按2字节对齐*/

typedef struct
{
	UINT16 DevName;        /*设备标识符*/
	UINT32 HLHTId; 			/*互联互通ID*/	
	UINT8 IpAddrBlue[DSURP_LEN_IP_ADDR];      /*蓝网ip地址*/
	UINT8 IpAddrRed[DSURP_LEN_IP_ADDR];       /*红网ip地址*/

	UINT16 SfpBluePort;    /*Sfp本地蓝网端口号*/
	UINT16 SfpRedPort;     /*Sfp本地红网端口号*/

	UINT16 RpBluePort;     /*Rp本地蓝网端口号*/
	UINT16 RpRedPort;     /*Rp本地红网端口号*/

	UINT16 RsspBluePort;     /*Rssp本地蓝网端口号*/
	UINT16 RsspRedPort;     /*Rssp本地红网端口号*/

	UINT16 BlueNetSourcePort;  /*蓝网UDP发送源端口*/
	UINT16 RedNetSourcePort;   /*红网UDP发送源端口*/

}DSU_IP_INFO_STRU;
#pragma pack()   /*取消指定对齐，恢复缺省对齐*/


/*设备类型与通信协议关系表*/
typedef struct  
{
	UINT8 EmitterType;        /*发送设备类型*/
	UINT8 ReceptorType;       /*接受设备类型*/

	UINT8 CommuType;          /*所使用的通信协议，1为Sfp，2为Rp，3为Rssp*/
}DSU_COMM_PROTCL_STRU;


/*CCOV通信关系配置表信息*/
typedef struct  
{
	UINT16 DeviceName_VOBC;	/*2	VOBC设备类型+设备ID*/
	UINT16 	DeviceName_WGB;	/*2	WGB设备类型+设备ID*/
	UINT16 	DeviceName_TimeServer;	/*2	信号系统时钟服务器设备类型+设备ID*/
	UINT16 	DeviceName_NMS;	/*2	NMS设备类型+设备ID*/
	UINT16 	NMS_BluePort;	/*2	CCOV与NMS通信蓝网端口*/
	UINT16 	NMS_RedPort;	/*2	CCOV与NMS通信红网端口*/

}DSU_CCOV_COMM_INFO_STRU;

/*ZCDSU通信关系配置表信息*/
typedef struct  
{
	UINT16 LocalDevName;    /*本设备编号	2	包含设备类型与ID*/
	UINT16 CommObjNum;	    /*通信对象数量	2	暂定最大数量为15*/
	UINT16 CommObjBuf[DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO];	/*通信对象1编号	2	包含设备类型与ID*/
}DSU_ZCDSU_COMM_INFO_STRU;

/*设备网关表内容结构体*/
typedef struct  
{
	UINT16  Index;	                                   /*2	编号*/
	UINT16  DeviceName;	                           		/*2	设备名称*/
	UINT8   IpAddrGatewayBlue[DSURP_LEN_IP_ADDR];      /*4	设备蓝网网关*/
	UINT8   IpAddrGatewayRed[DSURP_LEN_IP_ADDR];       /*4	设备红网网关*/
	UINT8   IpAddrSubnetMask[DSURP_LEN_IP_ADDR];       /*4	子网掩码*/
}DSU_GATEWAY_INFO_STRU;


typedef struct
{
	/*Ip地址和端口号信息数据长度和数据*/
	UINT16 IpInfoStruLen;      
	DSU_IP_INFO_STRU *pIpInfoStru;

    /*设备类型与通信协议关系表*/
	UINT16 CommProtclStruLen;
	DSU_COMM_PROTCL_STRU *pCommProtclStru;

	/*CCOV通信关系配置表信息*/
	UINT16 CcovCommInfoLen;
	DSU_CCOV_COMM_INFO_STRU* pCcovCommInfoStru;

	/*ZCDSU通信关系配置表信息*/
	UINT16 ZcdsuCommInfoLen;
	DSU_ZCDSU_COMM_INFO_STRU* pZcdsuCommInfoStru;
	
	/*设备网关表信息*/
	UINT16 GatewayInfoLen;
	DSU_GATEWAY_INFO_STRU* pGatewayInfoStru;

}DSU_BOOK_IP_INFO_STRU;

extern DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     


/* 日志接口 */
void dquLogMsg(INT32 level, const CHAR* fmt, ...);

/*DSU提供的查询CCOV红蓝网属性的函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*返回值：  1-红网; 2-蓝网; 0xff-无效
*/
UINT8 dsuGetCcovRedOrBlue(const UINT8 CcovIpBuf[]);

/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*		 WgbBufSize 通信控制器IP地址缓冲区大小
*输出参数：WgbIpBuf  WGB的IP地址
*返回值：  0 查询失败; 1查询成功
*/
UINT8 dsuWgbIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 WgbIpBuf[][4], UINT16 WgbBufSize);

/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*        size 通信控制器的IP地址缓冲区大小
*输出参数：NtpServerIpBuf  NTP服务器的IP地址
*返回值：  0 查询失败; 1查询成功
*/
UINT8 dsuNtpServerIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NtpServerIpBuf[][4], UINT16 size);

/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*输出参数：NmsIpBuf  车辆段网管系统的IP地址
UINT16* NmsPort，端口号
*返回值：  0 查询失败; 1查询成功
*/
UINT8 dsuNmsIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NmsIpBuf[],UINT16* NmsPort);

/*通过ip地址查询设备名称和红蓝网信息
*输入参数: UINT8 CcovIpBuf[]  设备ip地址
*输出参数：UINT16* DevName 设备名称
*返回值：  1-红网; 2-蓝网; 0xff-无效
*/
UINT8 dsuGetDevNameByIp(const UINT8 DevIpBuf[],UINT16* DevName);

/*通过设备名称和红蓝网查询设备ip地址
*输入参数:  UINT16 DevName 设备名称
			      UINT8 RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：UINT8 CcovIpBuf[]  设备ip地址
*返回值：  1-查询成功;  0-查询失败
*修改记录: 修复QC689,黄成,20180816;
*/
UINT8 dsuGetDevIpByName(UINT8 DevIpBuf[], UINT16 DevName, UINT8 RedOrBlue);

/*通过设备名称和红蓝网查询设备IP地址和端口号
*输入参数: UINT16 DevName 设备名称
*			    int nIPMaxCount:IP地址的最大数量
*输出参数：ipInfo 设备IP地址信息数组
*返回值：  找到的IP数量
*/
UINT8 dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, UINT16 DevName);

/*
* 功能描述：根据红网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
UINT8 dsuGetRedDevName(UINT8* ipAdd,UINT16 port,UINT16* devName) ;

/*
* 功能描述：根据蓝网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
UINT8 dsuGetBlueDevName(UINT8* ipAdd,UINT16 port,UINT16* devName) ;

/*通过设备名称和红蓝网查询设备所有的ip地址
*输入参数: UINT16 DevName 设备名称
UINT8 RedOrBlue 1-红网; 2-蓝网; 0xff-无效
* UINT16 BufSize  设备IP地址缓冲区大小
*输出参数：UINT8 CcovIpBuf[]  设备ip地址
*返回值：  找到的IP地址的数量
*/
UINT8 dsuGetAllDevIpByName(UINT8 DevIpBuf[][4], UINT16 BufSize, UINT16 DevName, UINT8 RedOrBlue);

UINT8 dsuGetGateWayByName(UINT8 DevGateWayBuf[],UINT16 DeviceName,UINT8 RedOrBlue);

/*通过设备名称查询红网UDP发送源端口号
*输入参数: UINT16 DevName 设备名称
*输出参数：UINT16* SourcePort :UDP发送源端口号
*返回值：查询成功返回1，否则返回0
*/
UINT8 dsuGetRedSourcePortByName(UINT16 DevName,UINT16* SourcePort);

/*通过设备名称查询蓝网UDP发送源端口号
*输入参数: UINT16 DevName 设备名称
*输出参数：UINT16* SourcePort :UDP发送源端口号
*返回值：查询成功返回1，否则返回0
*/
UINT8 dsuGetBlueSourcePortByName(UINT16 DevName,UINT16* SourcePort);

/*功能描述: 访问接口
* 返 回 值：1，成功
*           0，失败
* 创建日期:201701
* 创建人:zlj
*/
UINT8 dsuCcovIpInit(UINT8*FSName);


#ifdef __cplusplus
}
#endif

#endif

