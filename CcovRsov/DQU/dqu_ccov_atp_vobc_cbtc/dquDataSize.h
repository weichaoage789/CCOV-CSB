/************************************************************************
*
* 文件名   ：  duqDataSize.h
* 版权说明 ：  北京交控科技科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2016.04.25
* 作者     ：  yunjun.ren
* 功能描述 ： 查询函数结构体内存分配大小宏定义
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/

#ifndef _DQU_DATA_SIZE_H_
#define _DQU_DATA_SIZE_H_

#include "dsuStruct.h"
#include "dsuIdmapStruct.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 静态数据长度信息结构体39*/
#define  WLINKLEN					1500            /*LINK表*/
#define  WPOINTLEN               	300				/*数据库中道岔的结构体的数量*/
#define  WSTATICRESSPEEDLEN         1500			/*数据库中静态限速的结构体的数量*/	
#define  WSIGNALLEN               	700				/*数据库中信号机的结构体的数量*/
#define  WBALISELEN               	2000			/*数据库中应答器的结构体的数量*/
#define  WAXLECOUNTERLEN            1400			/*数据库中计轴器的结构体的数量*/
#define  WSCREENLEN               	200				/*数据库中安全屏蔽门的结构体的数量*/
#define  WSTATIONLEN               	400				/*数据库中停车区域的结构体的数量*/
#define  WEMERGSTOPLEN              160				/*数据库中紧急停车按钮的结构体的数量*/
#define  WSTOPPOINTLEN              600				/*数据库中停车点的结构体的数量*/
#define  WRUNLEVELLEN               300            	/*数据库中运行级别结构体数量*/
#define  WGRADELEN               	800				/*数据库中坡度区域的结构体的数量*/	
#define  WEXITROUTELEN              120				/*数据库中退出CBTC 的结构体的数量*/	
#define  WTRAINLINELEN              50				/*数据库中车辆段的结构体的数量*/	
#define  WAXLESGMTLEN               1400			/*数据库中计轴区段的结构体的数量*/		
#define  WLOGICSGMTLEN              2000			/*数据库中逻辑区段的结构体的数量*/	
#define  WROUTELEN               	1200			/*数据库中进路的结构体的数量*/	
#define  WPROTECTLINKLEN            400				/*数据库中进路保护区段的结构体的数量*/	
#define  WCBTCACCESSLINKLEN         600				/*数据库中进路CBTC接近区段的结构体的数量*/	
#define  WBLOCACCESSLINKLEN         600				/*数据库中进路BLOC接近区段的结构体的数量*/	
#define  WDEFAULTROUTLEN            200				/*数据库中默认运行序列的结构体的数量*/	
#define  WARINLEN               	50				/*数据库中无人折返进入结构体数量*/
#define  WAROUTLEN               	50				/*数据库中无人折返退出结构体数量*/
#define  WZCDIVIDPOINTLEN           100          	/*ZC分界点表*/    /* 互联互通不使用，可以不分配 */
#define  WATSLEN               		50				/*数据库中ATS的结构体的数量*/	
#define  WCILEN               		50				/*数据库中CI的结构体的数量*/
#define  WARLEN               		50              /*数据库中无人折返结构体数量*/ /*对应新增结构体DSU_AR_STRU*/
#define  WZCCOMZONELEN              150           	/*ZC共管区域表*/     /* 互联互通不使用，可以不分配 */
#define  WCONRESSPEEDLEN            25              /*统一限速结构体的数量*/
#define  WCONGRADELEN               25              /*统一坡速结构体的数量*/ 
#define  WPHYSICALSGMTLEN           2000			/*物理区段结构体的数量*/ 
#define  WBLOCTRGAREALEN            1000			/*点式触发区段结构体的数量*/
#define  WCBTCTRGAREALEN            1000			/*CBTC 触发区段结构体的数量*/
#define  WPLATFORMLEN               160				/*站台表结构体的数量*/
#define  WCOMEGOROUTELEN			100     		/*往复进路表的结构体数量*/
#define  WFLOODGATELEN              150				/*防淹门表的结构体数量*/
#define  WSPKSBUTTONLEN             200		 		/*SPKS按钮表的结构体数量*/
#define  WDEPOTGATELEN              200		   		/*车库门表的结构体数量*/
#define  WTUNNELLEN               	1500			/*隧道的结构体数量*/
#define  WZCLEN               		50				/*ZC的结构体数量*/
#define  OBSPROECTZONE_LEN          500             /*障碍物防护区域数量*/
#define  POWERLESSZONE_LEN          100             /*分相区数量*/
#define  ADJTSTATIONZONE			500			    /*临线停车区域的结构体数量*/

