/************************************************************************
*
* 文件名   ：  dsuQuery.h
* 版权说明 ：  北京交控科技科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2009.11.21
* 作者     ：  软件部
* 功能描述 ：  dsu查询函数公共函数头文件
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _DSU_QUERYEXP_H
#define _DSU_QUERYEXP_H

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "CommonTypes.h"
#include "dsuStruct.h"
#include "dquVobcCommonStru.h"


#define DSU_MAX_LINK_BETWEEN 400


#ifdef __cplusplus
extern "C" {
#endif

/*ATO函数声明*/

/*2.两点间相对距离查询*/
UINT8 dsuDispCalcuByLOCExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement);

/*功能函数，根据道岔状态获取当前Link的相邻Link*/
UINT8 dsuGetAdjacentLinkIDExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId, UCHAR Direction, UCHAR PointNum,const DQU_POINT_STATUS_STRU *pPointStru, UINT16 *pLinkId);

/*功能函数，旧位置+位移=新位置*/
UCHAR dsuLODCalcuExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru);

/*功能函数，获取两点之间的link序列*/
UINT8 dsuGetLinkBtwLocExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
					   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
					   UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber);

/*功能函数，判断一个点是否在区域范围内*/
UINT8 dsuCheckLocInAreaExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						const LOD_STRU *pCurrentLODStru,UINT8 *pLocInArea); 

/*功能函数，获取link长度*/
UINT32 LinkLengthExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 wLinkID);

/*
* 函数功能： 此函数的功能是判断当前位置是否在指定区域范围内，此函数是对dsuCheckLocInAreaQuick
             的一个补充。用于将当前方向取反，再计算是否在区域内。
* 入口参数： const LOD_STRU * pLODStartStru,	区域始端位置
*            const LOD_STRU * pLODEndStru,	区域终端位置
* 出口参数： UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*            UINT16 *wLinkNumber                             Link个数
*			 const LOD_STRU * pCurrentLODStru,当前位置
*            UINT8 *pLocInArea         1,在指定区域
*                                     0,不在指定区域
* 返回值：   0，查询失败
*            1，查询成功
*/
UINT8 dsuCheckLocInAreaQuickExp2(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
							 UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
							 const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea);
/*
* 函数功能： 此函数的功能是将判断当前位置是否在指定区域范围内。
             此函数的功能与CheckLocInArea是有区别的。CheckLocInArea对方向没有要求，假设所有涉及的方向都是正确的。
			 而CheckLocInAreaQuick要求从StartLOD能够到达EndLOD。
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       查找算法为，首先判断当前位置与输入的S，E是否在同一Link上，
*       如果在同一Link上，根据方向和位移判断是否在区域内，如果不在同一Link，判断CLinkId是否与SE之间的某一LinkId相同
*       由于两点之间的Link可以保存下来，所以对于多次调用本函数查询是否在区域内的函数效率很有帮助
*
* ****************************************************************************************************
* 入口参数： const LOD_STRU * pLODStartStru,	区域始端位置
*            const LOD_STRU * pLODEndStru,	区域终端位置
* 出口参数：	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link序列数组
*               UINT16 *wLinkNumber                             Link个数
*			 const LOD_STRU * pCurrentLODStru,当前位置
* 出口参数： UINT8 *pLocInArea         1,在指定区域
*                                     0,不在指定区域
* 返回值：   0，查询失败
*            1，查询成功
* 使用注意：1.在当前位置在区域端点位置时，暂时按如下规则判断：
*             1) 当前位置在区域起点，认为该点属于本区域；
*             2) 当前位置在区域终点，认为该点不属于本区域；
*             3) 3点重合返回在区域内。
*           2.如果待查找的起点与终点都在区域内，可以调用两次本函数，互换始端与终端位置，只要两次调用有一次在区域内，说明在点在始端与终端形成的闭区域内
*           3.如果SE区域不存在，则函数返回0失败。如果SC,或者SE不存在，则函数返回1成功，但是点C不在区域内
*/
UINT8 dsuCheckLocInAreaQuickExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
							 UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
							 const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea);
/*功能函数,查询当前Garde的相邻Grade*/
UINT16 FindAdjacentGradeIDExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wCurrentGradeID, const UCHAR chDirection, const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pstPointStrut);

/*计算点所在坡度*/
UINT8 dsuGetGradeIdbyLocExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 linkId,const UINT32 offset,UINT16* GradeID);
/*计算点到所在坡度起点的距离*/
UINT8 dsuGetDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32 Ofst,UINT32* length);
/*计算坡度变坡点于坡度起点的距离*/
UINT8 dsuDividPointDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32* endPointDistance,UINT32* startPointDistance);
/*计算点所在坡度值*/
UINT8 dsuCalculateGradeValueExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst, FLOAT32* GradeVal);
/*计算最差坡度*/
UCHAR dsuGradientGetExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient);
/*判断后一坡度的坡度值相比前一个坡度的坡度值是增加，不变化还是减少 */
UINT8 dquGradeTrendExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 fistGradeID,UINT16 secondGradeID,UINT8 *GradeTrend);

