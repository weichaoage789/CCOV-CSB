#include "GlobalDefine.h"

#include <stdio.h>
#include <string.h>

#include "dsuRpFunc.h"
#include "dsuCcovFunc.h"
#include "stdarg.h"
#include "stdio.h"
#include "dfsDataRead.h"
#include "dquDataTypeDefine.h"

#define DSU_GATEWAY_INFO_STRU_SIZE 16


DSU_GATEWAY_SHEET_STRU DsuGatewaySheetStru;


/* 日志函数 */
void dquLogMsg(int level, const char* fmt, ...)
{
	char s[1000];
	va_list arg_ptr;
	if (level <= LOG_LEVEL)
	{
		va_start(arg_ptr, fmt);
		vsprintf(s, fmt, arg_ptr);
		va_end(arg_ptr);
	}
}


/*DSU提供的查询CCOV红蓝网属性的函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*返回值：  1-红网; 2-蓝网; 0xff-无效
*/
unsigned char dsuGetCcovRedOrBlue(const unsigned char CcovIpBuf[])
{
	/*暂时返回1表示红网,待DSU根据规则提供查询函数*/
	unsigned short i = 0;/*循环变量*/
	unsigned short j = 0;/*循环变量*/
	unsigned char bReturnValue;/*函数返回值*/
	DSU_IP_INFO_STRU* pCcovConfigStru;/*CCOV数据结构体信息*/

									  /*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历蓝网ip*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
		{
			if (pCcovConfigStru->IpAddrBlue[j] == CcovIpBuf[j])
			{
				/*若本字节ip地址相等，那么对比下一字节，直到结束。*/
			}
			else
			{
				break;
			}
		}
		/*与其中一个IP相符，函数返回蓝网ip*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			bReturnValue = CCOV_BLUE_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*没有符合的蓝网ip，就遍历红网ip*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
		{
			if (pCcovConfigStru->IpAddrRed[j] == CcovIpBuf[j])
			{
				/*若本字节ip地址相等，那么对比下一字节，直到结束。*/
			}
			else
			{
				break;
			}
		}
		/*与其中一个IP相符，函数返回红网ip*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			bReturnValue = CCOV_RED_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	/*红网和篮网ip均不符合，返回无效值*/
	bReturnValue = 0xff;
	return bReturnValue;
}

/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*输出参数：WgbIpBuf  WGB的IP地址
*返回值：  找到的Wgb设备的IP地址的数量
*/
unsigned char dsuWgbIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char WgbIpBuf[][4], unsigned short WgbBufSize)
{
	/*仅在上电时查询一次*/
	unsigned short i = 0;/*循环变量*/
	unsigned char bReturnValue;/*函数返回值*/
	unsigned char bRedOrBlueIP;/*红蓝网IP标志变量*/
	unsigned short tempVobcName; /*当前ccov所在vobc的名称*/
	unsigned short tempWgbName = 0;  /*wgb名称*/

							 /*根据当前ccov ip查询vobcName和红蓝网信息*/
	bRedOrBlueIP = dsuGetDevNameByIp(CcovIpBuf, &tempVobcName);
	if ((bRedOrBlueIP != CCOV_RED_IP) && (bRedOrBlueIP != CCOV_BLUE_IP))
	{
		/*查询vobcName失败，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*查询vobcName成功，继续执行*/
	}

	/*根据vobcName查询对应wgbName*/
	for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC == tempVobcName)
		{
			tempWgbName = DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB;
			break;
		}
		else
		{
			/*继续循环*/
		}
	}

	if (i == DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*查询wgbName失败，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*查询成功，继续执行*/
	}

	/*根据wgbName和红蓝网信息查询wgb设备ip*/
	bReturnValue = dsuGetAllDevIpByName(WgbIpBuf, 4, tempWgbName, bRedOrBlueIP);

	return bReturnValue;
}

