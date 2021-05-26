/************************************************************************
*@ 文件名	    ： dsuRsspDataStru.h
*@ 版权说明	：
*@ 版本号	    ： 1.0
*@ 创建时间	： 2020.4
*@ 作者		    ： HHY
*@ 功能描述	： 与RSR协议相关的dsu查询结构体定义，内部使用
*@ 使用注意	：
*@ 修改记录	：

************************************************************************/

#ifndef _DSU_RSSP2_DATA_STRU_H
#define _DSU_RSSP2_DATA_STRU_H


/*定义最大长度用于存储数据库中数据*/
#define DSU_DATA_PRTCL_RSSP2_RMT_TYPE_LINE_SIZE  (40)
#define DSU_DATA_PRTCL_RSSP2_LOC_TYPE_LINE_SIZE  (7) 
#define DSU_DATA_PRTCL_RSSP2_DEV_LINE_SIZE  (40) 

#define DSU_DATA_PRTCL_RSSP2_RMT_TYPE_CFG		 (7)
#define DSU_DATA_PRTCL_RSSP2_LOC_TYPE_CFG (8)
#define DSU_DATA_PRTCL_RSSP2_DEV_CFG	 (9)

/* VOBC使用端编号 */
#define VOBC_ENDID_TC1     1    /* TC1端 */
#define VOBC_ENDID_TC2     2    /* TC2端*/
#define VOBC_ENDID_REDUN   0    /* 2IP车（首尾冗余车） */

#undef RSSP2_2IP_MODE /*调试用*/

typedef enum
{
	DEVICE_TYPE_ZC = 0x01, /* ZC */
	DEVICE_TYPE_ATP = 0x02, /* 车载ATP */
	DEVICE_TYPE_AOM = 0x03, /* 车载AOM */
	DEVICE_TYPE_ATS = 0x04, /* ATS */
	DEVICE_TYPE_DSU = 0x05, /* DSU */
	DEVICE_TYPE_CI = 0x06, /* CI */
	DEVICE_TYPE_MSS = 0x07, /* MSS */
	DEVICE_TYPE_UNKNOWN = 0xFF  /* 未知 */
}RSSP2_DEVICE_TYPE;


	/*以下结构体定义用于dsu数据库中的数据存储，不对外发布*/
	/*rssp2 表7 协议栈层共享配置信息结构体*/
	typedef struct
	{
		unsigned char RemoteEtcsType;			/*对方ETCS类型*/
		unsigned int SAITsyn;					/* SAI层Tsyn定时器超时值*/
		unsigned int SAIECRtnPeriod;			/* SAI层长周期回调的周期数 */
		unsigned short SAISNDiffMax;            /* SAI层序号最大差值 */
		unsigned int SAIECAlarm;				/* SAI层EC报警状态值*/
		unsigned int SAIECRtnValue;			/* SAI层EC负参数回调值 TO BE DONE */
		unsigned int SAIMaxErrValue;			/*SAI层连续错误最大值 */
		unsigned int SAIUpdFailCountMax;      /* SAI层更新过程连续错误最大值 */
		unsigned int MASLTsynTimeOut;			/* MASL层Testab定时器超时值 */
		unsigned int ALETsynTimeOut;			/* ALE层Tsyn定时器超时值 */
		unsigned int ALETconTimeOut;			/*ALE层Tcon定时器超时值(通控使用,协议适配层不使用) */
		unsigned char ALETSNCheckMode;          /* ALE层TSN检查模式 */
	}DSU_RSSP2_WS_TYPE_CFG_STRU;  /*%RELAX<MISRA_8_9>*/

								  /*rssp2 表8 RSSP2设备类型参数表结构体*/
	typedef struct
	{
		unsigned char LocalEtcsType;			/* 本方ETCS类型*/
		unsigned int SAI_Tupd; 		        /* Tupd定时器超时值*/
		unsigned short SAI_EcPeriod;    		/* SAI层EC自周期值 */
	}DSU_RSSP2_LOC_TYPE_CFG_STRU;/*%RELAX<MISRA_8_9>*/

	typedef struct
	{
		unsigned char Ip[4];
		unsigned short Port;
	}DSU_IP_PORT;  /*%RELAX<MISRA_5_3>*/

				   /*rssp2 表9 RSSP2设备通信参数表结构体*/
	typedef struct
	{
		unsigned int EtcsId;				/*设备ETCS编号*/
		unsigned char Key[24];              /*鉴定密钥*/
		DSU_IP_PORT BlueIpPort;
		DSU_IP_PORT RedIpPort;
	}DSU_RSSP2_DEV_CFG_STRU;/*%RELAX<MISRA_8_9>*/

	typedef struct
	{
		unsigned int dsuRssp2WsTypeCfgLen;
		DSU_RSSP2_WS_TYPE_CFG_STRU *pDsuRssp2WsTypeCfgStru;/* 表7 */

		unsigned int dsuRssp2LocTypeCfgLen;
		DSU_RSSP2_LOC_TYPE_CFG_STRU *pDsuRssp2LocTypeCfgStru;/* 表8 */

		unsigned int dsuRssp2DevCfgLen;
		DSU_RSSP2_DEV_CFG_STRU *pDsuRssp2DevCfgStru;/* 表9 */

	}DSU_RSSP2_INFO_STRU, *LPDSU_RSSP2_INFO_STRU;/*%RELAX<MISRA_8_9>*/


#endif

