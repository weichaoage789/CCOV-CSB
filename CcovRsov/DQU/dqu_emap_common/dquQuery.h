/************************************************************************
*
* �ļ���   ��  dsuQuery.h
* ��Ȩ˵�� ��  �������ؿƼ��Ƽ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.11.21
* ����     ��  ������
* �������� ��  dsu��ѯ������������ͷ�ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _DSU_QUERY_H
#define _DSU_QUERY_H

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

/*ATO��������*/

/*���õ�ǰ����Դ*/
UINT8 dquSetCurrentQueryDataSource(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx);


/*2.�������Ծ����ѯ*/
/*
*���ʽӿ�
* ���������� ��A��B����λ�õõ�����λ�ơ�
* ��������� const LOD_STRU * pLODAStru, A��λ��
*            const LOD_STRU * pLODBStru, B��λ��
*            UINT8 PointNum,             ��������
*            const POINT_STRU * pPointStru, ������Ϣ�ṹ��
* ��������� INT32 *Displacement��       ABλ�ƣ����з����������ű�ʾ����            
* ����ֵ��   1,�ɹ�
*            0,ʧ��
* ʹ��ע�⣺�Ͽ���෿��Ŀ:����B�ķ�����ң�ABλ����LinkB����Ϊ���գ�
            ������ͨ��Ŀ�������߼�����仯�㣬�㷨��Ϊ����A�ķ�����ң�ABλ����LinkA����Ϊ���ա�
* author:qxt 20170811
*/
UINT8 dsuDispCalcuByLOC(const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement);

/*���ܺ��������ݵ���״̬��ȡ��ǰLink������Link*/
UINT8 dsuGetAdjacentLinkID(UINT16 LinkId, UCHAR Direction, UCHAR PointNum,const DQU_POINT_STATUS_STRU *pPointStru, UINT16 *pLinkId);

/*���ܺ�������λ��+λ��=��λ��*/
/*
* ���������� ���ܺ��������ݵ�ǰλ�ã���λ���Լ�����ĵ�����Ϣ���õ���λ��.
���һ���㼴��ǰLnk��㣬������һ��Link�յ㣬�����뵱ǰ��pLODStru�����Link Id����
* ��������� const LOD_STRU * pLODStru, ��ǰλ��
*            INT32 Displacement,        λ�ƣ��з��ţ��������Ϊ����ʾ����ȡ��
*            UCHAR PointNum,            ��������
*            const POINT_STRU * pPointStru,  ������Ϣ
* ��������� LOD_STRU *pNewLODStru       ��λ��
* ����ֵ��   1,�ɹ�
*            0,ʧ��
*/
UCHAR dsuLODCalcu(const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru);


/*
* �������ܣ�	�˺����Ĺ����ǲ�ѯ����λ�ü������link���С�
* ****************************************************************************************************
*        1          2              3                4                    5              6
*       -----��---------*--��--------------��--------------------��-------*-----��------------
*                       S                                                 E
*        ͼ����  ����Link�˵㡣*����ѯ�����λ�á�1��2��3...Linkʾ����
*        ��ͼ��ʾ��Link3��4Ϊ��������
* ****************************************************************************************************
* ��ڲ�����	const LOD_STRU * pLODStartStru,	ʼ��λ��
*               const LOD_STRU * pLODEndStru	�ն�λ��
*               const UINT8 PointNum			������Ŀ 
*               const POINT_STRU * pPointStru	������Ϣ
* ���ڲ�����	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link��������
*               UINT16 *wLinkNumber                             Link����
* ����ֵ��   	0,  ��ѯʧ��
*               1,  ��ѯ�ɹ�                                                             
* ʹ��ע�⣺    Ŀǰ������û�а������˵�Link Id��
*               ����Ҫ��������֮������γ����򣬷��򷵻�0
*              
               �Ͽ���෿��Ŀ�� S����pLODEndStru->Dir������ң��ҵ�E�򷵻سɹ����Ҳ����򷵻�ʧ�ܡ�
*              ������ͨ��Ŀ�� S����pLODStartStru->Dir������ң��ҵ�E�򷵻سɹ����Ҳ����򷵻�ʧ�ܡ�
*  author: qxt 20170814
*/
UINT8 dsuGetLinkBtwLoc(const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
					   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
					   UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber);