/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*输出参数：NtpServerIpBuf  NTP服务器的IP地址
*返回值：  找到NTP服务器IP地址的数量
*/
unsigned char dsuNtpServerIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NtpServerIpBuf[][4], unsigned short size)
{
	/*仅在上电时查询一次*/
	unsigned short i = 0;/*循环变量*/
	unsigned char bReturnValue = 0;/*函数返回值*/
	unsigned char bRedOrBlueIP = 0;/*红蓝网IP标志变量*/
	unsigned char TempRet = 0;  /*临时返回值变量*/
	unsigned short tempVobcName = 0; /*当前ccov所在vobc的名称*/
	unsigned short tempNtpName = 0;  /*Ntp名称*/

							 /*根据当前ip查询vobcName和红蓝网信息*/
	bRedOrBlueIP = dsuGetDevNameByIp(CcovIpBuf, &tempVobcName);
	if ((bRedOrBlueIP != CCOV_RED_IP) && (bRedOrBlueIP != CCOV_BLUE_IP))
	{
		/*查询vobcName失败，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*查询vobcName成功，继续执行*/
	}

	/*根据vobcName查询NtpName*/
	for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC == tempVobcName)
		{
			tempNtpName = DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer;
			break;
		}
		else
		{
			/*继续循环*/
		}
	}

	if (i == DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*查询NtpName失败，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*查询成功，继续执行*/
	}

	/*根据NtpName和红蓝网信息查询设备ip*/
	TempRet = dsuGetAllDevIpByName(NtpServerIpBuf, size, tempNtpName, bRedOrBlueIP);

	return TempRet;
}


/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*输出参数：NmsIpBuf  车辆段网管系统的IP地址
unsigned short* NmsPort，端口号
*返回值：  0 查询失败; 1查询成功
*/
unsigned char dsuNmsIpGetByCcovIp(const unsigned char CcovIpBuf[], unsigned char NmsIpBuf[], unsigned short* NmsPort)
{
	/*仅在上电时查询一次*/
	unsigned short i = 0;/*循环变量*/
	unsigned char bReturnValue;/*函数返回值*/
	unsigned char bRedOrBlueIP;/*红蓝网IP标志变量*/
	unsigned char TempRet;  /*临时返回值变量*/
	unsigned short tempVobcName; /*当前ccov所在vobc的名称*/
	unsigned short tempNmsName = 0;  /*Nms名称*/

							 /*根据当前ip查询vobcName和红蓝网信息*/
	bRedOrBlueIP = dsuGetDevNameByIp(CcovIpBuf, &tempVobcName);
	if ((bRedOrBlueIP != CCOV_RED_IP) && (bRedOrBlueIP != CCOV_BLUE_IP))
	{
		/*查询vobcName失败，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*查询vobcName成功，继续执行*/
	}

	/*根据vobcName查询NmsName*/
	for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC == tempVobcName)
		{
			/*nmsName赋值*/
			tempNmsName = DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS;
			if (bRedOrBlueIP == CCOV_RED_IP)
			{
				/*端口赋值，当前为红网*/
				*NmsPort = DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort;
			}
			else
			{
				/*端口赋值，当前为蓝网*/
				*NmsPort = DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort;
			}
			break;
		}
		else
		{
			/*继续循环*/
		}
	}

	if (i == DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*查询NmsName失败，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*查询成功，继续执行*/
	}

	/*根据NmsName和红蓝网信息查询设备ip*/
	TempRet = dsuGetDevIpByName(NmsIpBuf, tempNmsName, bRedOrBlueIP);

	if (TempRet == 1)
	{
		/*查询成功*/
		bReturnValue = 1;
	}
	else
	{
		/*查询失败*/
		bReturnValue = 0;
	}

	return bReturnValue;
}

/*通过ip地址查询设备名称和红蓝网信息
*输入参数: unsigned char CcovIpBuf[]  设备ip地址
*输出参数：unsigned short* DevName 设备名称
*返回值：  1-红网; 2-蓝网; 0xff-无效
*/
unsigned char dsuGetDevNameByIp(const unsigned char DevIpBuf[], unsigned short* DevName)
{
	unsigned short i = 0;
	unsigned short j = 0;
	unsigned char bReturnValue;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历蓝网ip*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
		{
			if (pCcovConfigStru->IpAddrBlue[j] == DevIpBuf[j])
			{
				/*若本字节ip地址相等，那么对比下一字节，直到结束。*/
			}
			else
			{
				break;
			}
		}
		/*与其中一个IP相符，函数返回*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			*DevName = pCcovConfigStru->DevName;
			bReturnValue = CCOV_BLUE_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;
	/*没有符合的蓝网ip，就遍历红网ip*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		for (j = 0; j<DSURP_LEN_IP_ADDR; j++)
		{
			if (pCcovConfigStru->IpAddrRed[j] == DevIpBuf[j])
			{
				/*若本字节ip地址相等，那么对比下一字节，直到结束。*/
			}
			else
			{
				break;
			}
		}
		/*与其中一个IP相符，函数返回*/
		if (DSURP_LEN_IP_ADDR == j)
		{
			*DevName = pCcovConfigStru->DevName;
			bReturnValue = CCOV_RED_IP;
			return bReturnValue;
		}
		else
		{
			pCcovConfigStru++;
		}
	}

	/*红网和蓝网ip均不符合，返回无效值*/
	bReturnValue = 0xff;
	return bReturnValue;
}

