

/************************************************************************
* 文件名	： dsuCcovFunc.c
* 版权说明	：	
* 版本号	： 1.0  
* 创建时间	： 2009.9.11
* 作者		： 系统集成部--高国栋
* 功能描述	： DSU为CCOV提供的查询函数 
* 使用注意	： 
* 修改记录	： 
2011.1.09,孙军国，
1）数据代码分离公关项目，将数据放到了数据文件中，而不是.h文件中
2）根据新设计的数据结构进行了修改
*20200825 lp 将原有RP协议中的FS读取初始化内容移到CCOV私有查询函数中
************************************************************************/
#include <stdio.h>
#include <string.h>
#include "dsuCcovFunc.h"
#include "stdarg.h"
#include "stdio.h"
#include "dquDataTypeDefine.h"

/*结构体内部数据类型总和*/
#define DSU_IP_INFO_STRU_SIZE 30  
#define DSU_COMM_PROTCL_STRU_SIZE 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE 12
/*源自FAO由34改为(34+30)，BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE (34)
#define DSU_GATEWAY_INFO_STRU_SIZE 16


DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     /*初始化之后用于保存初始化之后的数据的变量*/


/* 日志函数 */
void dquLogMsg(INT32 level, const CHAR* fmt, ...)
{
	INT8 s[1000];
	va_list arg_ptr;
	if(level<=LOG_LEVEL)
	{
		va_start(arg_ptr,fmt);
		vsprintf(s,fmt,arg_ptr);
		va_end(arg_ptr);
		//logMsg("%s",s);
	}
}


/*DSU提供的查询CCOV红蓝网属性的函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*返回值：  1-红网; 2-蓝网; 0xff-无效
*/
UINT8 dsuGetCcovRedOrBlue(const UINT8 CcovIpBuf[])
{
    /*暂时返回1表示红网,待DSU根据规则提供查询函数*/
	UINT16 i = 0;/*循环变量*/
	UINT16 j = 0;/*循环变量*/
	UINT8 bReturnValue;/*函数返回值*/
	DSU_IP_INFO_STRU* pCcovConfigStru;/*CCOV数据结构体信息*/

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历蓝网ip*/
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
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
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
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
UINT8 dsuWgbIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 WgbIpBuf[][4], UINT16 WgbBufSize)
{
	/*仅在上电时查询一次*/
	UINT16 i = 0;/*循环变量*/
	UINT8 bReturnValue;/*函数返回值*/
	UINT8 bRedOrBlueIP;/*红蓝网IP标志变量*/
	UINT16 tempVobcName; /*当前ccov所在vobc的名称*/
	UINT16 tempWgbName=0;  /*wgb名称*/

	/*根据当前ccov ip查询vobcName和红蓝网信息*/
	bRedOrBlueIP=dsuGetDevNameByIp(CcovIpBuf,&tempVobcName);
	if ((bRedOrBlueIP!=CCOV_RED_IP)&&(bRedOrBlueIP!=CCOV_BLUE_IP))
	{
		/*查询vobcName失败，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*查询vobcName成功，继续执行*/
	}

	/*根据vobcName查询对应wgbName*/	
	for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC==tempVobcName)
		{
			tempWgbName=DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB;
			break;
		}
		else
		{
			/*继续循环*/
		}
	}

	if (i==DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*查询wgbName失败，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*查询成功，继续执行*/
	}

	/*根据wgbName和红蓝网信息查询wgb设备ip*/
	bReturnValue=dsuGetAllDevIpByName(WgbIpBuf, 4, tempWgbName,bRedOrBlueIP);

	return bReturnValue;
}