/*
* �������ܣ� �˺����Ĺ����ǽ��жϵ�ǰλ���Ƿ���ָ������Χ�ڡ�
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*  �Ͽ���෿��Ŀ�� �����㷨Ϊ������λ��DisplacementSC��DisplacementCE���������λ�Ʒ�����ͬ��˵��C��SE������
*  ������ͨ��Ŀ�������߼�����仯�㣩��   �����㷨Ϊ������λ��DisplacementCS��DisplacementCE���������λ�Ʒ��Ų���ͬ��˵��C��SE������
* ****************************************************************************************************
* ��ڲ����� const LOD_STRU * pLODStartStru,	����ʼ��λ��
*            const LOD_STRU * pLODEndStru,	�����ն�λ��
*            const UINT8 PointNum,			������Ŀ 
*            const POINT_STRU * pPointStru,	������Ϣ
*			 const LOD_STRU * pCurrentLODStru,��ǰλ��
* ���ڲ����� UINT8 *pLocInArea         1,��ָ������
*                                     0,����ָ������
* ����ֵ��   0����ѯʧ��
*            1����ѯ�ɹ�
* ʹ��ע�⣺1.�ڵ�ǰλ��������˵�λ��ʱ����ʱ�����¹����жϣ�
*             1) ��ǰλ����������㣬��Ϊ�õ����ڱ�����
*             2) ��ǰλ���������յ㣬��Ϊ�õ㲻���ڱ�����
*             3) 3���غϷ����������ڡ�
*           2.��������ҵ�������յ㶼�������ڣ����Ե������α�����������ʼ�����ն�λ�ã�ֻҪ���ε�����һ���������ڣ�˵���ڵ���ʼ�����ն��γɵı�������
*           3.���SE���򲻴��ڣ���������0ʧ�ܡ����SC,����SE�����ڣ���������1�ɹ������ǵ�C����������
*           4.�෿�ͺϿ���Ŀ��ʼ��λ�ú��ն�λ�ô���ķ����豣֤һ��
*             ������ͨ����֤ʼ��λ�õķ�����ȷ���ɡ�
*/
UINT8 dsuCheckLocInArea(const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						const LOD_STRU *pCurrentLODStru,UINT8 *pLocInArea); 

/*���ܺ�������ȡlink����*/
UINT32 LinkLength(UINT16 wLinkID);

/*
* �������ܣ� �˺����Ĺ������жϵ�ǰλ���Ƿ���ָ������Χ�ڣ��˺����Ƕ�dsuCheckLocInAreaQuick
             ��һ�����䡣���ڽ���ǰ����ȡ�����ټ����Ƿ��������ڡ�
* ��ڲ����� const LOD_STRU * pLODStartStru,	����ʼ��λ��
*            const LOD_STRU * pLODEndStru,	�����ն�λ��
* ���ڲ����� UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link��������
*            UINT16 *wLinkNumber                             Link����
*			 const LOD_STRU * pCurrentLODStru,��ǰλ��
*            UINT8 *pLocInArea         1,��ָ������
*                                     0,����ָ������
* ����ֵ��   0����ѯʧ��
*            1����ѯ�ɹ�
*/
UINT8 dsuCheckLocInAreaQuick2(const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
							 UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
							 const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea);