/*通过设备名称和红蓝网查询设备IP地址和端口号
*输入参数: unsigned short DevName 设备名称
*			int nIPMaxCount:IP地址的最大数量
*输出参数：ipInfo 设备IP地址信息数组
*返回值：  找到的IP数量
*/
unsigned char dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, unsigned short DevName)
{
	unsigned short i = 0;   /* 临时循环变量 */
	int	nIPCount = 0;  /* IP地址数量 */
	DSU_IP_INFO_STRU* pCcovConfigStru = nullptr;  /* IP地址信息指针 */

											   /*入口参数检查*/
	if (nullptr == ipInfo)
	{
		/* 如果参数无效，返回-1 */
		return 0;
	}
	else
	{
		/* 入口参数有效，不需要额外处理 */
	}

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历设备ip地址表*/
	for (i = 0; i < DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (nIPCount < nIPMaxCount)
		{
			/*
			printf("pCcovConfigStru->DevName = 0x%.4x\n", pCcovConfigStru->DevName);
			printf("DevName = 0x%.4x\n", DevName);
			*/
			if (pCcovConfigStru->DevName == DevName)
			{
				/* 获取当前IP信息 */
				memcpy(&ipInfo[nIPCount], pCcovConfigStru, sizeof(DSU_IP_INFO_STRU));

				/*已找到设备，返回成功*/
				nIPCount++;
			}
			else
			{
				/*未找到设备名称，继续循环*/
			}
		}

		/*查询指针后移*/
		pCcovConfigStru++;
	}

	/* 返回找到的IP地址数量 */
	return nIPCount;
}

/*通过设备名称和红蓝网查询设备ip地址
*输入参数: unsigned short DevName 设备名称
unsigned char RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：unsigned char CcovIpBuf[]  设备ip地址
*返回值：  1-成功，0-失败
*/
unsigned char dsuGetDevIpByName(unsigned char DevIpBuf[], unsigned short DevName, unsigned char RedOrBlue)
{
	unsigned short i = 0;
	unsigned char bReturnValue;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*入口参数检查*/
	if ((RedOrBlue == CCOV_RED_IP) || (RedOrBlue == CCOV_BLUE_IP))
	{
		/*红蓝网参数有效，继续执行*/
	}
	else
	{
		/*输入参数无效，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历设备ip地址表*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (pCcovConfigStru->DevName == DevName)
		{
			/*已找到设备名称*/
			if (RedOrBlue == CCOV_RED_IP)
			{
				/*当前红网*/
				memcpy(DevIpBuf, pCcovConfigStru->IpAddrRed, DSURP_LEN_IP_ADDR);
			}
			else
			{
				/*当前蓝网*/
				memcpy(DevIpBuf, pCcovConfigStru->IpAddrBlue, DSURP_LEN_IP_ADDR);
			}

			/*已找到设备，返回成功*/
			bReturnValue = 1;
			return bReturnValue;
		}
		else
		{
			/*未找到设备名称，继续循环*/
		}

		/*查询指针后移*/
		pCcovConfigStru++;
	}

	/*没有找到devName，返回查询失败*/
	bReturnValue = 0;
	return bReturnValue;
}

