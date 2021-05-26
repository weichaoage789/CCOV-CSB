/************************************************************************
*@ 文件名	    ： dsuCcovFunc.h
*@ 版权说明	：
*@ 版本号	    ： 1.0
*@ 创建时间	： 2020.4
*@ 作者		    ： 
*@ 功能描述	： DSU为CCOV提供的查询函数
*@ 使用注意	：
*@ 修改记录	：

************************************************************************/

#ifndef _DSU_CCOV_FUNC_H
#define _DSU_CCOV_FUNC_H

#include "GlobalDefine.h"
#include "common/CommonTypes.h"
#include "dsuRpFunc.h"

/*CCOV红蓝网IP宏定义*/
#define CCOV_RED_IP  0x01
#define CCOV_BLUE_IP 0x02
#define CCOV_UNKNOWN_IP	0xFF


	/*设备网关表内容结构体*/
	typedef struct
	{
		unsigned short  Index;	                                   /*2	编号*/
		unsigned short  DeviceName;	                           /*2	设备名称*/
		unsigned char   IpAddrGatewayBlue[DSURP_LEN_IP_ADDR];      /*4	设备蓝网网关*/
		unsigned char   IpAddrGatewayRed[DSURP_LEN_IP_ADDR];       /*4	设备红网网关*/
		unsigned char   IpAddrSubnetMask[DSURP_LEN_IP_ADDR];       /*4	子网掩码*/
	}DSU_GATEWAY_INFO_STRU;

	/*设备网关表信息*/
	typedef struct
	{
		unsigned short GatewayInfoLen;
		DSU_GATEWAY_INFO_STRU* pGatewayInfoStru;

	}DSU_GATEWAY_SHEET_STRU;


	/* 日志接口 */
	void dquLogMsg(int level, const char* fmt, ...);

	/*DSU提供的查询CCOV红蓝网属性的函数
	*输入参数: CcovIpBuf 通信控制器的IP地址
	*返回值：  1-红网; 2-蓝网; 0xff-无效
	*/
	unsigned char dsuGetCcovRedOrBlue(const unsigned char CcovIpBuf[]);

	/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
	*输入参数: CcovIpBuf 通信控制器的IP地址
	*		 WgbBufSize 通信控制器IP地址缓冲区大小
	*输出参数：WgbIpBuf  WGB的IP地址
	*返回值：  0 查询失败; 1查询成功
	*/
	unsigned char dsuWgbIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char WgbIpBuf[][4], unsigned short WgbBufSize);

	/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
	*输入参数: CcovIpBuf 通信控制器的IP地址
	*        size 通信控制器的IP地址缓冲区大小
	*输出参数：NtpServerIpBuf  NTP服务器的IP地址
	*返回值：  0 查询失败; 1查询成功
	*/
	unsigned char dsuNtpServerIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NtpServerIpBuf[][4], unsigned short size);

	/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
	*输入参数: CcovIpBuf 通信控制器的IP地址
	*输出参数：NmsIpBuf  车辆段网管系统的IP地址
	unsigned short* NmsPort，端口号
	*返回值：  0 查询失败; 1查询成功
	*/
	unsigned char dsuNmsIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NmsIpBuf[], unsigned short* NmsPort);

	/*通过ip地址查询设备名称和红蓝网信息
	*输入参数: unsigned char CcovIpBuf[]  设备ip地址
	*输出参数：unsigned short* DevName 设备名称
	*返回值：  1-红网; 2-蓝网; 0xff-无效
	*/
	unsigned char dsuGetDevNameByIp(const unsigned char DevIpBuf[], unsigned short* DevName);

	/*通过设备名称和红蓝网查询设备ip地址
	*输入参数: unsigned short DevName 设备名称
	unsigned char RedOrBlue 1-红网; 2-蓝网; 0xff-无效
	*输出参数：unsigned char CcovIpBuf[]  设备ip地址
	*返回值：  1-成功，0-失败
	*/
	unsigned char dsuGetDevIpByName(unsigned char DevIpBuf[], unsigned short DevName, unsigned char RedOrBlue);

	/*通过设备名称和红蓝网查询设备IP地址和端口号
	*输入参数: unsigned short DevName 设备名称
	*			    int nIPMaxCount:IP地址的最大数量
	*输出参数：ipInfo 设备IP地址信息数组
	*返回值：  找到的IP数量
	*/
	unsigned char dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, unsigned short DevName);

	/*
	* 功能描述：根据红网的IP地址和端口号获取设备名称
	* 输入参数：unsigned short port 端口号
	*           unsigned char* ipAdd  IP地址
	* 输出参数：devName 设备名称  unsigned short*
	* 返 回 值：1:正常返回 0：错误返回
	*/
	unsigned char dsuGetRedDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName);

	/*
	* 功能描述：根据蓝网的IP地址和端口号获取设备名称
	* 输入参数：unsigned short port 端口号
	*           unsigned char* ipAdd  IP地址
	* 输出参数：devName 设备名称  unsigned short*
	* 返 回 值：1:正常返回 0：错误返回
	*/
	unsigned char dsuGetBlueDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName);

	/*通过设备名称和红蓝网查询设备所有的ip地址
	*输入参数: unsigned short DevName 设备名称
	unsigned char RedOrBlue 1-红网; 2-蓝网; 0xff-无效
	* unsigned short BufSize  设备IP地址缓冲区大小
	*输出参数：unsigned char CcovIpBuf[]  设备ip地址
	*返回值：  找到的IP地址的数量
	*/
	unsigned char dsuGetAllDevIpByName(unsigned char DevIpBuf[][4], unsigned short BufSize, unsigned short DevName, unsigned char RedOrBlue);

	unsigned char dsuGetGateWayByName(unsigned char DevGateWayBuf[], unsigned short DeviceName, unsigned char RedOrBlue);

	unsigned char dsuGatewayInit(char*FSName);


#endif