/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*输出参数：NtpServerIpBuf  NTP服务器的IP地址
*返回值：  找到NTP服务器IP地址的数量
*/
UINT8 dsuNtpServerIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NtpServerIpBuf[][4], UINT16 size)
{
	/*仅在上电时查询一次*/
	UINT16 i = 0;/*循环变量*/
	UINT8 bReturnValue = 0;/*函数返回值*/
	UINT8 bRedOrBlueIP = 0;/*红蓝网IP标志变量*/
	UINT8 TempRet = 0;  /*临时返回值变量*/
	UINT16 tempVobcName = 0; /*当前ccov所在vobc的名称*/
	UINT16 tempNtpName=0;  /*Ntp名称*/

	/*根据当前ip查询vobcName和红蓝网信息*/
	bRedOrBlueIP=dsuGetDevNameByIp(CcovIpBuf,&tempVobcName);
	if ((bRedOrBlueIP!=CCOV_RED_IP)&&(bRedOrBlueIP!=CCOV_BLUE_IP))
	{
		/*查询vobcName失败，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*查询vobcName成功，继续执行*/
	}

	/*根据vobcName查询NtpName*/	
	for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC==tempVobcName)
		{
			tempNtpName=DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer;
			break;
		}
		else
		{
			/*继续循环*/
		}
	}

	if (i==DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*查询NtpName失败，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*查询成功，继续执行*/
	}

	/*根据NtpName和红蓝网信息查询设备ip*/
	TempRet=dsuGetAllDevIpByName(NtpServerIpBuf, size, tempNtpName, bRedOrBlueIP);

	return TempRet;
}


/*DSU提供的与CCOV相连接的WGB的IP地址查询函数
*输入参数: CcovIpBuf 通信控制器的IP地址
*输出参数：NmsIpBuf  车辆段网管系统的IP地址
UINT16* NmsPort，端口号
*返回值：  0 查询失败; 1查询成功
*/
UINT8 dsuNmsIpGetByCcovIp(const UINT8 CcovIpBuf[], UINT8 NmsIpBuf[],UINT16* NmsPort)
{
	/*仅在上电时查询一次*/
	UINT16 i = 0;/*循环变量*/
	UINT8 bReturnValue;/*函数返回值*/
	UINT8 bRedOrBlueIP;/*红蓝网IP标志变量*/
	UINT8 TempRet;  /*临时返回值变量*/
	UINT16 tempVobcName; /*当前ccov所在vobc的名称*/
	UINT16 tempNmsName=0;  /*Nms名称*/

	/*根据当前ip查询vobcName和红蓝网信息*/
	bRedOrBlueIP=dsuGetDevNameByIp(CcovIpBuf,&tempVobcName);
	if ((bRedOrBlueIP!=CCOV_RED_IP)&&(bRedOrBlueIP!=CCOV_BLUE_IP))
	{
		/*查询vobcName失败，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*查询vobcName成功，继续执行*/
	}

	/*根据vobcName查询NmsName*/	
	for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
	{
		if (DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC==tempVobcName)
		{
			/*nmsName赋值*/
			tempNmsName=DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS;
			if (bRedOrBlueIP==CCOV_RED_IP)
			{
				/*端口赋值，当前为红网*/
				*NmsPort=DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort;
			}
			else
			{
				/*端口赋值，当前为蓝网*/
				*NmsPort=DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort;
			}
			break;
		}
		else
		{
			/*继续循环*/
		}
	}

	if (i==DsuBookIpInfoStru.CcovCommInfoLen)
	{
		/*查询NmsName失败，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}
	else
	{
		/*查询成功，继续执行*/
	}

	/*根据NmsName和红蓝网信息查询设备ip*/
	TempRet=dsuGetDevIpByName(NmsIpBuf,tempNmsName,bRedOrBlueIP);

	if (TempRet==1)
	{
		/*查询成功*/
		bReturnValue=1;
	}
	else
	{
		/*查询失败*/
		bReturnValue=0;
	}

	return bReturnValue;
}