/*通过设备名称和红蓝网查询设备所有的ip地址
*输入参数: unsigned short DevName 设备名称
*unsigned short BufSize:IP地址缓冲区大小
unsigned char RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：unsigned char CcovIpBuf[]  设备ip地址
*返回值：  找到的IP地址的数量,0未查找到,非0查找成功(设备IP的数量)
*/
unsigned char dsuGetAllDevIpByName(unsigned char DevIpBuf[][4], unsigned short BufSize, unsigned short DevName, unsigned char RedOrBlue)
{
	unsigned short i = 0;
	unsigned char bReturnValue;
	unsigned char Index = 0;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*入口参数检查*/
	if ((RedOrBlue == CCOV_RED_IP) || (RedOrBlue == CCOV_BLUE_IP))
	{
		/*红蓝网参数有效，继续执行*/
	}
	else
	{
		/*输入参数无效，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历设备ip地址表*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (pCcovConfigStru->DevName == DevName)
		{
			if (Index<BufSize)
			{
				/*已找到设备名称*/
				if (RedOrBlue == CCOV_RED_IP)
				{
					/*当前红网*/
					memcpy(DevIpBuf[Index], pCcovConfigStru->IpAddrRed, DSURP_LEN_IP_ADDR);
				}
				else
				{
					/*当前蓝网*/
					memcpy(DevIpBuf[Index], pCcovConfigStru->IpAddrBlue, DSURP_LEN_IP_ADDR);
				}

				Index++;
			}
		}
		else
		{
			/*未找到设备名称，继续循环*/
		}

		/*查询指针后移*/
		pCcovConfigStru++;
	}

	/*没有找到devName，返回值查找到的数量*/
	return Index;
}

/*
* 功能描述：根据红网的IP地址和端口号获取设备名称
* 输入参数：unsigned short port 端口号
*           unsigned char* ipAdd  IP地址
* 输出参数：devName 设备名称  unsigned short*
* 返 回 值：1:正常返回 0：错误返回
*/
unsigned char dsuGetRedDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName)
{
	unsigned char retnVal = 0;
	unsigned short i = 0;/* 循环使用 */
	unsigned char flag = 0;
	unsigned short ipInfoLen = 0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen = DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru = DsuBookIpInfoStru.pIpInfoStru;

	for (i = 0; i<ipInfoLen; i++)
	{
		if ((ipAdd[0] == pTempIpInfoStru[i].IpAddrRed[0]) && (ipAdd[1] == pTempIpInfoStru[i].IpAddrRed[1]) &&
			(ipAdd[2] == pTempIpInfoStru[i].IpAddrRed[2]) && (ipAdd[3] == pTempIpInfoStru[i].IpAddrRed[3]))
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}
		if ((flag == 1) && ((port == pTempIpInfoStru[i].RpRedPort) || (port == pTempIpInfoStru[i].RsspRedPort) ||
			(port == pTempIpInfoStru[i].SfpRedPort)))
		{
			*devName = pTempIpInfoStru[i].DevName;
			retnVal = 1;
			break;
		}
		else
		{
			retnVal = 0;
		}
	}

	return retnVal;
}

/*
* 功能描述：根据蓝网的IP地址和端口号获取设备名称
* 输入参数：unsigned short port 端口号
*           unsigned char* ipAdd  IP地址
* 输出参数：devName 设备名称  unsigned short*
* 返 回 值：1:正常返回 0：错误返回
*/
unsigned char dsuGetBlueDevName(unsigned char* ipAdd, unsigned short port, unsigned short* devName)
{
	unsigned char retnVal = 0;
	unsigned short i = 0;/* 循环使用 */
	unsigned char flag = 0;
	unsigned short ipInfoLen = 0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen = DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru = DsuBookIpInfoStru.pIpInfoStru;

	for (i = 0; i<ipInfoLen; i++)
	{
		if ((ipAdd[0] == pTempIpInfoStru[i].IpAddrBlue[0]) && (ipAdd[1] == pTempIpInfoStru[i].IpAddrBlue[1]) &&
			(ipAdd[2] == pTempIpInfoStru[i].IpAddrBlue[2]) && (ipAdd[3] == pTempIpInfoStru[i].IpAddrBlue[3]))
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}
		if ((flag == 1) && ((port == pTempIpInfoStru[i].RpBluePort) || (port == pTempIpInfoStru[i].RsspBluePort) ||
			(port == pTempIpInfoStru[i].SfpBluePort)))
		{
			*devName = pTempIpInfoStru[i].DevName;
			retnVal = 1;
			break;
		}
		else
		{
			retnVal = 0;
		}
	}

	return retnVal;
}