/*
* �������ܣ� �˺����Ĺ����ǽ��жϵ�ǰλ���Ƿ���ָ������Χ�ڡ�
             �˺����Ĺ�����CheckLocInArea��������ġ�CheckLocInArea�Է���û��Ҫ�󣬼��������漰�ķ�������ȷ�ġ�
			 ��CheckLocInAreaQuickҪ���StartLOD�ܹ�����EndLOD��
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       �����㷨Ϊ�������жϵ�ǰλ���������S��E�Ƿ���ͬһLink�ϣ�
*       �����ͬһLink�ϣ����ݷ����λ���ж��Ƿ��������ڣ��������ͬһLink���ж�CLinkId�Ƿ���SE֮���ĳһLinkId��ͬ
*       ��������֮���Link���Ա������������Զ��ڶ�ε��ñ�������ѯ�Ƿ��������ڵĺ���Ч�ʺ��а���
*
* ****************************************************************************************************
* ��ڲ����� const LOD_STRU * pLODStartStru,	����ʼ��λ��
*            const LOD_STRU * pLODEndStru,	�����ն�λ��
* ���ڲ�����	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link��������
*               UINT16 *wLinkNumber                             Link����
*			 const LOD_STRU * pCurrentLODStru,��ǰλ��
* ���ڲ����� UINT8 *pLocInArea         1,��ָ������
*                                     0,����ָ������
* ����ֵ��   0����ѯʧ��
*            1����ѯ�ɹ�
* ʹ��ע�⣺1.�ڵ�ǰλ��������˵�λ��ʱ����ʱ�����¹����жϣ�
*             1) ��ǰλ����������㣬��Ϊ�õ����ڱ�����
*             2) ��ǰλ���������յ㣬��Ϊ�õ㲻���ڱ�����
*             3) 3���غϷ����������ڡ�
*           2.��������ҵ�������յ㶼�������ڣ����Ե������α�����������ʼ�����ն�λ�ã�ֻҪ���ε�����һ���������ڣ�˵���ڵ���ʼ�����ն��γɵı�������
*           3.���SE���򲻴��ڣ���������0ʧ�ܡ����SC,����SE�����ڣ���������1�ɹ������ǵ�C����������
*/
UINT8 dsuCheckLocInAreaQuick(const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
							 UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
							 const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea);
/*���ܺ���,��ѯ��ǰGarde������Grade*/
UINT16 FindAdjacentGradeID(const UINT16 wCurrentGradeID, const UCHAR chDirection, const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pstPointStrut);

/*����������¶�*/
UINT8 dsuGetGradeIdbyLoc(const UINT16 linkId,const UINT32 offset,UINT16* GradeID);
/*����㵽�����¶����ľ���*/
UINT8 dsuGetDistanceFromOrigin(UINT16 gradeId,UINT16 linkId,UINT32 Ofst,UINT32* length);
/*�����¶ȱ��µ����¶����ľ���*/
UINT8 dsuDividPointDistanceFromOrigin(UINT16 GradeId,UINT16 linkId,UINT32* endPointDistance,UINT32* startPointDistance);
/*����������¶�ֵ*/
UINT8 dsuCalculateGradeValue(UINT16 LinkId,UINT32 ofst, FLOAT32* GradeVal);
/*��������¶�*/
UCHAR dsuGradientGet(const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient);
/*�жϺ�һ�¶ȵ��¶�ֵ���ǰһ���¶ȵ��¶�ֵ�����ӣ����仯���Ǽ��� */
UINT8 dquGradeTrend(UINT16 fistGradeID,UINT16 secondGradeID,UINT8 *GradeTrend);

/*
�������ܣ���ȡǰ����¶�
���������UINT16 linkId,��ǰlink
���������gradeFrnt �¶� 
			Null��  ��·�յ��ǰ���¶�Ϊͳһ�¶�  
			��Null��ǰ���¶����¶ȱ��пɲ�
  ����ֵ��0ʧ�� 1�ɹ�
*/
UINT8 dsuGetGradeFrnt(UINT16 linkId, DSU_GRADE_STRU** gradeFrnt) ;

/*
�������ܣ���ȡ������¶�
���������UINT16 linkId,��ǰlink
���������gradeRear �¶� 
			Null��  ��·�յ������¶�Ϊͳһ�¶�  
			��Null�������¶����¶ȱ��пɲ�
  ����ֵ��0ʧ�� 1�ɹ�
*/
UINT8 dsuGetGradeRear(UINT16 linkId, DSU_GRADE_STRU** gradeRear) ;

