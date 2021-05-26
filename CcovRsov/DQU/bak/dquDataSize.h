/************************************************************************
*@ �ļ���	    �� duqDataSize.h
*@ ��Ȩ˵��	��
*@ �汾��	    �� 1.0
*@ ����ʱ��	�� 2020.4
*@ ����		    �� ��ѯ�����ṹ���ڴ�����С�궨��
*@ ��������	��
*@ ʹ��ע��	��
*@ �޸ļ�¼	��

************************************************************************/

#ifndef _DQU_DATA_SIZE_H_
#define _DQU_DATA_SIZE_H_

#include "common/CommonTypes.h"
#include "dsuStruct.h"
#include "dsuIdmapStruct.h"


	/* ��̬���ݳ�����Ϣ�ṹ��39*/
#define  WLINKLEN					1500            /*LINK��*/
#define  WPOINTLEN               	500				/*���ݿ��е���Ľṹ�������*/
#define  WSTATICRESSPEEDLEN         2000			/*���ݿ��о�̬���ٵĽṹ�������*/	
#define  WSIGNALLEN               	1000			/*���ݿ����źŻ��Ľṹ�������*/
#define  WBALISELEN               	2000			/*���ݿ���Ӧ�����Ľṹ�������*/
#define  WAXLECOUNTERLEN            1400			/*���ݿ��м������Ľṹ�������*/
#define  WSCREENLEN               	300				/*���ݿ��а�ȫ�����ŵĽṹ�������*/
#define  WSTATIONLEN               	500				/*���ݿ���ͣ������Ľṹ�������*/
#define  WEMERGSTOPLEN              200				/*���ݿ��н���ͣ����ť�Ľṹ�������*/
#define  WSTOPPOINTLEN              800				/*���ݿ���ͣ����Ľṹ�������(ͨ��������ѯ�����Ա���ͣ�����ʼ��ʱ��ֽ��ͣ����ռ���ڴ�����)*/
#define  WRUNLEVELLEN               800            	/*���ݿ������м���ṹ������*/
#define  WGRADELEN               	1000			/*���ݿ����¶�����Ľṹ�������*/	
#define  WEXITROUTELEN              120				/*���ݿ����˳�CBTC �Ľṹ�������*/	
#define  WTRAINLINELEN              50				/*���ݿ��г����εĽṹ�������*/	
#define  WAXLESGMTLEN               1400			/*���ݿ��м������εĽṹ�������*/		
#define  WLOGICSGMTLEN              2000			/*���ݿ����߼����εĽṹ�������*/	
#define  WROUTELEN               	1500			/*���ݿ��н�·�Ľṹ�������*/	
#define  WPROTECTLINKLEN            600				/*���ݿ��н�·�������εĽṹ�������*/	
#define  WCBTCACCESSLINKLEN         600				/*���ݿ��н�·CBTC�ӽ����εĽṹ�������*/	
#define  WBLOCACCESSLINKLEN         600				/*���ݿ��н�·BLOC�ӽ����εĽṹ�������*/	
#define  WDEFAULTROUTLEN            200				/*���ݿ���Ĭ���������еĽṹ�������*/	
#define  WARINLEN               	50				/*���ݿ��������۷�����ṹ������*/
#define  WAROUTLEN               	50				/*���ݿ��������۷��˳��ṹ������*/
#define  WZCDIVIDPOINTLEN           150          	/*ZC�ֽ���*/    /* ������ͨ��ʹ�ã����Բ����� */
#define  WATSLEN               		60				/*���ݿ���ATS�Ľṹ�������*/	
#define  WCILEN               		60				/*���ݿ���CI�Ľṹ�������*/
#define  WARLEN               		50              /*���ݿ��������۷��ṹ������*/ /*��Ӧ�����ṹ��DSU_AR_STRU*/
#define  WZCCOMZONELEN              200           	/*ZC���������*/     /* ������ͨ��ʹ�ã����Բ����� */
#define  WCONRESSPEEDLEN            30              /*ͳһ���ٽṹ�������*/
#define  WCONGRADELEN               30              /*ͳһ���ٽṹ�������*/ 
#define  WPHYSICALSGMTLEN           2000			/*�������νṹ�������*/ 
#define  WBLOCTRGAREALEN            1000			/*��ʽ�������νṹ�������*/
#define  WCBTCTRGAREALEN            1000			/*CBTC �������νṹ�������*/
#define  WPLATFORMLEN               300				/*վ̨��ṹ�������*/
#define  WCOMEGOROUTELEN			200     		/*������·��Ľṹ������*/
#define  WFLOODGATELEN              150				/*�����ű�Ľṹ������*/
#define  WSPKSBUTTONLEN             200		 		/*SPKS��ť��Ľṹ������*/
#define  WDEPOTGATELEN              200		   		/*�����ű�Ľṹ������*/
#define  WTUNNELLEN               	1500			/*����Ľṹ������*/
#define  WZCLEN               		60				/*ZC�Ľṹ������*/
/*add 20180322 xb V12.0���ݽṹ*/
#define  OBSPROECTZONE_LEN          500             /*�ϰ��������������*/