/*通过设备名称和红蓝网查询设备网关
*输入参数: unsigned short DevName 设备名称
unsigned char RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：unsigned char DevGateWayBuf[] 设备网关地址
*返回值：  1-成功，0-失败
*/
unsigned char dsuGetGateWayByName(unsigned char DevGateWayBuf[], unsigned short DeviceName, unsigned char RedOrBlue)
{
	unsigned short i = 0;
	unsigned char bReturnValue;
	DSU_GATEWAY_INFO_STRU* pCcovConfigStru = nullptr;

	/*入口参数检查*/
	if ((RedOrBlue == CCOV_RED_IP) || (RedOrBlue == CCOV_BLUE_IP))
	{
		/*红蓝网参数有效，继续执行*/
	}
	else
	{
		/*输入参数无效，函数返回失败*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuGatewaySheetStru.pGatewayInfoStru;

	/*遍历设备ip地址表*/
	for (i = 0; i<DsuGatewaySheetStru.GatewayInfoLen; i++)
	{
		if (pCcovConfigStru->DeviceName == DeviceName)
		{
			/*已找到设备名称*/
			if (RedOrBlue == CCOV_RED_IP)
			{
				/*当前红网*/
				memcpy(DevGateWayBuf, pCcovConfigStru->IpAddrGatewayRed, DSURP_LEN_IP_ADDR);
			}
			else
			{
				/*当前蓝网*/
				memcpy(DevGateWayBuf, pCcovConfigStru->IpAddrGatewayBlue, DSURP_LEN_IP_ADDR);
			}
			//logMsg("IPPP=%d,%d,%d,%d\n", DevGateWayBuf[0], DevGateWayBuf[1], DevGateWayBuf[2], DevGateWayBuf[3], 0, 0);
			/*已找到设备，返回成功*/
			bReturnValue = 1;
			return bReturnValue;
		}
		else
		{
			/*未找到设备名称，继续循环*/
		}

		/*查询指针后移*/
		pCcovConfigStru++;
	}

	/*没有找到devName，返回查询失败*/
	bReturnValue = 0;
	return bReturnValue;
}

/*
* 功能描述：设备网关表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：unsigned char* FSName， 文件名或者指针地址
pDsuIpInfoStru， 字节数据头指针
* 输出参数：pDsuIpInfoStru， 初始化后的数据
* 返 回 值：1，成功
*          			 0，失败，数据有错误
*/

unsigned char dsuGatewayInit(char*FSName)
{
	unsigned char chReturnValue = 1; /*返回值*/
	unsigned char tempRet;
	unsigned char* pData = nullptr;       /*用于读取数据的指针*/
	unsigned int dataLen = 0;        /*数据长度*/
	unsigned int dataOff = 0;        /*数据偏移量*/
	unsigned int i;

	/*有效性检查*/
	if (nullptr == FSName)
	{
		chReturnValue = 0;
	}

	/*初始化设备网关表信息*/
	/*读取数据*/
	tempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_GATEWAY_ID, &pData, &dataLen);
	if (tempRet != 1)
	{
		/*数据读取失败*/
		chReturnValue = 0;
	}
	else
	{
		/*继续执行*/
	}

	DsuGatewaySheetStru.GatewayInfoLen = (unsigned short)(dataLen / DSU_GATEWAY_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuGatewaySheetStru.pGatewayInfoStru = (DSU_GATEWAY_INFO_STRU*)malloc(sizeof(DSU_GATEWAY_INFO_STRU)*(DsuGatewaySheetStru.GatewayInfoLen)); /*为结构体申请空间*/
	if (DsuGatewaySheetStru.pGatewayInfoStru == nullptr)
	{
		/*空间分配失败*/
		chReturnValue = 0;
	}
	else
	{
		/*继续执行*/
		memset(DsuGatewaySheetStru.pGatewayInfoStru, 0, sizeof(DSU_GATEWAY_INFO_STRU)*(DsuGatewaySheetStru.GatewayInfoLen));
		dataOff = 0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i = 0; i<DsuGatewaySheetStru.GatewayInfoLen; i++)
		{
			/*编号 2*/
			dataOff += 2;

			/*设备名称2*/
			DsuGatewaySheetStru.pGatewayInfoStru[i].DeviceName = ShortFromChar(pData + dataOff);
			dataOff += 2;

			/*设备红网网关 4*/
			memcpy(DsuGatewaySheetStru.pGatewayInfoStru[i].IpAddrGatewayRed, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			/*设备蓝网网关 4*/
			memcpy(DsuGatewaySheetStru.pGatewayInfoStru[i].IpAddrGatewayBlue, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			/*子网掩码 4*/
			memcpy(DsuGatewaySheetStru.pGatewayInfoStru[i].IpAddrSubnetMask, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

		}

		if (dataOff != dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue = 0;
		}
		else
		{

		}
	}

	/*继续执行*/
	return chReturnValue;

}