/*为管理函数建立全局索引数组,静态数据索引数组大小结构体定义39*/
#define   nLINKINDEXNUM              2000		    /*区段索引数组空间大小*/
#define   POINTINDEXNUM             500				/*道岔索引数组空间大小*/
#define   BALISEINDEXNUM            3000			/*应答器索引数组空间大小*/
#define   SIGNALINDEXNUM            900				/*信号机索引数组空间大小*/
#define   AXLEINDEXNUM              2000			/*计轴器索引数组空间大小*/
#define   SCREENINDEXNUM            300				/*安全屏蔽门索引数组空间大小*/
#define   STATIONINDEXNUM           400				/*停车区域索引数组空间大小*/
#define   EMERGSTOPINDEXNUM         170				/*紧急停车按钮索引数组空间大小*/
#define   AXLESGMTINDEXNUM          2000			/*计轴区段索引数组空间大小*/
#define   STOPPOINTINDEXNUM         600				/*停车点索引数组空间大小*/
#define   TRAINLINEINDEXNUM         25				/*车辆段索引数组空间大小*/
#define   EXITROUTEINDEXNUM         120				/*退出CBTC区域索引数组空间大小*/ 
#define   RUNLEVELINDEXNUM          300       		/*站间运行级别索引数组大小*/
#define   STATICRESSPEEDNUM         2000			/*线路静态限速索引数组空间大小*/
#define   GRADEINDEXNUM             800				/*坡度索引数组空间大小*/
#define   LOGICSGMTINDEXNUM         2400			/*逻辑区段索引数组空间大小*/
#define   ROUTEINDEXNUM             1200			/*进路索引数组空间大小*/
#define   PROTECTLINKINDEXNUM       600				/*保护区段索引数组空间大小*/
#define   CBTCACCESSLINKINDEXNUM    650				/*CBTC接近区段索引数组空间大小*/
#define   BLOCACCESSLINKINDEXNUM    650				/*BLOC接近区段索引数组空间大小*/
#define   DEFAULTROUTEINDEXNUM      200				/*默认行车序列索引数组空间大小*/
#define   ARINNUM                   50				/*无人折返进入区域索引数组空间大小*/
#define   AROUTNUM                  50				/*无人折返退出区域索引数组空间大小*/
#define   ATSINDEXNUM               50				/*ATS索引数组空间大小*/
#define   CIINDEXNUM                50				/*CI区段索引数组空间大小*/
#define   ARNUM                		50             	/*无人折返索引数组大小*/  
#define   ZCCOMZONENUM              200         	/*ZC共管区段索引数组大小*/  /* 互联互通不使用，可以不分配 */
#define   CONRESSPEEDINDEXNUM       30				/*统一限速索引数组大小*/ 
#define   CONGRADEINDEXNUM          30				/*统一坡度索引数组大小*/ 
#define   ZCDIVIDPOINTINDEXNUM      150 			/*ZC分界点索引数组大小*/  /* 互联互通不使用，可以不分配 */
#define   PHYSICALSGMTINDEXNUM      2000     		/*物理区段索引数组大小*/
#define   BLOCTRGAREAINDEXNUM       1000     		/*点式触发区段索引数组大小 */
#define   CBTCTRGAREAINDEXNUM       1000     		/*CBTC触发区段索引数组大小*/
#define   PLATFORMINDEXNUM          250     		/*站台索引数组大小*/
#define   COMEGOINDEXNUM            500         	/*往复进路索引数组大小*/
#define   FLOODGATEINDEXNUM         250				/*防淹门索引数组大小*/
#define   SPKSBUTTONINDEXNUM        200				/*SPKS开关索引数组大小*/
#define   DEPOTGATEINDEXNUM         200				/*车库门索引数组大小*/
#define   TUNNELINDEXNUM            2000			/*隧道索引数组大小*/
#define   ZCINDEXNUM				60				/*ZC索引数组大小*/
#define   OBSPROECTZONE_NUM         500             /*障碍物防护区域索引数组大小*/
#define   POWERLESSZONE_NUM         100             /*分相区索引数组大小*/
#define  ADJTSTATIONINDEXNUM		500				/*临线停车区域索引数组大小*/