/**
�������ܣ���ȡ��ǰLINK�ľ�̬���ٸ���
���������
@linkId����ǰlink
�����������
����ֵ����̬���ٸ���
*/
UINT8 GetStcLmtSpdCntByLnkId(const UINT16 linkId);

/**
�������ܣ���ȡ�ƶ�LINK�ĵ�ָ���±�ľ�̬��ʱ���ٵĶ���ָ��
���������
@linkId����ǰLink���
@lmtSpdIdx����ӦLink�ĵڼ������٣�ȡֵ��Χ1-10
����ֵ��
@NULL����ȡʧ�� 
@�ǿգ���Ӧ���ٽṹ��ָ��
*/
DSU_STATIC_RES_SPEED_STRU *GetStcLmtSpdInfoByLnkId(const UINT16 linkId,const UINT8 idx);

/*
�������ܣ����࿪����תΪ�������
���������multiPointId �࿪�������
          multiPointState �࿪����״̬

���������wVirtualPointIds ��ֺ����������� 
          wVirtualPointStates ��ֺ��������״̬
����ֵ������ֵ����ֺ������������, 0ʧ��  

�����㷨���࿪������������������� Nλ��R1λ��L1λ��R2λ��L2λ
            ����Ӧ�������е������� 0��  1��   2��   3��   4
		    ����Ӧ�Ķ࿪����״̬�� 1��  2��   3��   4��   5
		   ��˵� ����=�࿪����״̬multiPointState-1 ʱ������Ӧ�������������ԭֵ�������������״̬ȡ������λ<=>��λ��

ע�������������������wVirtualPointIds��״̬wVirtualPointStates��������Ч���������״̬����Ϊ3����
         
		               / L                                      / L
		              /                                        /
		             /                                        /       101                
		-|------------------------         ===��   ---|------/----------------- N      
		   1         \           N                          102       \
		              \                                                \
					   \                                                \
		              R                                                 R
		           ͼ1.ʵ����������                            ͼ2.���ⵥ������

		   ��ͼ1ʵ����������1��״̬Ϊ3ʱ���Ƴ�ͼ2���ⵥ������ΪLλ����ʱ101Ϊ��Ч�����������״̬Ϊ3������ֵ��������Ϊ2������
		   wVirtualPointIds[0]=102;
		   wVirtualPointStates[0]=2;
		   wVirtualPointIds[1]=101;
		   wVirtualPointStates[1]=3;
author:������Ŀ add by qxt 20170622
*/
UINT8 ConvertToVirtualSwitches(const UINT16 multiPointId,const UINT8 multiPointState,UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM]);

/*
�������ܣ����������תΪ�࿪����
���������wVirtualPointIds[] ���������� 
          wVirtualPointStates[] �������״̬
		  wVirtualPointNum    �����������
���������multiPointId ��Ӧ�࿪�������
          multiPointState ��Ӧ�࿪����״̬
����ֵ��0ʧ�� 1�ɹ�

ע�������������������wVirtualPointIds��״̬wVirtualPointStates��������Ч���������������Ҫ���롣��
           
		 		      /L                                                 L
		             /                                                /
		            /                                                /
		           /                                                /
		---|------/-----------\------ N      =====>        ------------------ N
		          1          2 \                                    \
							    \                                    \
		                          R                                      R
		         ͼ1.���ⵥ������                             ͼ2.ʵ����������
		 
		 ��ͼ1����������1��״̬Ϊ2ʱ���Ƴ�ͼ2�࿪������״̬ΪLλ����ʱͼһ�������2Ϊ��Ч�ģ����������������Ϊ���£�
         wVirtualPointIds[0]=1;
		 wVirtualPointStates[0]=2;
         wVirtualPointNum=1;
author:������Ŀ add by qxt 20170622
*/
UINT8 ConvertToMultiSwitch(const UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointNum,UINT16 *multiPointId,UINT8 *multiPointState);


#ifdef __cplusplus
}
#endif

#endif