#define  MULTIPOINT_LEN				20				/*�࿪����ṹ������*/

#define  POWERLESSZONE_LEN          100             /*����������*/
/*add 20190122 zlj V15.0���ݽṹ*/
#define  CARSTOP_LEN          		100             /*����������*/
#define  ADJTSTATIONZONE			500			    /*����ͣ������Ľṹ������*/

/*Ϊ����������ȫ����������,��̬�������������С�ṹ�嶨��39*/
#define   nLINKINDEXNUM             2000		    /*������������ռ��С edit 20180322 xb �޸ĺ궨��*/
#define   POINTINDEXNUM             500				/*������������ռ��С*/
#define   BALISEINDEXNUM            3000			/*Ӧ������������ռ��С*/
#define   SIGNALINDEXNUM            1000		    /*�źŻ���������ռ��С*/
#define   AXLEINDEXNUM              2000			/*��������������ռ��С*/
#define   SCREENINDEXNUM            300				/*��ȫ��������������ռ��С*/
#define   STATIONINDEXNUM           500				/*ͣ��������������ռ��С*/
#define   EMERGSTOPINDEXNUM         200				/*����ͣ����ť��������ռ��С*/
#define   AXLESGMTINDEXNUM          2000			/*����������������ռ��С*/
#define   STOPPOINTINDEXNUM         1000			/*ͣ������������ռ��С*/
#define   TRAINLINEINDEXNUM         50				/*��������������ռ��С�����߲�ȱ���޸�,�ı�����ռ��СΪ50,Baix,20200525*/
#define   EXITROUTEINDEXNUM         120				/*�˳�CBTC������������ռ��С*/ 
#define   RUNLEVELINDEXNUM          800       		/*վ�����м������������С*/
#define   STATICRESSPEEDNUM         2000			/*��·��̬������������ռ��С*/
#define   GRADEINDEXNUM             1000		    /*�¶���������ռ��С*/
#define   LOGICSGMTINDEXNUM         2400			/*�߼�������������ռ��С*/
#define   ROUTEINDEXNUM             1500			/*��·��������ռ��С*/
#define   PROTECTLINKINDEXNUM       600				/*����������������ռ��С*/
#define   CBTCACCESSLINKINDEXNUM    650				/*CBTC�ӽ�������������ռ��С*/
#define   BLOCACCESSLINKINDEXNUM    650				/*BLOC�ӽ�������������ռ��С*/
#define   DEFAULTROUTEINDEXNUM      200				/*Ĭ���г�������������ռ��С*/
#define   ARINNUM                   50				/*�����۷�����������������ռ��С*/
#define   AROUTNUM                  50				/*�����۷��˳�������������ռ��С*/
#define   ATSINDEXNUM               60				/*ATS��������ռ��С*/
#define   CIINDEXNUM                60				/*CI��������ռ��С*/
#define   ARNUM                		50             	/*�����۷����������С*/  
#define   ZCCOMZONENUM              200         	/*ZC�����������������С*/  /* ������ͨ��ʹ�ã����Բ����� */
#define   CONRESSPEEDINDEXNUM       30				/*ͳһ�������������С*/ 
#define   CONGRADEINDEXNUM          30				/*ͳһ�¶����������С*/ 
#define   ZCDIVIDPOINTINDEXNUM      150 			/*ZC�ֽ�����������С*/  /* ������ͨ��ʹ�ã����Բ����� */
#define   PHYSICALSGMTINDEXNUM      2000     		/*�����������������С*/
#define   BLOCTRGAREAINDEXNUM       1000     		/*��ʽ�����������������С */
#define   CBTCTRGAREAINDEXNUM       1000     		/*CBTC�����������������С*/
#define   PLATFORMINDEXNUM          300     		/*վ̨���������С*/
#define   COMEGOINDEXNUM            500         	/*������·���������С*/
#define   FLOODGATEINDEXNUM         250				/*���������������С*/
#define   SPKSBUTTONINDEXNUM        200				/*SPKS�������������С*/
#define   DEPOTGATEINDEXNUM         200				/*���������������С*/
#define   TUNNELINDEXNUM            2000			/*������������С*/
#define   ZCINDEXNUM				60				/*ZC���������С*/
#define   OBSPROECTZONE_NUM         500             /*�ϰ�������������������С*/
#define   POWERLESSZONE_NUM         100             /*���������������С*/
#define   CARSTOPZONE_NUM			100				/*���������������С*/
#define   ADJTSTATIONINDEXNUM		500				/*����ͣ���������������С*/

	/*Ϊ����������ȫ������12*/
