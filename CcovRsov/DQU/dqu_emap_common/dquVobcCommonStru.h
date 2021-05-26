
/************************************************************************
*
* 文件名   ：dquVobcCommonStru.h
* 版权说明 ：  北京交控科技科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2011.12.08
* 作者     ：  软件部
* 功能描述 ：VOBC应用公用结构体文件
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef DQU_VOBC_COMMON_STRUCT_H
#define DQU_VOBC_COMMON_STRUCT_H


#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif


/*位置结构体*/
typedef struct
{
	UINT16 Lnk;/*Link*/
	UINT32 Off;/*Offset*/
	UINT8 Dir;/*方向*/
}LOD_STRU;


/*道岔信息：包括道岔的ID及状态*/
typedef struct{
	UCHAR   PointType;				/*道岔的类型，可以考虑不用*/
	UINT16	PointId;				/*道岔的ID*/
	UINT16	Lnk;					/*障碍物所在区段号（D_ OBSTACLEL）*/
	UINT32	Off;					/*障碍物所在区段偏移量（D_ OBSTACLEO）*/
	UCHAR	PointStatus;			/*道岔的状态*/
	UCHAR	PointExpectedStatus;	/*道岔应该的状态*/
}DQU_POINT_STATUS_STRU;


/*临时限速结构体*/
typedef struct
{
	UINT16 TsrSpd;/*限速值*/
	UINT16 TsrHeadLnk;/*TSR起点Link*/
	UINT32 TsrHeadOff;/*TSR起点Offset*/
	UINT16 TsrTailLnk;/*TSR终点Link*/
	UINT32 TsrTailOff;/*TSR终点Offset*/
}TSR_STRU;

#ifdef __cplusplus
}
#endif

#endif
