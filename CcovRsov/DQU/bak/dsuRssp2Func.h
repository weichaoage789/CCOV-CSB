/************************************************************************
*@ 文件名	    ： dsuRsspDataStru.h
*@ 版权说明	：
*@ 版本号	    ： 1.0
*@ 创建时间	： 2020.4
*@ 作者		    ： HHY
*@ 功能描述	： RSSP2协议查询函数
*@ 使用注意	： RSSP2查询函数需要使用编号对照表，所以应用需要分别初始化RSSP2查询函数和编号对照表（无先后次序要求），这两部分都初始化完成后才可以使用RSSP2协议查询函数
热加载时在RSSP2查询函数和编号对照表都变更到新的数据源后（无先后次序要求）才可以使用RSSP2协议查询函数
*@ 修改记录	：

************************************************************************/

#ifndef _DSU_RSSP2_FUNC_H_
#define _DSU_RSSP2_FUNC_H_

#include "common/CommonTypes.h"
#include "dsuRssp2DataStru.h"
#include "dsuIdmapVar.h"
#include "dsuEmapDefine.h"


#define GET_VOBC_END_ID(EtcsId) (unsigned char)((EtcsId)&0x03)
#define ETCSID_MASK (0x7FFFFFF)

	extern DSU_RSSP2_INFO_STRU dsuRssp2InfoStru;	     /* 用于保存RSSP2表信息的结构体对象。*/
	extern DSU_RSSP2_INFO_STRU *g_dsuRssp2Stru;      /* 当前正在操作的数据库 */

													 /* 设置RSSP2配置数据静态结构体 */
	unsigned char dquSetInitRssp2Ref(DSU_RSSP2_INFO_STRU * pDsuRssp2Stru);

	/*RSSP2协议查询函数初始化*/
	unsigned char dsuRssp2Init(char* FSName);

	/*2字节内部编号转4字节ETCSID*/
	unsigned char dquDevName2EtcsId(unsigned short DevName, unsigned short LogId, unsigned int *pEtcsId);

	/*4字节ETCSID转2字节内部编号*/
	unsigned char dquEtcsId2DevName(unsigned int EtcsId, unsigned short *pDevName, unsigned short *pLogId);

	/*根据ETCSID获取RSSP-2协议使用的IP端口*/
	unsigned char dquGetIpPortByEtcsId(unsigned int EtcsId, unsigned char *pNum, DSU_IP_PORT *pRedIpPort, DSU_IP_PORT *pBlueIpPort);

	/*根据RSSP-2协议使用的IP端口获取ETCSID*/
	unsigned char dquGetEtcsIdByIpPort(DSU_IP_PORT IpPort, unsigned int *pEtcsId);

	/*获取轨旁设备类型配置数据*/
	unsigned char dquRssp2GetWaysideTypeCfg(unsigned char WsEtcsType, DSU_RSSP2_WS_TYPE_CFG_STRU *pRssp2RmtTypeCfg);

	/*获取本地设备类型配置数据*/
	unsigned char dquRssp2GetLocalTypeCfg(unsigned char LocalEtcsType, DSU_RSSP2_LOC_TYPE_CFG_STRU *pRssp2LocTypeCfg);

	/*获取设备配置数据*/
	unsigned char dquRssp2GetDevCfg(unsigned int EtcsId, DSU_RSSP2_DEV_CFG_STRU *pRssp2DevCfg);

	/* 根据互联互通设备编号获取互联互通设备类型 */
	unsigned char GetEtcsTypeFromEtcsId(unsigned int EtcsId, RSSP2_DEVICE_TYPE *pDevType);

	/*根据本方设备编号获取本方的通信角色*/
	unsigned char dquRssp2GetRole(unsigned int etcsId_local, unsigned char *pRole);

	/*内存释放函数*/
	unsigned char dsuRsspIIFreeSpace(void);


#endif