#define   ZCNUMINDEXSIZE            60				/*ZC��ű���������ռ��С*/
#define   CINUMINDEXSIZE            60				/*CI��ű���������ռ��С*/
#define   ATSNUMINDEXSIZE           60				/*ATS��ű���������ռ��С*/
#define   SIGNALNUMINDEXSIZE        900				/*�źŻ���ű���������ռ��С*/
#define   TRACKSECNUMINDEXSIZE      2000			/*������α���������ռ��С*/
#define   SWITCHNUMINDEXSIZE        500				/*�����ű���������ռ��С*/
#define   STATIONNUMINDEXSIZE       300				/*վ̨��ű���������ռ��С*/
#define   EMERGBTNNUMINDEXSIZE      200				/*�����رհ�ť��ű���������ռ��С*/
#define   BALISENUMINDEXSIZE        3000			/*Ӧ������ű���������ռ��С*/
#define   PSDNUMINDEXSIZE           300				/*�����ű�ű���������ռ��С*/
#define   VOBCNUMINDEXSIZE          200				/*VOBC��ű���������ռ��С*/
#define   FPDNUMINDEXSIZE           100				/*�����ű�ű���������ռ��С*/
#define   DSUNUMINDEXSIZE           50				/*DSU��ű���������ռ��С*/
#define   LOGICSECNUMINDEXSIZE		2400			/*�߼����α�ű���������ռ��С*/
/*add 20180726 xb V13.0���ݽṹ*/
#define   IDMAP_AOM_INDEX_SIZE      200             /*AOM���������С*/
#define   IDMAP_SPKS_INDEX_SIZE     200             /*SPKS���������С*/
#define   IDMAP_GATE_INDEX_SIZE     200             /*���������������С*/

/*��ž�̬���ݸ������ݽṹ��ͷָ��14*/	
#define WZCNUMLEN               	60				/*���ݿ���zc��ű�Ľṹ�������*/
#define WCINUMLEN               	60				/*���ݿ���ci��ű�Ľṹ�������*/
#define WATSNUMLEN               	60				/*���ݿ���ats��ű�Ľṹ�������*/
#define WSIGNALNUMLEN               900				/*���ݿ����źŻ���ű�Ľṹ�������*/
#define WTRACKSECNUMLEN             1500			/*���ݿ��й�����α�Ľṹ�������*/
#define WSWITCHNUMLEN               500				/*���ݿ��е����ű�Ľṹ�������*/
#define WSTATIONNUMLEN              300				/*���ݿ���վ̨��ű�Ľṹ�������---����ɶ�5���̻���������150 songxy 20190606*/
#define WEMERGBTNNUMLEN             200				/*���ݿ��н����رհ�ť��ű�Ľṹ�������*/
#define WBALISENUMLEN               2000			/*���ݿ���Ӧ������ű�Ľṹ�������*/
#define WPSDOORNUMLEN               200				/*���ݿ��������ű�ű�Ľṹ�������*/
#define WVOBCNUMLEN               	200				/*���ݿ���vobc��ű�Ľṹ�������*/
#define WFPDOORNUMLEN               100				/*���ݿ��з����ű�ű�Ľṹ�������*/
#define WDSUNUMLEN					50				/*���ݿ���DSU��ű�Ľṹ�������*/
#define WLOGICLEN					2000			/*���ݿ����߼����α�ű�Ľṹ�������*/
/*add 20180726 xb V13.0���ݽṹ*/
#define IDMAP_AOM_ID_SIZE      		200             /*AOM��������С*/
#define IDMAP_SPKS_ID_SIZE     		200             /*SPKS��������С*/
#define IDMAP_GATE_ID_SIZE     		200             /*�����ű�������С*/


/*add 20180320 xb V12.0���ݽṹ*/
#define VOBCCOMZONELEN				400				/*�����������ӹܵĵ����豸��Ϣ����*/
#define LINERELATELEN			    20				/*��·���ӹ�ϵ����*/

/*************************************************************************************************************
* ��������:    ��̬���ݳ�����Ϣ�ṹ���ʼ��
* �������:    DSU_EMAP_STRU *pDsuEmapStu ���ӵ�ͼ�ܽṹ��
* �������:    ��.
* ȫ�ֱ���:    ��.
* ����ֵ  :    0:��ʼ��ʧ��;1:��ʼ���ɹ�
*************************************************************************************************************/
unsigned char vDquEmapStructInit(DSU_EMAP_STRU *pDsuEmapStu);

	/*************************************************************************************************************
	* ��������:    ��̬���ݳ�����Ϣ�ṹ���ʼ��
	* �������:    DSU_IDMAP_STRU *pIdEmapStru ��Ŷ��ձ��ܽṹ��
	* �������:    ��.
	* ȫ�ֱ���:    ��.
	* ����ֵ  :    0:ʧ��;1�ɹ�
	*************************************************************************************************************/
	unsigned char vDquIdmapStructInit(DSU_IDMAP_STRU *pIdEmapStru);


#endif