/*通过ip地址查询设备名称和红蓝网信息
*输入参数: UINT8 CcovIpBuf[]  设备ip地址
*输出参数：UINT16* DevName 设备名称
*返回值：  1-红网; 2-蓝网; 0xff-无效
*/
UINT8 dsuGetDevNameByIp(const UINT8 DevIpBuf[],UINT16* DevName)
{
	UINT16 i = 0;
	UINT16 j = 0;
	UINT8 bReturnValue;
	DSU_IP_INFO_STRU* pCcovConfigStru;

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

	/*遍历蓝网ip*/
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
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
			*DevName=pCcovConfigStru->DevName;
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
	for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
	{
		for (j=0;j<DSURP_LEN_IP_ADDR;j++)
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
			*DevName=pCcovConfigStru->DevName;
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
*输入参数: UINT16 DevName 设备名称
*			int nIPMaxCount:IP地址的最大数量
*输出参数：ipInfo 设备IP地址信息数组
*返回值：  找到的IP数量
*/
UINT8 dsuGetDevIPInfoByName(DSU_IP_INFO_STRU ipInfo[], int nIPMaxCount, UINT16 DevName)
{
	UINT16 i = 0;   /* 临时循环变量 */
	int	nIPCount = 0;  /* IP地址数量 */
	DSU_IP_INFO_STRU* pCcovConfigStru = NULL;  /* IP地址信息指针 */

	/*入口参数检查*/
	if (NULL == ipInfo)
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
*输入参数:  UINT16 DevName 设备名称
			      UINT8 RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：UINT8 CcovIpBuf[]  设备ip地址
*返回值：  1-查询成功;  0-查询失败
*修改记录: 修复QC689,黄成,20180816;
*/
UINT8 dsuGetDevIpByName(UINT8 DevIpBuf[], UINT16 DevName, UINT8 RedOrBlue)
{
	UINT16 i = 0;			 /*循环控制变量*/
	UINT8 bReturnValue = 0;  /*函数返回值*/
	DSU_IP_INFO_STRU *pCcovConfigStru = NULL; /*IP地址结构体*/

	/*入口参数检查*/
	if((NULL != DevIpBuf) && ((CCOV_RED_IP == RedOrBlue) || (CCOV_BLUE_IP == RedOrBlue)))
	{
		/*红蓝网参数有效,继续执行*/
		
		/*利用管理函数,将CCOV的头地址给局部变量*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;
	
		/*遍历设备ip地址表*/
		for(i = 0;i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{
			if(pCcovConfigStru->DevName == DevName)
			{
				/*已找到设备名称*/
				if(CCOV_RED_IP == RedOrBlue)
				{
					/*当前红网*/
					memcpy(DevIpBuf, pCcovConfigStru->IpAddrRed, DSURP_LEN_IP_ADDR);
				}
				else 
				{
					/*当前蓝网*/
					memcpy(DevIpBuf, pCcovConfigStru->IpAddrBlue, DSURP_LEN_IP_ADDR);
				}
	
				/*已找到设备,返回成功*/
				bReturnValue = 1;
				break;
			}
			else
			{
				/*未找到设备名称,继续循环*/
			}
	
			/*查询指针后移*/
			pCcovConfigStru++;
		}
	}
	else
	{
		/*输入参数无效,返回失败*/
		bReturnValue = 0;
	}

	return bReturnValue;
}

/*通过设备名称和红蓝网查询设备所有的ip地址
*输入参数: UINT16 DevName 设备名称
*UINT16 BufSize:IP地址缓冲区大小
UINT8 RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：UINT8 CcovIpBuf[]  设备ip地址
*返回值：  找到的IP地址的数量,0未查找到,非0查找成功(设备IP的数量)
*修改记录: 修复QC689,李元鹏,20190313.
*/
UINT8 dsuGetAllDevIpByName(UINT8 DevIpBuf[][4], UINT16 BufSize, UINT16 DevName, UINT8 RedOrBlue)
{
	UINT16 i = 0;  /*循环变量*/
	UINT8 Index = 0;  /*函数返回值*/
	DSU_IP_INFO_STRU *pCcovConfigStru = NULL; /*IP地址结构体指针*/

	/*入口参数检查*/
	if((CCOV_RED_IP == RedOrBlue) || (CCOV_BLUE_IP == RedOrBlue))
	{
		/*红蓝网参数有效,继续执行*/

		/*利用管理函数,将CCOV的头地址给局部变量*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

		/*遍历设备ip地址表*/
		for(i = 0;i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{
			if(pCcovConfigStru->DevName == DevName)
			{
				if(Index < BufSize)
				{
					/*已找到设备名称*/
					if(CCOV_RED_IP == RedOrBlue)
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
				/*未找到设备名称,继续循环*/
			}

			/*查询指针后移*/
			pCcovConfigStru++;
		}
	}
	else
	{
		/*输入参数无效*/
		Index = 0;
	}

	return Index;
}

/*
* 功能描述：根据红网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
UINT8 dsuGetRedDevName(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
    UINT16 i=0;/* 循环使用 */
	UINT8 flag=0;
    UINT16 ipInfoLen=0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru=DsuBookIpInfoStru.pIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrRed[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrRed[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrRed[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrRed[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpRedPort)||(port==pTempIpInfoStru[i].RsspRedPort)||
			(port==pTempIpInfoStru[i].SfpRedPort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	
	return retnVal;
}

/*
* 功能描述：根据蓝网的IP地址和端口号获取设备名称
* 输入参数：UINT16 port 端口号
*           UINT8* ipAdd  IP地址
* 输出参数：devName 设备名称  UINT16*
* 返 回 值：1:正常返回 0：错误返回
*/
UINT8 dsuGetBlueDevName(UINT8* ipAdd,UINT16 port,UINT16* devName)
{
	UINT8 retnVal=0;
	UINT16 i=0;/* 循环使用 */
	UINT8 flag=0;
	UINT16 ipInfoLen=0;
	DSU_IP_INFO_STRU* pTempIpInfoStru;
	ipInfoLen =DsuBookIpInfoStru.IpInfoStruLen;
	pTempIpInfoStru=DsuBookIpInfoStru.pIpInfoStru;

	for(i=0;i<ipInfoLen;i++)
	{
		if ((ipAdd[0]==pTempIpInfoStru[i].IpAddrBlue[0])&&(ipAdd[1]==pTempIpInfoStru[i].IpAddrBlue[1])&&
			(ipAdd[2]==pTempIpInfoStru[i].IpAddrBlue[2])&&(ipAdd[3]==pTempIpInfoStru[i].IpAddrBlue[3]))
		{
			flag=1;
		}
		else
		{
			flag =0;
		}
		if ((flag==1)&&((port==pTempIpInfoStru[i].RpBluePort)||(port==pTempIpInfoStru[i].RsspBluePort)||
			(port==pTempIpInfoStru[i].SfpBluePort)))
		{
			*devName=pTempIpInfoStru[i].DevName;
			retnVal =1;
			break;
		}
		else
		{
			retnVal =0;
		}
	}
	
	return retnVal;
}

/*通过设备名称和红蓝网查询设备网关
*输入参数: UINT16 DevName 设备名称
UINT8 RedOrBlue 1-红网; 2-蓝网; 0xff-无效
*输出参数：UINT8 DevGateWayBuf[] 设备网关地址
*返回值：  1-成功，0-失败
*/
UINT8 dsuGetGateWayByName(UINT8 DevGateWayBuf[],UINT16 DeviceName,UINT8 RedOrBlue)
{
	UINT16 i = 0;
	UINT8 bReturnValue;
	DSU_GATEWAY_INFO_STRU* pCcovConfigStru = NULL;

	/*入口参数检查*/
	if ((RedOrBlue==CCOV_RED_IP)||(RedOrBlue==CCOV_BLUE_IP))
	{
		/*红蓝网参数有效，继续执行*/
	}
	else
	{
		/*输入参数无效，函数返回失败*/
		bReturnValue=0;
		return bReturnValue;
	}

	/*利用管理函数，将CCOV的头地址给局部变量*/
	pCcovConfigStru = DsuBookIpInfoStru.pGatewayInfoStru;

	/*遍历设备ip地址表*/
	/*20200803 lp 修复原有代码白盒缺陷*/
	for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
	{
		if (pCcovConfigStru->DeviceName == DeviceName)
		{
			/*已找到设备名称*/
			if (RedOrBlue==CCOV_RED_IP)
			{
				/*当前红网*/
				memcpy(DevGateWayBuf,pCcovConfigStru->IpAddrGatewayRed,DSURP_LEN_IP_ADDR);
			}
			else 
			{
				/*当前蓝网*/
				memcpy(DevGateWayBuf,pCcovConfigStru->IpAddrGatewayBlue,DSURP_LEN_IP_ADDR);
			}
			/*已找到设备，返回成功*/
			bReturnValue=1;
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


/*通过设备名称查询红网UDP发送源端口号
*输入参数: UINT16 DevName 设备名称
*输出参数：UINT16* SourcePort :UDP发送源端口号
*返回值：查询成功返回1，否则返回0
*/
UINT8 dsuGetRedSourcePortByName(UINT16 DevName,UINT16* SourcePort)
{
	UINT16 i = 0;   /* 临时循环变量 */
	UINT8 rslt = 0;  /* 查询结果返回值*/
	DSU_IP_INFO_STRU* pCcovConfigStru = NULL;  /* IP地址信息指针 */

	/*入口参数检查*/
	if (NULL == DevName)
	{
		/* 如果参数无效，返回0*/
		rslt = 0;
	}
	else
	{
		/*利用管理函数，将CCOV的头地址给局部变量*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

		/*遍历设备ip地址表*/
		for (i = 0; i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{

			if (pCcovConfigStru->DevName == DevName)
			{			
				/*找到一个可以匹配的即退出查找*/
				*SourcePort=pCcovConfigStru->RedNetSourcePort;
				rslt = 1;
				break;
			}
			else
			{
				/*未找到设备名称，继续循环*/
			}
			/*查询指针后移*/
			pCcovConfigStru++;
		}
	}
	return rslt;
}

/*通过设备名称查询蓝网UDP发送源端口号
*输入参数: UINT16 DevName 设备名称
*输出参数：UINT16* SourcePort :UDP发送源端口号
*返回值：查询成功返回1，否则返回0
*/
UINT8 dsuGetBlueSourcePortByName(UINT16 DevName,UINT16* SourcePort)
{
	UINT16 i = 0;   /* 临时循环变量 */
	UINT8 rslt = 0;  /* 查询结果返回值*/
	DSU_IP_INFO_STRU* pCcovConfigStru = NULL;  /* IP地址信息指针 */

	/*入口参数检查*/
	if (NULL == DevName)
	{
		/* 如果参数无效，返回0*/
		rslt = 0;
	}
	else
	{
		/*利用管理函数，将CCOV的头地址给局部变量*/
		pCcovConfigStru = DsuBookIpInfoStru.pIpInfoStru;

		/*遍历设备ip地址表*/
		for (i = 0; i < DsuBookIpInfoStru.IpInfoStruLen; i++)
		{

			if (pCcovConfigStru->DevName == DevName)
			{			
				/*找到一个可以匹配的即退出查找*/
				*SourcePort=pCcovConfigStru->BlueNetSourcePort;
				rslt = 1;
				break;
			}
			else
			{
				/*未找到设备名称，继续循环*/
			}
			/*查询指针后移*/
			pCcovConfigStru++;
		}
	}
	return rslt;
}


/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：UINT8* FSName， 文件名或者指针地址
pDsuIpInfoStru， 字节数据头指针
* 输出参数：pDsuIpInfoStru， 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*合并说明：由CPK引入，BY LJ,20161210
*/
static UINT8 dsuIpInit_CPK(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;

	/*有效性检查*/
	if(NULL == FSName)
	{
		chReturnValue=0;
	}
	/*初始化ip地址结构体表*/
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;
		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}	

	/*初始化设备类型与通信协议关系表*/
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}
	

	/*初始化CCOV通信关系配置表信息*/
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}	

	/*初始化ZCDSU通信关系配置表信息*/
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<DSU_MAX_ZCDSU_COMM_OBJ_NUM;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
				dataOff+=2;

			}

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{

		}
	}	

	/*继续执行*/
	return chReturnValue;
}

/*
* 功能描述：IP配置表初始化函数，用于检查数据正确性，并将数据结构化。
* 输入参数：UINT8* FSName， 文件名或者指针地址
pDsuIpInfoStru， 字节数据头指针
* 输出参数：pDsuIpInfoStru， 初始化后的数据
* 返 回 值：1，成功
*           0，失败，数据有错误
*合并说明：FAO，HLT共用，BY LJ,20161210
* 创建日期:201701
* 创建人:zlj
*/
static UINT8 dsuIpInit_DFT(UINT8*FSName)
{
	UINT8 chReturnValue =1 ; /*返回值*/
	UINT8 tempRet;
	UINT8* pData=NULL;       /*用于读取数据的指针*/
	UINT32 dataLen=0;        /*数据长度*/
	UINT32 dataOff=0;        /*数据偏移量*/
	UINT32 i;
	UINT32 j;
	UINT8 tmpDsuMaxZcDsuCommObjNum = 0;/*临时记录ZCDSU的目标个数*/
	UINT8 cbtcVersionType=0;

	/*有效性检查*/
	if(NULL == FSName)
	{
		chReturnValue=0;
	}
	
	cbtcVersionType = GetCbtcSysType();

	/*初始化ip地址结构体表*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.IpInfoStruLen=(UINT16)(dataLen/DSU_IP_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pIpInfoStru=(DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pIpInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pIpInfoStru,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen),0,sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.IpInfoStruLen;i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName=ShortFromChar(pData+dataOff);
			dataOff+=2;
			
			DsuBookIpInfoStru.pIpInfoStru[i].HLHTId=LongFromChar(pData+dataOff);
			dataOff+=4;
			
			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pIpInfoStru[i].BlueNetSourcePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
			DsuBookIpInfoStru.pIpInfoStru[i].RedNetSourcePort=ShortFromChar(pData+dataOff);
			dataOff+=2;
		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}

	

	/*初始化设备类型与通信协议关系表*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.CommProtclStruLen=(UINT16)(dataLen/DSU_COMM_PROTCL_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCommProtclStru=(DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCommProtclStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCommProtclStru,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen),0,sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CommProtclStruLen;i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType=pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType=pData[dataOff];
			dataOff++;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}	


	/*初始化CCOV通信关系配置表信息*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen=(UINT16)(dataLen/DSU_CCOV_COMM_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pCcovCommInfoStru=(DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pCcovCommInfoStru,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen),0,sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.CcovCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort=ShortFromChar(pData+dataOff);
			dataOff+=2;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}

	

	/*初始化ZCDSU通信关系配置表信息*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	
	DsuBookIpInfoStru.ZcdsuCommInfoLen=(UINT16)(dataLen/DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO);  /*计算表格中数据的个数*/
	
	DsuBookIpInfoStru.pZcdsuCommInfoStru=(DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pZcdsuCommInfoStru,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen),0,sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff=0;
		
		tmpDsuMaxZcDsuCommObjNum = DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO;  /*计算表格中数据的个数*/
		
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.ZcdsuCommInfoLen;i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName=ShortFromChar(pData+dataOff);
			dataOff+=2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum=ShortFromChar(pData+dataOff);
			dataOff+=2;

			for (j=0;j<tmpDsuMaxZcDsuCommObjNum;j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j]=ShortFromChar(pData+dataOff);
				dataOff+=2;

			}

		}
		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{
			/*继续执行*/
		}
	}



	/*初始化设备网关表信息*/
	/*读取数据*/
	tempRet=dquGetConfigData((CHAR*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_GATEWAY_ID, &pData , &dataLen);
	if (tempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
	}

	DsuBookIpInfoStru.GatewayInfoLen=(UINT16)(dataLen/DSU_GATEWAY_INFO_STRU_SIZE);  /*计算表格中数据的个数*/
	DsuBookIpInfoStru.pGatewayInfoStru=(DSU_GATEWAY_INFO_STRU*)malloc(sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen)); /*为结构体申请空间*/
	if (DsuBookIpInfoStru.pGatewayInfoStru==NULL)
	{
		/*空间分配失败*/
		chReturnValue=0;
	}
	else
	{
		/*继续执行*/
		CommonMemSet(DsuBookIpInfoStru.pGatewayInfoStru,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen),0,sizeof(DSU_GATEWAY_INFO_STRU)*(DsuBookIpInfoStru.GatewayInfoLen));
		dataOff=0;
		/*通过循环将数据解析到结构体变量中，顺序与结构体定义相对应*/
		for (i=0;i<DsuBookIpInfoStru.GatewayInfoLen;i++)
		{
			/*编号 2*/
			dataOff+=2;

			/*设备名称2*/
			DsuBookIpInfoStru.pGatewayInfoStru[i].DeviceName = ShortFromChar(pData+dataOff);
			dataOff+=2;

			/*设备红网网关 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayRed,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*设备蓝网网关 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrGatewayBlue,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

			/*子网掩码 4*/
			memcpy(DsuBookIpInfoStru.pGatewayInfoStru[i].IpAddrSubnetMask,pData+dataOff,DSURP_LEN_IP_ADDR);
			dataOff+=DSURP_LEN_IP_ADDR;

		}

		if (dataOff!=dataLen)
		{
			/*数据长度不是解析数据的整数倍，说明有异常*/
			chReturnValue=0;
		}
		else
		{

		}
	}	

	/*继续执行*/
	return chReturnValue;
}


/*功能描述: 访问接口
* 返 回 值：1，成功
*           0，失败
* 创建日期:201701
* 创建人:zlj
*/
UINT8 dsuCcovIpInit(UINT8*FSName)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;

	/*有效性检查*/
	if(NULL != FSName)
	{			
		cbtcVersionType=GetCbtcSysType();

		if (DQU_CBTC_CPK == cbtcVersionType )
		{
		
			rtnValue = dsuIpInit_CPK(FSName);		
		} 
		else if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)|| (DQU_CBTC_DG== cbtcVersionType) )
		{
			rtnValue = dsuIpInit_DFT(FSName);		
		}
		else
		{
			rtnValue = 0;		
		}
	}
	else
	{
		rtnValue = 0;		
	}
	
	return rtnValue; 
}