/*为管理函数建立全局索引12*/
#define   ZCNUMINDEXSIZE            60				/*ZC编号表索引数组空间大小*/
#define   CINUMINDEXSIZE            50				/*CI编号表索引数组空间大小*/
#define   ATSNUMINDEXSIZE           10				/*ATS编号表索引数组空间大小*/
#define   SIGNALNUMINDEXSIZE        900				/*信号机编号表索引数组空间大小*/
#define   TRACKSECNUMINDEXSIZE      2000			/*轨道区段表索引数组空间大小*/
#define   SWITCHNUMINDEXSIZE        500				/*道岔编号表索引数组空间大小*/
#define   STATIONNUMINDEXSIZE       100				/*站台编号表索引数组空间大小*/
#define   EMERGBTNNUMINDEXSIZE      200				/*紧急关闭按钮编号表索引数组空间大小*/
#define   BALISENUMINDEXSIZE        3000			/*应答器编号表索引数组空间大小*/
#define   PSDNUMINDEXSIZE           300				/*屏蔽门编号表索引数组空间大小*/
#define   VOBCNUMINDEXSIZE          200				/*VOBC编号表索引数组空间大小*/
#define   FPDNUMINDEXSIZE           100				/*防淹门编号表索引数组空间大小*/
#define   DSUNUMINDEXSIZE           50				/*DSU编号表索引数组空间大小*/
#define   LOGICSECNUMINDEXSIZE		2400			/*逻辑区段编号表索引数组空间大小*/
/*存放静态数据各个数据结构的头指针14*/	
#define WZCNUMLEN               	30				/*数据库中zc编号表的结构体的数量*/
#define WCINUMLEN               	50				/*数据库中/ci编号表的结构体的数量*/
#define WATSNUMLEN               	10				/*数据库中ats编号表的结构体的数量*/
#define WSIGNALNUMLEN               900				/*数据库中信号机编号表的结构体的数量*/
#define WTRACKSECNUMLEN             1500			/*数据库中轨道区段表的结构体的数量*/
#define WSWITCHNUMLEN               500				/*数据库中道岔编号表的结构体的数量*/
#define WSTATIONNUMLEN              100				/*数据库中站台编号表的结构体的数量*/
#define WEMERGBTNNUMLEN             200				/*数据库中/紧急关闭按钮编号表的结构体的数量*/
#define WBALISENUMLEN               2000			/*数据库中应答器编号表的结构体的数量*/
#define WPSDOORNUMLEN               200				/*数据库中屏蔽门编号表的结构体的数量*/
#define WVOBCNUMLEN               	200				/*数据库中vobc编号表的结构体的数量*/
#define WFPDOORNUMLEN               100				/*数据库中防淹门编号表的结构体的数量*/
#define WDSUNUMLEN					50				/*数据库中DSU编号表的结构体的数量*/
#define WLOGICLEN					2000			/*数据库中逻辑区段编号表的结构体的数量*/
/*************************************************************************************************************
* 功能描述:    静态数据长度信息结构体初始化
* 输入参数:    DSU_EMAP_STRU *pDsuEmapStu 电子地图总结构体
* 输出参数:    无.
* 全局变量:    无.
* 返回值  :    0:初始化失败;1:初始化成功
*************************************************************************************************************/
UINT8 vDquEmapStructInit(DSU_EMAP_STRU *pDsuEmapStu);

/*************************************************************************************************************
* 功能描述:    静态数据长度信息结构体初始化
* 输入参数:    DSU_IDMAP_STRU *pIdEmapStru 编号对照表总结构体
* 输出参数:    无.
* 全局变量:    无.
* 返回值  :    0:失败;1成功
*************************************************************************************************************/
UINT8 vDquIdmapStructInit(DSU_IDMAP_STRU *pIdEmapStru);

#ifdef __cplusplus
}
#endif

#endif
