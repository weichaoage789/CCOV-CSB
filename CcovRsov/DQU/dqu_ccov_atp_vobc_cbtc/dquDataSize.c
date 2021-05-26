/************************************************************************
*
* 文件名   ：  duqDataSize.c
* 版权说明 ：  北京交控科技科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2016.04.25
* 作者     ：  yunjun.ren
* 功能描述 ： 查询函数结构体内存分配处理
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/
#include "dquDataSize.h"
#include "dquDataTypeDefine.h"
#include "dfsDataRead.h"
#include "stdlib.h"
#include "dsuVar.h"
#include "dsuIdmapVar.h"
#include "string.h"

/*************************************************************************************************************
* 功能描述:    静态数据长度信息结构体初始化
* 输入参数:    DSU_EMAP_STRU *pDsuEmapStu 电子地图全部结构的总结构体
* 输出参数:    无.
* 全局变量:    无.
* 返回值  :    1代表成功;0代表失败
*************************************************************************************************************/
UINT8 vDquEmapStructInit(DSU_EMAP_STRU *pDsuEmapStu)
{
	UINT8 InitValue = 1;  /*函数返回值*/
	UINT8 dquSetValue = 0;

	/*入口参数检查*/
  	if(NULL == pDsuEmapStu)
    {
        /*入口数据异常,查询失败*/
		InitValue = 0;
    }
	else
	{
		pDsuEmapStu->dsuEmapIndexStru = (DSU_EMAP_INDEX_STRU*)MyNew(sizeof(DSU_EMAP_INDEX_STRU));
		if(NULL == pDsuEmapStu->dsuEmapIndexStru)
		{
			InitValue = 0;
		}
		else
		{
			/*初始化为0*/ 
			memset(pDsuEmapStu->dsuEmapIndexStru, 0, sizeof(DSU_EMAP_INDEX_STRU));
		}
		
		pDsuEmapStu->dsuDataLenStru = (DSU_DATA_LEN_STRU*)MyNew(sizeof(DSU_DATA_LEN_STRU));
		if(NULL == pDsuEmapStu->dsuDataLenStru)
		{
			InitValue = 0;
		}
		else
		{
			/*初始化为0*/ 
			memset(pDsuEmapStu->dsuDataLenStru, 0, sizeof(DSU_DATA_LEN_STRU));
		}
		
		pDsuEmapStu->dsuStaticHeadStru = (DSU_STATIC_HEAD_STRU*)MyNew(sizeof(DSU_STATIC_HEAD_STRU));
		if(NULL == pDsuEmapStu->dsuStaticHeadStru)
		{
			InitValue = 0;
		}
		else
		{
			/*初始化为0*/ 
			memset(pDsuEmapStu->dsuStaticHeadStru, 0, sizeof(DSU_STATIC_HEAD_STRU));
		}
	
		if(1 == InitValue)
		{
			/*静态数据索引数组内存分配39*/
			pDsuEmapStu->dsuEmapIndexStru->dsuLinkIndex = (UINT16*)MyNew(sizeof(UINT16) *nLINKINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_LINK_ID, nLINKINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuLinkIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuLinkIndex, 0xFF, sizeof(UINT16) *nLINKINDEXNUM + 2);			
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuPointIndex = (UINT16*)MyNew(sizeof(UINT16) *POINTINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_POINT_ID, POINTINDEXNUM+1);
			if((NULL ==pDsuEmapStu->dsuEmapIndexStru->dsuPointIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuPointIndex, 0xFF, sizeof(UINT16) *POINTINDEXNUM + 2);			
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuBaliseIndex = (UINT16*)MyNew(sizeof(UINT16) *(BALISEINDEXNUM) + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_BALISE_ID, BALISEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuBaliseIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuBaliseIndex, 0xFF, sizeof(UINT16) *(BALISEINDEXNUM) + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuSignalIndex = (UINT16*)MyNew(sizeof(UINT16) *(SIGNALINDEXNUM) + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_SIGNAL_ID, SIGNALINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuSignalIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuSignalIndex, 0xFF, sizeof(UINT16) *(SIGNALINDEXNUM) + 2);		
			}		
			
			pDsuEmapStu->dsuEmapIndexStru->dsuAxleIndex = (UINT16*)MyNew(sizeof(UINT16) *AXLEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_AXLE_ID, AXLEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuAxleIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuAxleIndex, 0xFF, sizeof(UINT16) *AXLEINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuScreenIndex = (UINT16*)MyNew(sizeof(UINT16) *SCREENINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_SCREEN_ID, SCREENINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuScreenIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuScreenIndex, 0xFF, sizeof(UINT16) *SCREENINDEXNUM + 2);		
			}	
	
			pDsuEmapStu->dsuEmapIndexStru->dsuStationIndex = (UINT16*)MyNew(sizeof(UINT16) *STATIONINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_STATION_ID, STATIONINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuStationIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuStationIndex, 0xFF, sizeof(UINT16) *STATIONINDEXNUM + 2);		
			}	
			
			pDsuEmapStu->dsuEmapIndexStru->dsuEmergStopIndex = (UINT16*)MyNew(sizeof(UINT16) *EMERGSTOPINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_EMERG_STOP_BUTTON_ID, EMERGSTOPINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuEmergStopIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuEmergStopIndex, 0xFF, sizeof(UINT16) *EMERGSTOPINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuAxleSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) *AXLESGMTINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_AXLE_SGMT_ID, AXLESGMTINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuAxleSgmtIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuAxleSgmtIndex, 0xFF, sizeof(UINT16) *AXLESGMTINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuStopPointIndex = (UINT16*)MyNew(sizeof(UINT16) *STOPPOINTINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_STOPPOINT_ID, STOPPOINTINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuStopPointIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuStopPointIndex, 0xFF, sizeof(UINT16) *STOPPOINTINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuTrainLineIndex = (UINT16*)MyNew(sizeof(UINT16) *TRAINLINEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_TRAIN_LINE_ID, TRAINLINEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuTrainLineIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuTrainLineIndex, 0xFF, sizeof(UINT16) *TRAINLINEINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuExitRouteIndex = (UINT16*)MyNew(sizeof(UINT16) *EXITROUTEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_EXIT_ROUTE_ID, EXITROUTEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuExitRouteIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuExitRouteIndex, 0xFF, sizeof(UINT16) *EXITROUTEINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuRunLevelIndex = (UINT16*)MyNew(sizeof(UINT16) *RUNLEVELINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_RUN_LEVEL_ID, RUNLEVELINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuRunLevelIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuRunLevelIndex, 0xFF, sizeof(UINT16) *RUNLEVELINDEXNUM + 2);		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuStaticResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) *STATICRESSPEEDNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_STATIC_RES_SPEED_ID, STATICRESSPEEDNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuStaticResSpeedIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuStaticResSpeedIndex, 0xFF, sizeof(UINT16) *STATICRESSPEEDNUM + 2); 		
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuGradeIndex = (UINT16*)MyNew(sizeof(UINT16) *GRADEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_GRADE_ID, GRADEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuGradeIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuGradeIndex, 0xFF, sizeof(UINT16) *GRADEINDEXNUM + 2);			
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuLogicSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) *LOGICSGMTINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_LOGIC_SGMT_ID, LOGICSGMTINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuLogicSgmtIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuLogicSgmtIndex, 0xFF, sizeof(UINT16) *LOGICSGMTINDEXNUM + 2);			
			}
		
			pDsuEmapStu->dsuEmapIndexStru->dsuRouteIndex = (UINT16*)MyNew(sizeof(UINT16) *ROUTEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_ROUTE_ID, ROUTEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuRouteIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuRouteIndex, 0xFF, sizeof(UINT16) *ROUTEINDEXNUM + 2);			
			}	
	
			pDsuEmapStu->dsuEmapIndexStru->dsuProtectLinkIndex = (UINT16*)MyNew(sizeof(UINT16) *PROTECTLINKINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_PROTECT_LINK_ID, PROTECTLINKINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuProtectLinkIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuProtectLinkIndex, 0xFF, sizeof(UINT16) *PROTECTLINKINDEXNUM + 2);			
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuCBTCAccessLinkIndex = (UINT16*)MyNew(sizeof(UINT16) *CBTCACCESSLINKINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_CBTC_ACCESS_LINK_ID, CBTCACCESSLINKINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuCBTCAccessLinkIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuCBTCAccessLinkIndex, 0xFF, sizeof(UINT16) *CBTCACCESSLINKINDEXNUM + 2);			
			}
			
			pDsuEmapStu->dsuEmapIndexStru->dsuBLOCAccessLinkIndex = (UINT16*)MyNew(sizeof(UINT16) *BLOCACCESSLINKINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_BLOC_ACCESS_LINK_ID, BLOCACCESSLINKINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuBLOCAccessLinkIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuBLOCAccessLinkIndex, 0xFF, sizeof(UINT16) *BLOCACCESSLINKINDEXNUM + 2);			
			}		
	
			pDsuEmapStu->dsuEmapIndexStru->dsuDefaultRoutIndex = (UINT16*)MyNew(sizeof(UINT16) *DEFAULTROUTEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_DEFAULT_ROUTE_ID, DEFAULTROUTEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuDefaultRoutIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuDefaultRoutIndex, 0xFF, sizeof(UINT16) *DEFAULTROUTEINDEXNUM + 2); 		
			}		
	
			pDsuEmapStu->dsuEmapIndexStru->dsuARInIndex = (UINT16*)MyNew(sizeof(UINT16) *ARINNUM + 2);
			pDsuEmapStu->dsuEmapIndexStru->dsuAROutIndex = (UINT16*)MyNew(sizeof(UINT16) *AROUTNUM + 2);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuARInIndex) || (NULL == pDsuEmapStu->dsuEmapIndexStru->dsuAROutIndex))
			{
				InitValue = 0;	
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuARInIndex, 0xFF, sizeof(UINT16) *ARINNUM + 2); 
	
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuAROutIndex, 0xFF, sizeof(UINT16) *AROUTNUM + 2);				
			}	
	
			pDsuEmapStu->dsuEmapIndexStru->dsuATSIndex = (UINT16*)MyNew(sizeof(UINT16) *ATSINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_ATS_ID, ATSINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuATSIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuATSIndex, 0xFF, sizeof(UINT16) *ATSINDEXNUM + 2);					
			}	
	
			pDsuEmapStu->dsuEmapIndexStru->dsuCIIndex = (UINT16*)MyNew(sizeof(UINT16) *CIINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_CI_ID, CIINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuCIIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuCIIndex, 0xFF, sizeof(UINT16) *CIINDEXNUM + 2);					
			}	
	
			pDsuEmapStu->dsuEmapIndexStru->dsuARIndex = (UINT16*)MyNew(sizeof(UINT16) *ARNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_AR_ID, ARNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuARIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuARIndex, 0xFF, sizeof(UINT16) *ARNUM + 2); 				
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuZCComZoneIndex = (UINT16*)MyNew(sizeof(UINT16) *ZCCOMZONENUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_ZC_COM_ZONE_ID, ZCCOMZONENUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuZCComZoneIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuZCComZoneIndex, 0xFF, sizeof(UINT16) *ZCCOMZONENUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuConResSpeedIndex = (UINT16*)MyNew(sizeof(UINT16) *CONRESSPEEDINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_CONRESSPEED_ID, CONRESSPEEDINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuConResSpeedIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuConResSpeedIndex, 0xFF, sizeof(UINT16) *CONRESSPEEDINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuConGradeIndex = (UINT16*)MyNew(sizeof(UINT16) *CONGRADEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_CONGRADE_ID, CONGRADEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuConGradeIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuConGradeIndex, 0xFF, sizeof(UINT16) *CONGRADEINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuZCDividPointIndex = (UINT16*)MyNew(sizeof(UINT16) *ZCDIVIDPOINTINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_ZC_DIVID_POINT_ID, ZCDIVIDPOINTINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuZCDividPointIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuZCDividPointIndex, 0xFF, sizeof(UINT16) *ZCDIVIDPOINTINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuPhysicalSgmtIndex = (UINT16*)MyNew(sizeof(UINT16) *PHYSICALSGMTINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_PHYSICAL_SGMT_ID, PHYSICALSGMTINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuPhysicalSgmtIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuPhysicalSgmtIndex, 0xFF, sizeof(UINT16) *PHYSICALSGMTINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuBlocTrgAreaIndex = (UINT16*)MyNew(sizeof(UINT16) *BLOCTRGAREAINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_BLOC_TRG_AREA_ID, BLOCTRGAREAINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuBlocTrgAreaIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuBlocTrgAreaIndex, 0xFF, sizeof(UINT16) *BLOCTRGAREAINDEXNUM + 2);					
			}
			
			pDsuEmapStu->dsuEmapIndexStru->dsuCbtcTrgAreaIndex = (UINT16*)MyNew(sizeof(UINT16) *CBTCTRGAREAINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_CBTC_TRG_AREA_ID, CBTCTRGAREAINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuCbtcTrgAreaIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuCbtcTrgAreaIndex, 0xFF, sizeof(UINT16) *CBTCTRGAREAINDEXNUM + 2);					
			}
			
			pDsuEmapStu->dsuEmapIndexStru->dsuPlatFormIndex = (UINT16*)MyNew(sizeof(UINT16) *PLATFORMINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_PLAT_FORM_ID, PLATFORMINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuPlatFormIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuPlatFormIndex, 0xFF, sizeof(UINT16) *PLATFORMINDEXNUM + 2);					
			}
			
			pDsuEmapStu->dsuEmapIndexStru->dsuComeGoIndex = (UINT16*)MyNew(sizeof(UINT16) *COMEGOINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_COME_GO_ID, COMEGOINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuComeGoIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuComeGoIndex, 0xFF, sizeof(UINT16) *COMEGOINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuFloodGateIndex = (UINT16*)MyNew(sizeof(UINT16) *FLOODGATEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_FLOOD_GATE_ID, FLOODGATEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuFloodGateIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuFloodGateIndex, 0xFF, sizeof(UINT16) *FLOODGATEINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuSpksButtonIndex = (UINT16*)MyNew(sizeof(UINT16) *SPKSBUTTONINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_SPKS_BUTTON_ID, SPKSBUTTONINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuSpksButtonIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuSpksButtonIndex, 0xFF, sizeof(UINT16) *SPKSBUTTONINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuDepotGateIndex = (UINT16*)MyNew(sizeof(UINT16) *DEPOTGATEINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_DEPOT_GATE_ID, DEPOTGATEINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuDepotGateIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuDepotGateIndex, 0xFF, sizeof(UINT16) *DEPOTGATEINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuTunnelIndex = (UINT16*)MyNew(sizeof(UINT16) *TUNNELINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_TUNNEL_ID, TUNNELINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuTunnelIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuTunnelIndex, 0xFF, sizeof(UINT16) *TUNNELINDEXNUM + 2);					
			}
	
			pDsuEmapStu->dsuEmapIndexStru->dsuZCIndex = (UINT16*)MyNew(sizeof(UINT16) *ZCINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_ZC_ID, ZCINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuZCIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuZCIndex, 0xFF, sizeof(UINT16) *ZCINDEXNUM + 2);					
			}
			/*20180817 lp 合入新版查询函数，没有下面这张表，关闭此段代码*/
			#if 0
			pDsuEmapStu->dsuEmapIndexStru->dsuObsProectZoneIndex = (UINT16*)MyNew(sizeof(UINT16) *OBSPROECTZONE_NUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_OBSPROECTZONE_ID, OBSPROECTZONE_NUM + 1);
			if ((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuObsProectZoneIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuObsProectZoneIndex, 0xFF, sizeof(UINT16) *OBSPROECTZONE_NUM + 2);
			}
			#endif
			pDsuEmapStu->dsuEmapIndexStru->dsuPowerlessZoneIndex = (UINT16*)MyNew(sizeof(UINT16) *POWERLESSZONE_NUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_POWERLESSZONE_ID, POWERLESSZONE_NUM + 1);
			if ((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuPowerlessZoneIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuPowerlessZoneIndex, 0xFF, sizeof(UINT16) *POWERLESSZONE_NUM + 2);
			}
			/*初始化临线停车区域表*/
			pDsuEmapStu->dsuEmapIndexStru->dsuAdjtStationIndex = (UINT16*)MyNew(sizeof(UINT16) *ADJTSTATIONINDEXNUM + 2);
			dquSetValue = dquSetEmapDataIndexArrayLen(EMAP_ADJTSTATION_ID, ADJTSTATIONINDEXNUM+1);
			if((NULL == pDsuEmapStu->dsuEmapIndexStru->dsuAdjtStationIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuEmapIndexStru->dsuAdjtStationIndex, 0xFF, sizeof(UINT16) *ADJTSTATIONINDEXNUM + 2);					
			}

			#if 0
			/*存放静态数据各个数据结构内存分配40*/
			pDsuEmapStu->dsuStaticHeadStru->dsuStaticDataVersion = (UINT8*)MyNew(sizeof(UINT8) *4);
			if(NULL == pDsuEmapStu->dsuStaticHeadStru->dsuStaticDataVersion)
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/ 
				memset(pDsuEmapStu->dsuStaticHeadStru->dsuStaticDataVersion, 0xFF, sizeof(UINT8) *4);					
			}
			#endif

			pDsuEmapStu->dsuStaticHeadStru->pLinkStru = (DSU_LINK_STRU*)MyNew(sizeof(DSU_LINK_STRU) *WLINKLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_LINK_ID, WLINKLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pLinkStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pLinkStru, 0xFF, sizeof(DSU_LINK_STRU) *WLINKLEN);					
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pPointStru = (DSU_POINT_STRU*)MyNew(sizeof(DSU_POINT_STRU) *WPOINTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_POINT_ID, WPOINTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pPointStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pPointStru, 0xFF, sizeof(DSU_POINT_STRU) *WPOINTLEN);					
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pStaticResSpeedStru = (DSU_STATIC_RES_SPEED_STRU*)MyNew(sizeof(DSU_STATIC_RES_SPEED_STRU) *WSTATICRESSPEEDLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_STATIC_RES_SPEED_ID, WSTATICRESSPEEDLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pStaticResSpeedStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pStaticResSpeedStru, 0xFF, sizeof(DSU_STATIC_RES_SPEED_STRU) *WSTATICRESSPEEDLEN);					
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pSignalStru = (DSU_SIGNAL_STRU*)MyNew(sizeof(DSU_SIGNAL_STRU) *WSIGNALLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_SIGNAL_ID, WSIGNALLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pSignalStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pSignalStru, 0xFF, sizeof(DSU_SIGNAL_STRU) *WSIGNALLEN); 				
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pBaliseStru = (DSU_BALISE_STRU*)MyNew(sizeof(DSU_BALISE_STRU) *WBALISELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_BALISE_ID, WBALISELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pBaliseStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pBaliseStru, 0xFF, sizeof(DSU_BALISE_STRU) *WBALISELEN); 				
			}		
	
			pDsuEmapStu->dsuStaticHeadStru->pAxleStru = (DSU_AXLE_STRU*)MyNew(sizeof(DSU_AXLE_STRU) *WAXLECOUNTERLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_AXLE_ID, WAXLECOUNTERLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pAxleStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pAxleStru, 0xFF, sizeof(DSU_AXLE_STRU) *WAXLECOUNTERLEN);					
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pScreenStru = (DSU_SCREEN_STRU*)MyNew(sizeof(DSU_SCREEN_STRU) *WSCREENLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_SCREEN_ID, WSCREENLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pScreenStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pScreenStru, 0xFF, sizeof(DSU_SCREEN_STRU) *WSCREENLEN); 				
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pStationStru = (DSU_STATION_STRU*)MyNew(sizeof(DSU_STATION_STRU) *WSTATIONLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_STATION_ID, WSTATIONLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pStationStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pStationStru, 0xFF, sizeof(DSU_STATION_STRU) *WSTATIONLEN);					
			}	
	
			pDsuEmapStu->dsuStaticHeadStru->pEmergStopStru = (DSU_EMERG_STOP_STRU*)MyNew(sizeof(DSU_EMERG_STOP_STRU) *WEMERGSTOPLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_EMERG_STOP_BUTTON_ID, WEMERGSTOPLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pEmergStopStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pEmergStopStru, 0xFF, sizeof(DSU_EMERG_STOP_STRU) *WEMERGSTOPLEN);					
			}		
	
			pDsuEmapStu->dsuStaticHeadStru->pStopPointStru = (DSU_STOPPOINT_STRU*)MyNew(sizeof(DSU_STOPPOINT_STRU) *WSTOPPOINTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_STOPPOINT_ID, WSTOPPOINTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pStopPointStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pStopPointStru, 0xFF, sizeof(DSU_STOPPOINT_STRU) *WSTOPPOINTLEN);					
			}	
			
			pDsuEmapStu->dsuStaticHeadStru->pRunLevelStru = (DSU_RUN_LEVEL_STRU*)MyNew(sizeof(DSU_RUN_LEVEL_STRU) *WRUNLEVELLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_RUN_LEVEL_ID, WRUNLEVELLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pRunLevelStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pRunLevelStru, 0xFF, sizeof(DSU_RUN_LEVEL_STRU) *WRUNLEVELLEN);					
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pGradeStru = (DSU_GRADE_STRU*)MyNew(sizeof(DSU_GRADE_STRU) *WGRADELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_GRADE_ID, WGRADELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pGradeStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pGradeStru, 0xFF, sizeof(DSU_GRADE_STRU) *WGRADELEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pExitRouteStru = (DSU_EXIT_ROUTE_STRU*)MyNew(sizeof(DSU_EXIT_ROUTE_STRU) *WEXITROUTELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_EXIT_ROUTE_ID, WEXITROUTELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pExitRouteStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pExitRouteStru, 0xFF, sizeof(DSU_EXIT_ROUTE_STRU) *WEXITROUTELEN);		
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pTrainLineStru = (DSU_TRAIN_LINE_STRU*)MyNew(sizeof(DSU_TRAIN_LINE_STRU) *WTRAINLINELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_TRAIN_LINE_ID, WTRAINLINELEN);
			if((NULL ==pDsuEmapStu->dsuStaticHeadStru->pTrainLineStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pTrainLineStru, 0xFF, sizeof(DSU_TRAIN_LINE_STRU) *WTRAINLINELEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pAxleSgmtStru = (DSU_AXLE_SGMT_STRU*)MyNew(sizeof(DSU_AXLE_SGMT_STRU) *WAXLESGMTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_AXLE_SGMT_ID, WAXLESGMTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pAxleSgmtStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pAxleSgmtStru, 0xFF, sizeof(DSU_AXLE_SGMT_STRU) *WAXLESGMTLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pLogicSgmtStru = (DSU_LOGIC_SGMT_STRU*)MyNew(sizeof(DSU_LOGIC_SGMT_STRU) *WLOGICSGMTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_LOGIC_SGMT_ID, WLOGICSGMTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pLogicSgmtStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pLogicSgmtStru, 0xFF, sizeof(DSU_LOGIC_SGMT_STRU) *WLOGICSGMTLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pRouteStru = (DSU_ROUTE_STRU*)MyNew(sizeof(DSU_ROUTE_STRU) *WROUTELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_ROUTE_ID, WROUTELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pRouteStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pRouteStru, 0xFF, sizeof(DSU_ROUTE_STRU) *WROUTELEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pProtectLinkStru = (DSU_PROTECT_LINK_STRU*)MyNew(sizeof(DSU_PROTECT_LINK_STRU) *WPROTECTLINKLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_PROTECT_LINK_ID, WPROTECTLINKLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pProtectLinkStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pProtectLinkStru, 0xFF, sizeof(DSU_PROTECT_LINK_STRU) *WPROTECTLINKLEN); 	
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pCBTCAccessLinkStru = (DSU_CBTC_ACCESS_LINK_STRU*)MyNew(sizeof(DSU_CBTC_ACCESS_LINK_STRU) *WCBTCACCESSLINKLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_CBTC_ACCESS_LINK_ID, WCBTCACCESSLINKLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pCBTCAccessLinkStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pCBTCAccessLinkStru, 0xFF, sizeof(DSU_CBTC_ACCESS_LINK_STRU) *WCBTCACCESSLINKLEN);		
			}		
			
			pDsuEmapStu->dsuStaticHeadStru->pBLOCAccessLinkStru = (DSU_BLOC_ACCESS_LINK_STRU*)MyNew(sizeof(DSU_BLOC_ACCESS_LINK_STRU) *WBLOCACCESSLINKLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_BLOC_ACCESS_LINK_ID, WBLOCACCESSLINKLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pBLOCAccessLinkStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pBLOCAccessLinkStru, 0xFF, sizeof(DSU_BLOC_ACCESS_LINK_STRU) *WBLOCACCESSLINKLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pDefaultRoutStru = (DSU_DEFAULTROUTE_STRU*)MyNew(sizeof(DSU_DEFAULTROUTE_STRU) *WDEFAULTROUTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_DEFAULT_ROUTE_ID, WDEFAULTROUTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pDefaultRoutStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pDefaultRoutStru, 0xFF, sizeof(DSU_DEFAULTROUTE_STRU) *WDEFAULTROUTLEN); 	
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pARInStru = (DSU_AR_IN_STRU*)MyNew(sizeof(DSU_AR_IN_STRU) *WARINLEN);	
			if(NULL == pDsuEmapStu->dsuStaticHeadStru->pARInStru)
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pARInStru, 0xFF, sizeof(DSU_AR_IN_STRU) *WARINLEN);		
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pAROutStru = (DSU_AR_OUT_STRU*)MyNew(sizeof(DSU_AR_OUT_STRU) *WAROUTLEN);
			if(NULL == pDsuEmapStu->dsuStaticHeadStru->pAROutStru)
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pAROutStru, 0xFF, sizeof(DSU_AR_OUT_STRU) *WAROUTLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pZCDividPointStru = (DSU_ZC_DIVID_POINT_STRU*)MyNew(sizeof(DSU_ZC_DIVID_POINT_STRU) *WZCDIVIDPOINTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_ZC_DIVID_POINT_ID, WZCDIVIDPOINTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pZCDividPointStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pZCDividPointStru, 0xFF, sizeof(DSU_ZC_DIVID_POINT_STRU) *WZCDIVIDPOINTLEN); 	
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pATSStruStru = (DSU_ATS_STRU*)MyNew(sizeof(DSU_ATS_STRU) *WATSLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_ATS_ID, WATSLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pATSStruStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pATSStruStru, 0xFF, sizeof(DSU_ATS_STRU) *WATSLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pCIStruStru = (DSU_CI_STRU*)MyNew(sizeof(DSU_CI_STRU) *WCILEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_CI_ID, WCILEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pCIStruStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pCIStruStru, 0xFF, sizeof(DSU_CI_STRU) *WCILEN); 	
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pARStru = (DSU_AR_STRU*)MyNew(sizeof(DSU_AR_STRU) *WARLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_AR_ID, WARLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pARStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pARStru, 0xFF, sizeof(DSU_AR_STRU) *WARLEN); 	
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pZCComZoneStru = (DSU_ZC_COM_ZONE_STRU*)MyNew(sizeof(DSU_ZC_COM_ZONE_STRU) *WZCCOMZONELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_ZC_COM_ZONE_ID, WZCCOMZONELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pZCComZoneStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pZCComZoneStru, 0xFF, sizeof(DSU_ZC_COM_ZONE_STRU) *WZCCOMZONELEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pConResSpeedStru = (DSU_CONRESSPEED_STRU*)MyNew(sizeof(DSU_CONRESSPEED_STRU) *WCONRESSPEEDLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_CONRESSPEED_ID, WCONRESSPEEDLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pConResSpeedStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pConResSpeedStru, 0xFF, sizeof(DSU_CONRESSPEED_STRU) *WCONRESSPEEDLEN);		
			}
			
			pDsuEmapStu->dsuStaticHeadStru->pConGradeStru = (DSU_CONGRADE_STRU*)MyNew(sizeof(DSU_CONGRADE_STRU) *WCONGRADELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_CONGRADE_ID, WCONGRADELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pConGradeStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pConGradeStru, 0xFF, sizeof(DSU_CONGRADE_STRU) *WCONGRADELEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pPhysicalSgmtStru = (DSU_PHYSICAL_SGMT_STRU*)MyNew(sizeof(DSU_PHYSICAL_SGMT_STRU) *WPHYSICALSGMTLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_PHYSICAL_SGMT_ID, WPHYSICALSGMTLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pPhysicalSgmtStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pPhysicalSgmtStru, 0xFF, sizeof(DSU_PHYSICAL_SGMT_STRU) *WPHYSICALSGMTLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pBlocTrgAreaStru = (DSU_BLOC_TRG_AREA_STRU*)MyNew(sizeof(DSU_BLOC_TRG_AREA_STRU) *WBLOCTRGAREALEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_BLOC_TRG_AREA_ID, WBLOCTRGAREALEN);
			if((NULL ==pDsuEmapStu->dsuStaticHeadStru->pBlocTrgAreaStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pBlocTrgAreaStru, 0xFF, sizeof(DSU_BLOC_TRG_AREA_STRU) *WBLOCTRGAREALEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pCbtcTrgAreaStru = (DSU_CBTC_TRG_AREA_STRU*)MyNew(sizeof(DSU_CBTC_TRG_AREA_STRU) *WCBTCTRGAREALEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_CBTC_TRG_AREA_ID, WCBTCTRGAREALEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pCbtcTrgAreaStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pCbtcTrgAreaStru, 0xFF, sizeof(DSU_CBTC_TRG_AREA_STRU) *WCBTCTRGAREALEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pPlatFormStru = (DSU_PLAT_FORM_STRU*)MyNew(sizeof(DSU_PLAT_FORM_STRU) *WPLATFORMLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_PLAT_FORM_ID, WPLATFORMLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pPlatFormStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pPlatFormStru, 0xFF, sizeof(DSU_PLAT_FORM_STRU) *WPLATFORMLEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pComeGoRouteStru = (DSU_COME_GO_STRU*)MyNew(sizeof(DSU_COME_GO_STRU) *WCOMEGOROUTELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_COME_GO_ID, WCOMEGOROUTELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pComeGoRouteStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pComeGoRouteStru, 0xFF, sizeof(DSU_COME_GO_STRU) *WCOMEGOROUTELEN);		
			}
	
			pDsuEmapStu->dsuStaticHeadStru->pFloodGateStru = (DSU_FLOOD_GATE_STRU*)MyNew(sizeof(DSU_FLOOD_GATE_STRU) *WFLOODGATELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_FLOOD_GATE_ID, WFLOODGATELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pFloodGateStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pFloodGateStru, 0xFF, sizeof(DSU_FLOOD_GATE_STRU) *WFLOODGATELEN);		
			}		
	
			pDsuEmapStu->dsuStaticHeadStru->pSpksButtonStru = (DSU_SPKS_BUTTON_STRU*)MyNew(sizeof(DSU_SPKS_BUTTON_STRU) *WSPKSBUTTONLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_SPKS_BUTTON_ID, WSPKSBUTTONLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pSpksButtonStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pSpksButtonStru, 0xFF, sizeof(DSU_SPKS_BUTTON_STRU) *WSPKSBUTTONLEN);		
			}			
	
			pDsuEmapStu->dsuStaticHeadStru->pDepotGateStru = (DSU_DEPOT_GATE_STRU*)MyNew(sizeof(DSU_DEPOT_GATE_STRU) *WDEPOTGATELEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_DEPOT_GATE_ID, WDEPOTGATELEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pDepotGateStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pDepotGateStru, 0xFF, sizeof(DSU_DEPOT_GATE_STRU) *WDEPOTGATELEN);		
			}			
	
			pDsuEmapStu->dsuStaticHeadStru->pTunnelStru = (DSU_TUNNEL_STRU*)MyNew(sizeof(DSU_TUNNEL_STRU) *WTUNNELLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_TUNNEL_ID, WTUNNELLEN);
			if((NULL == pDsuEmapStu->dsuStaticHeadStru->pTunnelStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pTunnelStru, 0xFF, sizeof(DSU_TUNNEL_STRU) *WTUNNELLEN); 	
			}	
	
			pDsuEmapStu->dsuStaticHeadStru->pZCStru = (DSU_ZC_STRU*)MyNew(sizeof(DSU_ZC_STRU) *WZCLEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_ZC_ID, WZCLEN);
			if((NULL ==pDsuEmapStu->dsuStaticHeadStru->pZCStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pDsuEmapStu->dsuStaticHeadStru->pZCStru, 0xFF, sizeof(DSU_ZC_STRU) *WZCLEN); 	
			}
			/*20180817 lp 合入新版查询函数，没有下面这张表，关闭此段代码*/
			#if 0
			pDsuEmapStu->dsuStaticHeadStru->pObsProectZoneStru = (DSU_OBS_PROECT_ZONE_STRU*)MyNew(sizeof(DSU_OBS_PROECT_ZONE_STRU) *OBSPROECTZONE_LEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_OBSPROECTZONE_ID, OBSPROECTZONE_LEN);
			if ((NULL == pDsuEmapStu->dsuStaticHeadStru->pObsProectZoneStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(pDsuEmapStu->dsuStaticHeadStru->pObsProectZoneStru, 0xFF, sizeof(DSU_OBS_PROECT_ZONE_STRU) *OBSPROECTZONE_LEN);
			}
			#endif
			pDsuEmapStu->dsuStaticHeadStru->pPowerlessZoneStru = (DSU_POWERLESS_ZONE_STRU*)MyNew(sizeof(DSU_POWERLESS_ZONE_STRU) *POWERLESSZONE_LEN);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_POWERLESSZONE_ID, POWERLESSZONE_LEN);
			if ((NULL == pDsuEmapStu->dsuStaticHeadStru->pPowerlessZoneStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/
				memset(pDsuEmapStu->dsuStaticHeadStru->pPowerlessZoneStru, 0xFF, sizeof(DSU_POWERLESS_ZONE_STRU) *POWERLESSZONE_LEN);
			}
			/*临线停车区域表结构体初始化*/
			pDsuEmapStu->dsuStaticHeadStru->pAdjtStationStru = (DSU_AdjtStation_STRU *)MyNew(sizeof(DSU_AdjtStation_STRU)*ADJTSTATIONZONE);
			dquSetValue = dquSetEmapDataStruArrayLen(EMAP_ADJTSTATION_ID,ADJTSTATIONZONE);
			if ((NULL == pDsuEmapStu->dsuStaticHeadStru->pAdjtStationStru)||(0 == dquSetValue))
			{
				InitValue = 0;
			} 
			else
			{
				/*初始化为0xFF*/
				memset(pDsuEmapStu->dsuStaticHeadStru->pAdjtStationStru, 0xFF, sizeof(DSU_AdjtStation_STRU)*ADJTSTATIONZONE);
			}
		}
	}

	return InitValue;
}

/*************************************************************************************************************
* 功能描述:    静态数据长度信息结构体初始化
* 输入参数:    DSU_IDMAP_STRU *pIdEmapStru 编号对照表总结构体
* 输出参数:    无.
* 全局变量:    无.
* 返回值  :    0:失败;1成功
*************************************************************************************************************/
UINT8 vDquIdmapStructInit(DSU_IDMAP_STRU *pIdEmapStru)
{
	UINT8 InitValue = 1; /*函数返回值*/
	UINT8 dquSetValue = 0;

	/*入口参数检查*/
  	if(NULL == pIdEmapStru)
    {
        /*入口数据异常,查询失败*/
		InitValue = 0;
    }
	else
	{
		pIdEmapStru->dsuIdmapIndexStru = (DSU_IDMAP_INDEX_STRU*)MyNew(sizeof(DSU_IDMAP_INDEX_STRU));
		if(NULL == pIdEmapStru->dsuIdmapIndexStru)
		{
			InitValue = 0;
		}
		else
		{
			/*初始化为0*/  
			memset(pIdEmapStru->dsuIdmapIndexStru, 0, sizeof(DSU_IDMAP_INDEX_STRU));
		}
		
		pIdEmapStru->dsuIdmapLenStru = (DSU_IDMAP_LEN_STRU*)MyNew(sizeof(DSU_IDMAP_LEN_STRU));
		if(NULL == pIdEmapStru->dsuIdmapLenStru)
		{
			InitValue = 0;
		}
		else
		{
			/*初始化为0*/  
			memset(pIdEmapStru->dsuIdmapLenStru, 0, sizeof(DSU_IDMAP_LEN_STRU));
		}
		
		pIdEmapStru->dsuStaticIdmapStru = (DSU_STATIC_IDMAP_STRU*)MyNew(sizeof(DSU_STATIC_IDMAP_STRU));
		if(NULL == pIdEmapStru->dsuStaticIdmapStru)
		{
			InitValue = 0;
		}
		else
		{
			/*初始化为0*/  
			memset(pIdEmapStru->dsuStaticIdmapStru, 0, sizeof(DSU_STATIC_IDMAP_STRU));
		}
		
		if(1 == InitValue)
		{
			/*为管理函数建立全局索引12*/
			pIdEmapStru->dsuIdmapIndexStru->dsuZCNumIndex = (UINT16*)MyNew(sizeof(UINT16) *ZCNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_ZC_ID, ZCNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuZCNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuZCNumIndex, 0xFF, sizeof(UINT16) *ZCNUMINDEXSIZE + 2);
			}	
		
			pIdEmapStru->dsuIdmapIndexStru->dsuCINumIndex = (UINT16*)MyNew(sizeof(UINT16) *CINUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_CI_ID, CINUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuCINumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuCINumIndex, 0xFF, sizeof(UINT16) *CINUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuATSNumIndex = (UINT16*)MyNew(sizeof(UINT16) *ATSNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_ATS_ID, ATSNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuATSNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuATSNumIndex, 0xFF, sizeof(UINT16) *ATSNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuSignalNumIndex = (UINT16*)MyNew(sizeof(UINT16) *SIGNALNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_SIGNAL_ID, SIGNALNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuSignalNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuSignalNumIndex, 0xFF, sizeof(UINT16) *SIGNALNUMINDEXSIZE + 2);
			}
			
			pIdEmapStru->dsuIdmapIndexStru->dsuTrackSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) *TRACKSECNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_TRACKSEC_ID, TRACKSECNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuTrackSecNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuTrackSecNumIndex, 0xFF, sizeof(UINT16) *TRACKSECNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuSwitchNumIndex = (UINT16*)MyNew(sizeof(UINT16) *SWITCHNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_SWITCH_ID, SWITCHNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuSwitchNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuSwitchNumIndex, 0xFF, sizeof(UINT16) *SWITCHNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuStationNumIndex = (UINT16*)MyNew(sizeof(UINT16) *STATIONNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_STATION_ID, STATIONNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuStationNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuStationNumIndex, 0xFF, sizeof(UINT16) *STATIONNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuEmergBtnIndex = (UINT16*)MyNew(sizeof(UINT16) *EMERGBTNNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_EMERGBTN_ID, EMERGBTNNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuEmergBtnIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuEmergBtnIndex, 0xFF, sizeof(UINT16) *EMERGBTNNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuBaliseNumIndex = (UINT16*)MyNew(sizeof(UINT16) *BALISENUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_BALISE_ID, BALISENUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuBaliseNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuBaliseNumIndex, 0xFF, sizeof(UINT16) *BALISENUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuPSDoorNumIndex = (UINT16*)MyNew(sizeof(UINT16) *PSDNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_PSD_ID, PSDNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuPSDoorNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuPSDoorNumIndex, 0xFF, sizeof(UINT16) *PSDNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuVobcNumIndex = (UINT16*)MyNew(sizeof(UINT16) *VOBCNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_VOBC_ID, VOBCNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuVobcNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuVobcNumIndex, 0xFF, sizeof(UINT16) *VOBCNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuFPDoorNumIndex = (UINT16*)MyNew(sizeof(UINT16) *FPDNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_FPD_ID, FPDNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuFPDoorNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuFPDoorNumIndex, 0xFF, sizeof(UINT16) *FPDNUMINDEXSIZE + 2);
			}
			
			pIdEmapStru->dsuIdmapIndexStru->dsuDsuNumIndex = (UINT16*)MyNew(sizeof(UINT16) *DSUNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_DSU_ID, DSUNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuDsuNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuDsuNumIndex, 0xFF, sizeof(UINT16) *DSUNUMINDEXSIZE + 2);
			}
	
			pIdEmapStru->dsuIdmapIndexStru->dsuLogicSecNumIndex = (UINT16*)MyNew(sizeof(UINT16) *LOGICSECNUMINDEXSIZE + 2);
			dquSetValue = dquSetIdmapDataIndexArrayLen(IDMAP_LOGICSEC_ID, LOGICSECNUMINDEXSIZE+1);
			if((NULL == pIdEmapStru->dsuIdmapIndexStru->dsuLogicSecNumIndex) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuIdmapIndexStru->dsuLogicSecNumIndex, 0xFF, sizeof(UINT16) *LOGICSECNUMINDEXSIZE + 2);
			}
	
			/*存放静态数据各个数据结构体内存分配12*/
			pIdEmapStru->dsuStaticIdmapStru->pZCNumStru = (DSU_ZC_NUM_STRU*)MyNew(sizeof(DSU_ZC_NUM_STRU) *WZCNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_ZC_ID, WZCNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pZCNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pZCNumStru, 0xFF, sizeof(DSU_ZC_NUM_STRU) *WZCNUMLEN);
			}
	
			pIdEmapStru->dsuStaticIdmapStru->pCINumStru = (DSU_CI_NUM_STRU*)MyNew(sizeof(DSU_CI_NUM_STRU) *WCINUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_CI_ID, WCINUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pCINumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pCINumStru, 0xFF, sizeof(DSU_CI_NUM_STRU) *WCINUMLEN);
			}
	
			pIdEmapStru->dsuStaticIdmapStru->pATSNumStru = (DSU_ATS_NUM_STRU*)MyNew(sizeof(DSU_ATS_NUM_STRU) *WATSNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_ATS_ID, WATSNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pATSNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pATSNumStru, 0xFF, sizeof(DSU_ATS_NUM_STRU) *WATSNUMLEN);
			}
			
			pIdEmapStru->dsuStaticIdmapStru->pSignalNumStru = (DSU_SIGNAL_NUM_STRU*)MyNew(sizeof(DSU_SIGNAL_NUM_STRU) *WSIGNALNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_SIGNAL_ID, WSIGNALNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pSignalNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pSignalNumStru, 0xFF, sizeof(DSU_SIGNAL_NUM_STRU) *WSIGNALNUMLEN);
			}
	
			pIdEmapStru->dsuStaticIdmapStru->pTrackSecNumStru = (DSU_TRACKSEC_NUM_STRU*)MyNew(sizeof(DSU_TRACKSEC_NUM_STRU) *WTRACKSECNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_TRACKSEC_ID, WTRACKSECNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pTrackSecNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pTrackSecNumStru, 0xFF, sizeof(DSU_TRACKSEC_NUM_STRU) *WTRACKSECNUMLEN);
			}		
	
			pIdEmapStru->dsuStaticIdmapStru->pSwitchNumStru = (DSU_SWITCH_NUM_STRU*)MyNew(sizeof(DSU_SWITCH_NUM_STRU) *WSWITCHNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_SWITCH_ID, WSWITCHNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pSwitchNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pSwitchNumStru, 0xFF, sizeof(DSU_SWITCH_NUM_STRU) *WSWITCHNUMLEN);
			}		
	
			pIdEmapStru->dsuStaticIdmapStru->pStationNumStru = (DSU_STATION_NUM_STRU*)MyNew(sizeof(DSU_STATION_NUM_STRU) *WSTATIONNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_STATION_ID, WSTATIONNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pStationNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pStationNumStru, 0xFF, sizeof(DSU_STATION_NUM_STRU) *WSTATIONNUMLEN);
			}	
	
			pIdEmapStru->dsuStaticIdmapStru->pEmergBtnNumStru = (DSU_EMERGBTN_NUM_STRU*)MyNew(sizeof(DSU_EMERGBTN_NUM_STRU) *WEMERGBTNNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_EMERGBTN_ID, WEMERGBTNNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pEmergBtnNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pEmergBtnNumStru, 0xFF, sizeof(DSU_EMERGBTN_NUM_STRU) *WEMERGBTNNUMLEN);
			}	
	
			pIdEmapStru->dsuStaticIdmapStru->pBaliseNumStru = (DSU_BALISE_NUM_STRU*)MyNew(sizeof(DSU_BALISE_NUM_STRU) *WBALISENUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_BALISE_ID, WBALISENUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pBaliseNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pBaliseNumStru, 0xFF, sizeof(DSU_BALISE_NUM_STRU) *WBALISENUMLEN);
			}			
			
			pIdEmapStru->dsuStaticIdmapStru->pPSDNumStru = (DSU_PSD_NUM_STRU*)MyNew(sizeof(DSU_PSD_NUM_STRU) *WPSDOORNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_PSD_ID, WPSDOORNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pPSDNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pPSDNumStru, 0xFF, sizeof(DSU_PSD_NUM_STRU) *WPSDOORNUMLEN);
			}
			
			pIdEmapStru->dsuStaticIdmapStru->pVobcNumStru = (DSU_VOBC_NUM_STRU*)MyNew(sizeof(DSU_VOBC_NUM_STRU) *WVOBCNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_VOBC_ID, WVOBCNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pVobcNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pVobcNumStru, 0xFF, sizeof(DSU_VOBC_NUM_STRU) *WVOBCNUMLEN);
			}
	
			pIdEmapStru->dsuStaticIdmapStru->pFPDoorNumStru = (DSU_FPD_NUM_STRU*)MyNew(sizeof(DSU_FPD_NUM_STRU) *WFPDOORNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_FPD_ID, WFPDOORNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pFPDoorNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pFPDoorNumStru, 0xFF, sizeof(DSU_FPD_NUM_STRU) *WFPDOORNUMLEN);
			}
	
			pIdEmapStru->dsuStaticIdmapStru->pDsuNumStru = (DSU_DSU_NUM_STRU*)MyNew(sizeof(DSU_DSU_NUM_STRU) *WDSUNUMLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_DSU_ID, WDSUNUMLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pDsuNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pDsuNumStru, 0xFF, sizeof(DSU_DSU_NUM_STRU) *WDSUNUMLEN);
			}
	
			pIdEmapStru->dsuStaticIdmapStru->pLogicSecNumStru = (DSU_LOGICSEC_NUM_STRU*)MyNew(sizeof(DSU_LOGICSEC_NUM_STRU) *WLOGICLEN);
			dquSetValue = dquSetIdmapDataStruArrayLen(IDMAP_LOGICSEC_ID, WLOGICLEN);
			if((NULL == pIdEmapStru->dsuStaticIdmapStru->pLogicSecNumStru) || (0 == dquSetValue))
			{
				InitValue = 0;
			}
			else
			{
				/*初始化为0xFF*/  
				memset(pIdEmapStru->dsuStaticIdmapStru->pLogicSecNumStru, 0xFF, sizeof(DSU_LOGICSEC_NUM_STRU) *WLOGICLEN);
			}
		}
	}

	return InitValue;
}