/*
函数功能：获取前面的坡度
输入参数：UINT16 linkId,当前link
输出参数：gradeFrnt 坡度 
			Null：  线路终点或前面坡度为统一坡度  
			非Null：前面坡度在坡度表中可查
  返回值：0失败 1成功
*/
UINT8 dsuGetGradeFrntExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeFrnt);
/*
函数功能：获取后面的坡度
输入参数：UINT16 linkId,当前link
输出参数：gradeRear 坡度 
			Null：  线路终点或后面坡度为统一坡度  
			非Null：后面坡度在坡度表中可查
  返回值：0失败 1成功
*/
UINT8 dsuGetGradeRearExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeRear);
/**
函数功能：获取当前LINK的静态限速个数
输入参数：
@linkId：当前link
输出参数：无
返回值：静态限速个数
*/
UINT8 GetStcLmtSpdCntByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId);

/**
函数功能：获取制定LINK的的指定下标的静态临时限速的对象指针
输入参数：
@linkId：当前Link编号
@lmtSpdIdx：对应Link的第几个限速，取值范围1-10
返回值：
@NULL：获取失败 
@非空：对应限速结构体指针
*/
DSU_STATIC_RES_SPEED_STRU *GetStcLmtSpdInfoByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId,const UINT8 idx);

/*
函数功能：将多开道岔转为虚拟道岔
输入参数：multiPointId 多开道岔编号
          multiPointState 多开道岔状态

输出参数：wVirtualPointIds 拆分后虚拟道岔编号 
          wVirtualPointStates 拆分后虚拟道岔状态
返回值：返回值：拆分后虚拟道岔个数, 0失败  

函数算法：多开道岔包含的虚拟道岔： N位，R1位，L1位，R2位，L2位
            所对应在数组中的索引： 0，  1，   2，   3，   4
		    所对应的多开道岔状态： 1，  2，   3，   4，   5
		   因此当 索引=多开道岔状态multiPointState-1 时，所对应的虚拟道岔保持原值，其余虚拟道岔状态取反（定位<=>反位）

注意事项：输出参数虚拟道岔wVirtualPointIds和状态wVirtualPointStates，对于无效道岔其道岔状态设置为3。如
         
		               / L                                      / L
		              /                                        /
		             /                                        /       101                
		-|------------------------         ===》   ---|------/----------------- N      
		   1         \           N                          102       \
		              \                                                \
					   \                                                \
		              R                                                 R
		           图1.实体三开道岔                            图2.虚拟单开道岔

		   当图1实体三开道岔1的状态为3时，推出图2虚拟单开道岔为L位。此时101为无效道岔，输出其状态为3，返回值道岔个数为2，如下
		   wVirtualPointIds[0]=102;
		   wVirtualPointStates[0]=2;
		   wVirtualPointIds[1]=101;
		   wVirtualPointStates[1]=3;
author:单轨项目 add by qxt 20170622
*/
UINT8 ConvertToVirtualSwitchesExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 multiPointId,const UINT8 multiPointState,UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM]);

/*
函数功能：将虚拟道岔转为多开道岔
输入参数：wVirtualPointIds[] 虚拟道岔编号 
          wVirtualPointStates[] 虚拟道岔状态
		  wVirtualPointNum    虚拟道岔数量
输出参数：multiPointId 对应多开道岔编号
          multiPointState 对应多开道岔状态
返回值：0失败 1成功

注意事项：输入参数虚拟道岔wVirtualPointIds和状态wVirtualPointStates，对于无效的虚拟道岔，不需要传入。如
           
		 		      /L                                                 L
		             /                                                /
		            /                                                /
		           /                                                /
		---|------/-----------\------ N      =====>        ------------------ N
		          1          2 \                                    \
							    \                                    \
		                          R                                      R
		         图1.虚拟单开道岔                             图2.实体三开道岔
		 
		 当图1虚拟道岔编号1的状态为2时，推出图2多开道岔的状态为L位。此时图一虚拟道岔2为无效的，其输入参数可设置为如下：
         wVirtualPointIds[0]=1;
		 wVirtualPointStates[0]=2;
         wVirtualPointNum=1;
author:单轨项目 add by qxt 20170622
*/
UINT8 ConvertToMultiSwitchExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointNum,UINT16 *multiPointId,UINT8 *multiPointState);



#ifdef __cplusplus
}
#endif

#endif