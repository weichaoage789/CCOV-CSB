/************************************************************************
* �ļ���    ��  dsuQuery.c
* ��Ȩ˵��  ��  �������ؿƼ����޹�˾
* �汾��  	��  1.0
* ����ʱ��	��	2009.09.26
* ����  		�����	
* ��������	��	dsu��ѯ�����ļ�  
* ʹ��ע��	�� 
* �޸ļ�¼	��	
************************************************************************/
#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "dquQueryExp.h"
#include "dsuVar.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "dquCbtcType.h"

/*
* ���������� �������з�����Ϣ�Լ�����״̬��Ϣ����ѯ�õ���ǰlink�����з��������link��
* ��������� UINT16 LinkId, ��ǰLink���
*            UCHAR Direction, ��ǰ����
*            UCHAR PointNum,  ������Ϣ���������
*            const POINT_STRU *pPointStru, ������Ϣ
* ��������� UINT16 *pLinkId����ǰLink������Link���
* ����ֵ��   1,��ѯ�ɹ�
*            0,��ѯʧ��,����������Ч������ʧ��
*		      	 0xFF,��·�յ㣬δ�ҵ�����
*             
*/
UINT8 dsuGetAdjacentLinkIDExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId, UCHAR Direction, UCHAR PointNum,const DQU_POINT_STATUS_STRU *pPointStru, UINT16 *pLinkId)
{
	UINT8 chReturnValue=0;             /*���ں�������ֵ*/
	UINT16 CurrentLinkIndex;           /*��ǰLink Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;   /*��ǰLink,��ȡ��������ʹ��*/
	UINT16 AdjancentLinkIndex;         /*��ǰLink Index*/
	DSU_LINK_STRU *pAdjancentLinkStru=NULL; /*��ǰLink,��ȡ��������ʹ��*/
	UINT8 FindFlag = 0;                /*���ұ�ǣ����ڼ�¼�����Ƿ�ɹ�*/
	UINT8 i;                           /*����ѭ��*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�ж�����LinkID����Ч��*/	
	if((LinkId>LINKINDEXNUM)||(dsuLinkIndex[LinkId] == 0xFFFF))
	{
		/*����LinkId��Ч����ѯʧ�ܣ�����*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�ж����뷽����Ч��*/
	if((Direction!=EMAP_SAME_DIR)&&(Direction!=EMAP_CONVER_DIR))
	{
		/*���뷽����Ч����ѯʧ�ܣ�����*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*���˱�ʾ����������Ч*/

	/*����LinkID��ȡ��ǰlink�ṹ��*/
	CurrentLinkIndex=dsuLinkIndex[LinkId];
	pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

	/*�������뷽�������������*/
	if(Direction==EMAP_CONVER_DIR)
	{
		/*Link������ң����յ�ǰLinkʼ�˶˵�������������*/
		if(pCurrentLinkStru->wOrgnJointMainLkId==DSU_NULL_16)
		{
			/*��·�յ㣬��ǰlink������link������*/
			chReturnValue = 0xff;
			return chReturnValue;

		}else if(pCurrentLinkStru->wOrgnPointType==EMAP_AXLE_DOT)
		{
			/*�����*/
			/*��������Link��Ϊ����*/
			*pLinkId = pCurrentLinkStru->wOrgnJointMainLkId;

			/*��ѯ�ɹ������ز�ѯ���*/
			chReturnValue=1;
			return chReturnValue;

		}else if(pCurrentLinkStru->wOrgnPointType==EMAP_POINT_DOT)
		{
			/*��ǰʼ�˶˵������ǵ����*/
			/*�жϵ�ǰlink�Ƿ�Ϊ����Ļ��Link*/
			if(pCurrentLinkStru->wOrgnJointSideLkId==DSU_NULL_16)
			{
				/*��ǰlink�ǻ��Link���ǻ��link����Linkֻ��һ��������״̬Ҳ������ȷ��������֤����״̬�Ƿ���ȷ*/

				/*����������Ϣ�����ҵ�ǰ�����״̬��Ϣ*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wOrgnPointId)
					{
						/*˵���������������Ϣ�в��ҵ���ǰLink�ն˴��ĵ���*/

						/*�ò���λΪTRUR*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*û���ҵ���ǰ������Ϣ����ǰLink����Link���ܴ���MA��Χ֮�⣬����ʧ��*/
					chReturnValue=0xff;
					return chReturnValue;
				}

				/*��ȡ��������Link��Ϣ,Ҳ���ǻ�ȡ���ڻ��Link��Ϣ*/
				AdjancentLinkIndex=dsuLinkIndex[pCurrentLinkStru->wOrgnJointMainLkId];
				pAdjancentLinkStru=dsuStaticHeadStru->pLinkStru+AdjancentLinkIndex;

				if(((pPointStru[i].PointStatus == EMAP_POINT_STATUS_MAIN)&&(LinkId==pAdjancentLinkStru->wTmnlJointMainLkId))
					||((pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)&&(LinkId==pAdjancentLinkStru->wTmnlJointSideLkId)))
				{
					/*����״̬��ȷ�����������1.���ڵ���Ϊ��λ����ǰLink������Link����������Iink��
					2.���ڵ���Ϊ��λ����ǰLink������Link�Ĳ�������Iink��
					*/
					*pLinkId=pCurrentLinkStru->wOrgnJointMainLkId;
				
					/*��ѯ�ɹ������ز�ѯ���*/
					chReturnValue=1;
					return chReturnValue;
				}
				else
				{
					/*���ҵ����ڵ����������ڵ���ĵ���״̬���Ի��ߵ���״̬ʧ���������ش��󣬺�������ʧ��*/
					chReturnValue=0;
					return chReturnValue;
				}
				
			}
			else
			{
				/*��ǰlinkΪ����Ļ��Link*/
				/*����������Ϣ�����ҵ�ǰ�����״̬��Ϣ*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wOrgnPointId)
					{
						/*˵���������������Ϣ�в��ҵ���ǰLink�ն˴��ĵ���*/

						/*�ò���λΪTRUR*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*û���ҵ���ǰ������Ϣ����ǰLink����Link���ܴ���MA��Χ֮�⣬����ʧ��*/
					chReturnValue=0xff;
					return chReturnValue;
				}
				else
				{
					/*���ݵ����ʵʱ״̬��Ϣ��������*/
					if (pPointStru[i].PointStatus == EMAP_POINT_STATUS_MAIN)
					{
						/*����״̬λ�ڶ�λ*/
						/*�����������Link��Ϊ����*/
						*pLinkId=pCurrentLinkStru->wOrgnJointMainLkId;

						/*��ѯ�ɹ������ز�ѯ���*/
						chReturnValue=1;
						return chReturnValue;
					}
					else if(pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)
					{
						/*����״̬λ�ڷ�λ*/
						/*������ڲ���Link��Ϊ����*/
						*pLinkId=pCurrentLinkStru->wOrgnJointSideLkId;
						
						/*��ѯ�ɹ������ز�ѯ���*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*����ʧ�� POINTLOSESTATUS ʧ����ֵ�Ĵ���*/
						chReturnValue=0;
						return chReturnValue;
					}
				}

			}

		}
		else
		{
			/*Link�˵����Ͳ��Ϸ�*/
			chReturnValue=0;
			return chReturnValue;
		}

	}
	else /*if(Direction==SAMEDIR)*/
	{
		/*������Link�������*/

		/*Link������ң����յ�ǰLink�ն˶˵�������������*/
		if(pCurrentLinkStru->wTmnlJointMainLkId==DSU_NULL_16)
		{
			/*�˵�����Ϊ��·�յ㣬���ز�ѯ���*/
			chReturnValue = 0xff;
			return chReturnValue;

		}else if(pCurrentLinkStru->wTmnlPointType==EMAP_AXLE_DOT)
		{
			/*��ǰLink�ն˶˵�����*/
			/*��������Link��Ϊ����*/

			*pLinkId=pCurrentLinkStru->wTmnlJointMainLkId;
			chReturnValue=1;
			return chReturnValue;

		}else if (pCurrentLinkStru->wTmnlPointType == EMAP_POINT_DOT)
		{
			/*��ǰLink�ն˶˵�����*/

			/*�жϵ�ǰlink�Ƿ�Ϊ����Ļ��Link*/
			if(pCurrentLinkStru->wTmnlJointSideLkId==DSU_NULL_16)
			{
				/*��ǰlink�ǻ��Link���ǻ��link����Linkֻ��һ��������״̬Ҳ������ȷ��������֤����״̬�Ƿ���ȷ*/

				/*����������Ϣ�����ҵ�ǰ�����״̬��Ϣ*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wTmnlPointId)
					{
						/*˵���������������Ϣ�в��ҵ���ǰLink�ն˴��ĵ���*/

						/*�ò���λΪTRUR*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*û���ҵ���ǰ������Ϣ����ǰLink����Link���ܴ���MA��Χ֮�⣬����ʧ��*/
					chReturnValue=0xff;
					return chReturnValue;
				}

				/*��ȡ��������Link��Ϣ,Ҳ���ǻ�ȡ���ڻ��Link��Ϣ*/
				AdjancentLinkIndex=dsuLinkIndex[pCurrentLinkStru->wTmnlJointMainLkId];
				pAdjancentLinkStru=dsuStaticHeadStru->pLinkStru+AdjancentLinkIndex;

				if(((pPointStru[i].PointStatus == EMAP_POINT_STATUS_MAIN)&&(LinkId==pAdjancentLinkStru->wOrgnJointMainLkId))
					||((pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)&&(LinkId==pAdjancentLinkStru->wOrgnJointSideLkId)))
				{
					/*����״̬��ȷ�����������1.���ڵ���Ϊ��λ����ǰLink������Link����������Iink��
					2.���ڵ���Ϊ��λ����ǰLink������Link�Ĳ�������Iink��
					*/
					*pLinkId=pCurrentLinkStru->wTmnlJointMainLkId;

					/*��ѯ�ɹ������ز�ѯ���*/
					chReturnValue=1;
					return chReturnValue;
				}
				else
				{
					/*���ҵ����ڵ����������ڵ���ĵ���״̬���Ի��ߵ���״̬ʧ���������ش��󣬺�������ʧ��*/
					chReturnValue=0;
					return chReturnValue;
				}
			}
			else
			{
				/*��ǰ���Link�����յ���ʵʱ״̬���ദ��*/

				/*���ҵ�ǰ������Ϣ*/
				FindFlag=0;
				for(i=0;i<PointNum;i++)
				{
					if (pPointStru[i].PointId == pCurrentLinkStru->wTmnlPointId)
					{
						/*˵���������������Ϣ�в��ҵ���ǰLink�ն˴��ĵ���*/
						FindFlag=1;
						break;
					}
				}

				if(FindFlag==0)
				{
					/*û���ҵ���ǰ������Ϣ����ǰLink����Link���ܴ���MA��Χ֮�⣬����ʧ��*/
					chReturnValue=0xff;
					return chReturnValue;
				}
				else
				{
					/*���ݵ���ʵʱ״̬�����������*/
					if(pPointStru[i].PointStatus==EMAP_POINT_STATUS_MAIN)
					{
						/*��ǰLink�ǻ��Link������λ���յ���������Link��Ϊ����*/

						*pLinkId=pCurrentLinkStru->wTmnlJointMainLkId;

						/*��ѯ�ɹ������ز�ѯ���*/
						chReturnValue=1;
						return chReturnValue;
					}
					else if (pPointStru[i].PointStatus == EMAP_POINT_STATUS_SIDE)
					{
						/*��ǰLink�ǻ��Link������λ���յ����ڲ���Link��Ϊ����*/

						*pLinkId=pCurrentLinkStru->wTmnlJointSideLkId;
						
						/*��ѯ�ɹ������ز�ѯ���*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*��ǰLink�ǻ��Link��ʧ��  POINTLOSESTATUS������ʧ��*/
						chReturnValue=0;
						return chReturnValue;
					}
				}

			}

		}
		else
		{
			/*Link�˵����Ͳ��Ϸ�����������ʧ��*/
			chReturnValue=0;
			return chReturnValue;
		}
	}

	/*�˴����ɴ�*/
	
}


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
*ע�⣺�Ͽ���෿ʹ��
*/
static UCHAR dsuLODCalcuExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru)
{
	UINT8 chReturnValue=0;       /*���ں�������ֵ*/
	UINT16 CurrentLinkId = 0;         /*���ڲ��ҹ�������ʱ���浱ǰLinkID*/
	UINT16 CurrentLinkIndex = 0; /*��ǰLink Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*����ѭ�����ҹ�������ʱ���浱ǰLink�ṹ��*/
	UINT8 FindDir = 0;                  /*���ڼ�¼ʵ�ʲ��������еĲ��ҷ���*/
	INT32 FindDisplacement = 0;         /*��ʱ����ƫ����*/
	INT32 FindDisplacementSum = 0;       /*�����ۼ�λ��ƫ����*/
	UINT16 k;                        /*ѭ���ñ���*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�ж�����LinkID����Ч��*/
	if((pLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStru->Lnk] == 0xFFFF))
	{
		/*����LinkId��Ч����ѯʧ�ܣ�����*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�ж��������ƫ��������Ч��*/
	if (pLODStru->Off > LinkLengthExp(pDsuEmapStru,pLODStru->Lnk))
	{
		/*������Ч����ѯʧ�ܣ�����*/
		chReturnValue=0;
		return chReturnValue;
	}
	/*�ж����뷽����Ч��*/
	if((pLODStru->Dir!=EMAP_SAME_DIR)&&(pLODStru->Dir!=EMAP_CONVER_DIR))
	{
		/*���뷽����Ч����ѯʧ�ܣ�����*/
		chReturnValue=0;
		return chReturnValue;
	}	

	/*�����������ƫ����DisplacementΪ0*/
	if (Displacement == 0)
	{
		/*displacement���Ϊ0�����ص�ǰλ��Ϊ����*/
		pNewLODStru->Lnk=pLODStru->Lnk;
		pNewLODStru->Off=pLODStru->Off;
		pNewLODStru->Dir=pLODStru->Dir;

		/*��ѯ�ɹ������ز�ѯ���*/
		chReturnValue=1;
		return chReturnValue;
	}

	/*���displacementС��0������displacement����ֵ��������λ�ã�����ȡ��*/
	if((pLODStru->Dir==EMAP_SAME_DIR)&&(Displacement>0))
	{
		/*�������*/
		FindDir=EMAP_SAME_DIR;
		FindDisplacement=Displacement;
	}
	else if((pLODStru->Dir==EMAP_CONVER_DIR)&&(Displacement<0))
	{
		/*�������*/
		FindDir=EMAP_SAME_DIR;
		FindDisplacement=-Displacement;
	}
	else if((pLODStru->Dir==EMAP_SAME_DIR)&&(Displacement<0))
	{
		/*�������*/
		FindDir=EMAP_CONVER_DIR;
		FindDisplacement=-Displacement;
	}
	else if((pLODStru->Dir==EMAP_CONVER_DIR)&&(Displacement>0))
	{
		/*�������*/
		FindDir=EMAP_CONVER_DIR;
		FindDisplacement=Displacement;
	}
	else
	{
		/*���ɴ��֧*/
	}

	/*����LinkID�����ݿ��в��ҵ�ǰlink��Ϣ*/
	CurrentLinkIndex=dsuLinkIndex[pLODStru->Lnk];
	pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;
	if(FindDir==EMAP_SAME_DIR)
	{
		/*�������*/

		/*���ݵ�ǰλ�������ۼ�ƫ����*/
		FindDisplacementSum=pCurrentLinkStru->dwLength-pLODStru->Off;

		/*����λ���ж���λ���Ƿ���ͬһ��Link��*/
		if(FindDisplacementSum>=FindDisplacement)
		{
			/*˵����ͬһ��link�ϣ��ۼ�λ�Ƶõ���λ��*/
			pNewLODStru->Lnk=pCurrentLinkStru->wId;
			pNewLODStru->Off=pLODStru->Off+FindDisplacement;
			pNewLODStru->Dir=pLODStru->Dir;
			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			for (k=0;k<LINKINDEXNUM;k++)
			{
				/*�˴�ѭ��������Ϊ��Ҫ����ÿһ��Link�����Ƕ�ѭ�����������ܳ���LINKINDEXNUM*/
				if( 1==dsuGetAdjacentLinkIDExp(pDsuEmapStru,pCurrentLinkStru->wId, FindDir, PointNum, pPointStru,&CurrentLinkId) )
				{
					/*pCurrentLinkStruָ������Link,����Link��Ϊ��ǰLink*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					FindDisplacementSum+=pCurrentLinkStru->dwLength;
					if(FindDisplacementSum>=FindDisplacement)
					{
						/*�ۼӵ�λ��ֵ���ϵ�ǰLink���ȴ�������ƫ������˵��Ŀ��λ�õ�ǰlink��*/
						pNewLODStru->Lnk=pCurrentLinkStru->wId;
						pNewLODStru->Off=FindDisplacement-(FindDisplacementSum-pCurrentLinkStru->dwLength);
						pNewLODStru->Dir=pLODStru->Dir;
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*�ۼӵ�λ��ֵ���ϵ�ǰLink����С������ƫ������˵��Ŀ��λ�ò��ڵ�ǰlink�ϣ�ѭ��������ǰ������һ��Link*/
					}
				}
				else
				{
					/*��ȡ��һ��linkʧ��*/
					chReturnValue=0;
					return chReturnValue;
				}
			}
				
			/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
			chReturnValue=0;
			return chReturnValue;
		}
	}
	else
	{
		/*�������*/

		/*�ۼ�ƫ������0*/
		FindDisplacementSum=0;

		/*���ݵ�ǰλ�÷����ۼ�ƫ����*/
		FindDisplacementSum=pLODStru->Off;

		if(FindDisplacementSum>=FindDisplacement)
		{
			/*˵����ͬһ��link��*/
			pNewLODStru->Lnk=pCurrentLinkStru->wId;
			pNewLODStru->Off=pLODStru->Off-FindDisplacement;
			pNewLODStru->Dir=pLODStru->Dir;

			/*��ѯ�ɹ������ز�ѯ���*/
			chReturnValue=1;
			return chReturnValue;
		}
		else
		{
			for (k=0;k<LINKINDEXNUM;k++)
			{
				/*�˴�ѭ��������Ϊ��Ҫ����ÿһ��Link�����Ƕ�ѭ�����������ܳ���LINKINDEXNUM*/
				if( 1==dsuGetAdjacentLinkIDExp(pDsuEmapStru,pCurrentLinkStru->wId, FindDir, PointNum, pPointStru,&CurrentLinkId) )
				{
					/*pCurrentLinkStruָ������Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					FindDisplacementSum+=pCurrentLinkStru->dwLength;
					if(FindDisplacementSum>=FindDisplacement)
					{
						/*�ۼӵ�λ��ֵ���ϵ�ǰLink���ȴ�������ƫ������˵��Ŀ��λ�õ�ǰlink��*/
						pNewLODStru->Lnk=pCurrentLinkStru->wId;
						pNewLODStru->Off=(FindDisplacementSum-FindDisplacement);
						pNewLODStru->Dir=pLODStru->Dir;
                        /*��ѯ�ɹ��������ѯ���*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*�ۼӵ�λ��ֵ���ϵ�ǰLink����С������ƫ������˵��Ŀ��λ�ò��ڵ�ǰlink�ϣ�ѭ��������ǰ������һ��Link*/
					}
				}
				else
				{
					/*��ȡ��һ��linkʧ��*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
			chReturnValue=0;
			return chReturnValue;
		}
	}
}

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
*author:qxt 20170811 ������ͨ����link�߼�����仯��
*/
static UCHAR dsuLODCalcuExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru)
{
	UINT8 chReturnValue=0;       /*���ں�������ֵ*/
	UINT8 chFlag=1;              /*����ִ��ʧ�ܱ��*/
	UINT16 wCurrentLinkId = 0;         /*���ڲ��ҹ�������ʱ���浱ǰLinkID*/
	UINT16 wCurrentLinkIndex = 0; /*��ǰLink Index*/
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*����ѭ�����ҹ�������ʱ���浱ǰLink�ṹ��*/
	UINT8 wFindDir = 0;                  /*���ڼ�¼ʵ�ʲ��������еĲ��ҷ���*/
	INT32 dwFindDisplacement = 0;         /*��ʱ����ƫ����*/
	INT32 dwFindDisplacementSum = 0;       /*�����ۼ�λ��ƫ����*/
	UINT16 k=0;             /*ѭ���ñ���*/
	UINT8 tmpLODDir = 0xff;    /*��ʱ����,������ڷ�����Ϣ*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if ((NULL != pDsuEmapStru)&&(NULL != pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*�ж�����LinkID����Ч��*/
		if((pLODStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16 == dsuLinkIndex[pLODStru->Lnk]))
		{
			/*����LinkId��Ч����ѯʧ�ܣ�����*/
			chFlag=0;
		}

		/*�ж��������ƫ��������Ч��*/
		if (pLODStru->Off > LinkLengthExp(pDsuEmapStru,pLODStru->Lnk))
		{
			/*������Ч����ѯʧ�ܣ�����*/
			chFlag=0;
		}
		/*�ж����뷽����Ч��*/
		if((EMAP_SAME_DIR!=pLODStru->Dir)&&(EMAP_CONVER_DIR!=pLODStru->Dir))
		{
			/*���뷽����Ч����ѯʧ�ܣ�����*/
			chFlag=0;
		}	
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chFlag = 0;

	}
	
	if (0!=chFlag)
	{
		/*�����������ƫ����DisplacementΪ0*/
		if (0==Displacement)
		{
			/*displacement���Ϊ0�����ص�ǰλ��Ϊ����*/
			pNewLODStru->Lnk=pLODStru->Lnk;
			pNewLODStru->Off=pLODStru->Off;
			pNewLODStru->Dir=pLODStru->Dir;

			/*��ѯ�ɹ������ز�ѯ���*/
			chReturnValue=1;
		}
		else/*0!=Displacement*/
		{
			tmpLODDir = pLODStru->Dir;
			
			/*���displacementС��0������displacement����ֵ��������λ�ã�����ȡ��*/
			if((EMAP_SAME_DIR==pLODStru->Dir)&&(Displacement>0))
			{
				/*�������*/
				wFindDir=EMAP_SAME_DIR;
				dwFindDisplacement=Displacement;
			}
			else if((EMAP_CONVER_DIR==pLODStru->Dir)&&(Displacement<0))
			{
				/*�������*/
				wFindDir=EMAP_SAME_DIR;
				dwFindDisplacement=-Displacement;
			}
			else if((EMAP_SAME_DIR==pLODStru->Dir)&&(Displacement<0))
			{
				/*�������*/
				wFindDir=EMAP_CONVER_DIR;
				dwFindDisplacement=-Displacement;
			}
			else if((EMAP_CONVER_DIR==pLODStru->Dir)&&(Displacement>0))
			{
				/*�������*/
				wFindDir=EMAP_CONVER_DIR;
				dwFindDisplacement=Displacement;
			}
			else
			{
				/*���ɴ��֧*/
			}

			/*����LinkID�����ݿ��в��ҵ�ǰlink��Ϣ*/
			wCurrentLinkIndex=dsuLinkIndex[pLODStru->Lnk];
			pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

			/*��ѯ����λ���ڲ�ѯ������ڵ�Link�����*/
			if(EMAP_SAME_DIR==wFindDir)
			{
				/*�������*/

				/*���ݵ�ǰλ�������ۼ�ƫ����*/
				dwFindDisplacementSum=pCurrentLinkStru->dwLength-pLODStru->Off;

				/*����λ���ж���λ���Ƿ���ͬһ��Link��*/
				if(dwFindDisplacementSum>=dwFindDisplacement)
				{
					/*˵����ͬһ��link�ϣ��ۼ�λ�Ƶõ���λ��*/
					pNewLODStru->Lnk=pCurrentLinkStru->wId;
					pNewLODStru->Off=pLODStru->Off+dwFindDisplacement;
					pNewLODStru->Dir=pLODStru->Dir;
					chReturnValue=1;
				}
				else
				{
                    /*��������*/
				}
			}
			else/*FindDir=EMAP_CONVER_DIR*/
			{
				/*�������*/

				/*�ۼ�ƫ������0*/
				dwFindDisplacementSum=0;

				/*���ݵ�ǰλ�÷����ۼ�ƫ����*/
				dwFindDisplacementSum=pLODStru->Off;

				if(dwFindDisplacementSum>=dwFindDisplacement)
				{
					/*˵����ͬһ��link��*/
					pNewLODStru->Lnk=pCurrentLinkStru->wId;
					pNewLODStru->Off=pLODStru->Off-dwFindDisplacement;
					pNewLODStru->Dir=pLODStru->Dir;

					/*��ѯ�ɹ������ز�ѯ���*/
					chReturnValue=1;
				}
				else
				{
					/*��������*/
				}
			}

			/*��ѯ����λ�����ѯ��㲻��ͬһ��Link*/
			if (1!=chReturnValue)
			{
				/*�˴�ѭ��������Ϊ��Ҫ����ÿһ��Link�����Ƕ�ѭ�����������ܳ���LINKINDEXNUM*/
				for (k=0;k<LINKINDEXNUM;k++)
				{	
					if( 1==dsuGetAdjacentLinkIDExp(pDsuEmapStru,pCurrentLinkStru->wId, wFindDir, PointNum, pPointStru,&wCurrentLinkId) )
					{	

						/*�ж�Link�Ƿ�Ϊ�߼�����仯��(��ѯ����Ϊ���Ҹ�link���յ�Ϊ�߼�����仯�� �� ��ѯ�������Ҹ�link�����Ϊ�߼�����仯)*/
						if(((EMAP_SAME_DIR==wFindDir)&&(0x55==pCurrentLinkStru->wLogicDirChanged))||  
							((EMAP_CONVER_DIR==wFindDir)&&(0xaa==pCurrentLinkStru->wLogicDirChanged)))
						{	
							wFindDir =~ wFindDir;/*�б仯�㣬�ı���ҷ���*/
							tmpLODDir =~ tmpLODDir;/*�б仯��,�ı����λ���߼�����*/
						}

						/*pCurrentLinkStruָ������Link,����Link��Ϊ��ǰLink*/
						wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkId];
						pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

						dwFindDisplacementSum+=pCurrentLinkStru->dwLength;
						if(dwFindDisplacementSum>=dwFindDisplacement)/*�ۼӵ�λ��ֵ���ϵ�ǰLink���ȴ�������ƫ������˵��Ŀ��λ�õ�ǰlink��*/
						{

							/*���յ�����λ��Ϊ���ҷ�����*/
							if (EMAP_CONVER_DIR==wFindDir)
							{
								pNewLODStru->Lnk=pCurrentLinkStru->wId;
								pNewLODStru->Off=dwFindDisplacementSum-dwFindDisplacement;
								pNewLODStru->Dir=tmpLODDir;
							}
							/*���յ�����λ��Ϊ���ҷ������� */
							else
							{

								pNewLODStru->Lnk=pCurrentLinkStru->wId;
								pNewLODStru->Off=dwFindDisplacement-(dwFindDisplacementSum-pCurrentLinkStru->dwLength);
								pNewLODStru->Dir=tmpLODDir;
							}					
							chReturnValue=1;
							break;
						}
						else
						{												
							/*�ۼӵ�λ��ֵ���ϵ�ǰLink����С������ƫ������˵��Ŀ��λ�ò��ڵ�ǰlink�ϣ�ѭ��������ǰ������һ��Link*/
						}
					}
					else
					{
						/*��ȡ��һ��linkʧ��*/
						chReturnValue=0;
						break;
					}
				}/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
			}			

		}/*(0!=Displacement) end*/

	}/*if (0!=chFlag) end */
	
	return chReturnValue;

}

/*���ʽӿ�*/
UINT8 dsuLODCalcuExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue =dsuLODCalcuExp_HLT(pDsuEmapStru,pLODStru,Displacement, PointNum,pPointStru,pNewLODStru);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue =dsuLODCalcuExp_DFT(pDsuEmapStru,pLODStru,Displacement, PointNum,pPointStru,pNewLODStru);
	}
	else
	{
		rtnValue = 0;
		
	}
	return rtnValue; 
}

/*
* ���������� ��A��B����λ�ú�B�㷽��õ�����λ�ơ�
* ��������� const LOD_STRU * pLODAStru, A��λ��
*            const LOD_STRU * pLODBStru, B��λ��
*            UINT8 PointNum,             ��������
*            const POINT_STRU * pPointStru, ������Ϣ�ṹ��
* ��������� INT32 *Displacement��       ABλ�ƣ����з����������ű�ʾ����
             ABλ����LinkB����Ϊ���ա�AB������LinkB������ͬ��λ��ֵȡ��������λ��ֵȡ����
* ����ֵ��   1,�ɹ�
*            0,ʧ��
* ʹ��ע�⣺ע��˴�ֻ����B�ķ�����в��ң�δ��עA��ķ��򡣣��Ͽ���෿��Ŀʹ�ã�
*/
static UINT8 dsuDispCalcuByLOCExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
	UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	UINT8 chReturnValue = 0;     /*���ں�������ֵ*/
	UINT8 chFunRtn=0;     /*���ú�������ֵ*/
	UINT16 CurrentLinkId = 0;     /*���ڲ��ҹ�������ʱ���浱ǰLinkID*/
	UINT16 CurrentLinkIndex; /*���ҹ����м�¼Link Index��ʱ����*/

	UINT8 AFindFlag=2;/*0������ʧ�ܡ�1�����ҳɹ���2,�������ҡ�*/
	UINT8 BFindFlag=2;/*0������ʧ�ܡ�1�����ҳɹ���2,�������ҡ�*/
	INT32 AFindDisplacementSum=0;        /*A����B�����е�λ���ۼ�ֵ*/
	INT32 BFindDisplacementSum=0;        /*B����A������λ���ۼ�ֵ*/
	DSU_LINK_STRU *pACurrentLinkStru=NULL;    /*A��ǰLink*/
	DSU_LINK_STRU *pBCurrentLinkStru=NULL;    /*B��ǰLink*/
	UINT16 k;                        /*ѭ���ñ���*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*Ԥ�ú�������ֵΪ0*/
	*Displacement=0;

	/*�ж�����A LinkID����Ч��*/
	if((pLODAStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODAStru->Lnk] == 0xFFFF))
	{
		/*����LinkId��Ч,��ѯʧ�ܣ�����*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�ж�����B LinkID����Ч��*/
	if((pLODBStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODBStru->Lnk] == 0xFFFF))
	{
		/*����LinkId��Ч,��ѯʧ�ܣ�����*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�ж��������linkAƫ��������Ч��*/
	if (pLODAStru->Off > LinkLengthExp(pDsuEmapStru,pLODAStru->Lnk))
	{
		/*��������쳣����ѯʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�ж��������linkBƫ��������Ч��*/
	if (pLODBStru->Off > LinkLengthExp(pDsuEmapStru,pLODBStru->Lnk))
	{
		/*��������쳣����ѯʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�ж�linkB���з������ȷ��*/
	if ((EMAP_SAME_DIR != pLODBStru->Dir) && (EMAP_CONVER_DIR != pLODBStru->Dir))
	{
		/*��������쳣����ѯʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�����������LinkA��LinkB����ͬһLink��*/
	if(pLODAStru->Lnk == pLODBStru->Lnk)
	{
		/*A,Bλ��ͬһlink����������������Ϣ*/
		*Displacement = pLODBStru->Off-pLODAStru->Off;
		
		if (EMAP_SAME_DIR == pLODBStru->Dir)
		{
			/**/
		}
		else
		{
			/*�����෴��ת��λ�Ʒ���*/
			*Displacement=-(*Displacement);
		}

		/*������ѯ�ɹ�������*/		
		chReturnValue=1;
		return 1;
	}


	/*����LinkID��ȡ��ǰlink*/
	CurrentLinkIndex=dsuLinkIndex[pLODAStru->Lnk];
	pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;
	CurrentLinkIndex=dsuLinkIndex[pLODBStru->Lnk];
	pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

	/*����B�ķ���ͬ���з���*/
	/*���һ�������ΪA��B���԰���B�ķ�����ǰ���ң�����ҵ��Է�����¼���ҵ���λ�ƣ����ؽ��*/
	if (pLODBStru->Dir==EMAP_SAME_DIR)
	{
		/*���������*/
		/*�ۼ�λ��ֵ*/
		AFindDisplacementSum=pACurrentLinkStru->dwLength-pLODAStru->Off;
		BFindDisplacementSum=pBCurrentLinkStru->dwLength-pLODBStru->Off;
		for (k=0;k<LINKINDEXNUM;k++)
		{
			/*�˴�ѭ����Ϊ�˼�����ѭ���������ܳ���LINKINDEXNUM*/
			if(AFindFlag==2)
			{
				/*A��ǰ����һ��link*/
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pACurrentLinkStru->wId, EMAP_SAME_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1==chFunRtn )
				{
					/*pACurrentLinkStruָ������Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pACurrentLinkStru->wId==pLODBStru->Lnk)
					{
						/*A�ҵ���B*/

						/*���ҳɹ�*/
						AFindFlag=1;
						/*�ۼ�ƫ������������ֵ*/
						AFindDisplacementSum+=(pLODBStru->Off);
						/*��ʱA�������ҵ���B��ƫ����Ϊ��*/
						*Displacement=AFindDisplacementSum;

						/*���ҳɹ������ؽ��*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*Aû���ҵ�B���ۼ�ƫ��������������*/
						AFindDisplacementSum+=pACurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*��·�յ���ߵ���δ�ҵ���A��ֹͣ����*/
					AFindFlag=0;
				}
				else
				{
					/*A��ȡ��һ��linkʧ��,Linkʧ������������*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			/*B��ǰ����һ��link*/
			if(BFindFlag==2){
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pBCurrentLinkStru->wId,EMAP_SAME_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1== chFunRtn)
				{
					/*pBCurrentLinkStruָ������Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pBCurrentLinkStru->wId==pLODAStru->Lnk)
					{
						/*B�ҵ���A*/
						BFindFlag=1;

						/*�ۼ�ƫ������������ֵ*/
						BFindDisplacementSum+=(pLODAStru->Off);

						/*��ʱB�������ҵ���A��ƫ����Ϊ��*/
						*Displacement=-BFindDisplacementSum;

						/*���ҳɹ������ؽ��*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*���Bû���ҵ�A���ۼ�ƫ��������������*/
						BFindDisplacementSum+=pBCurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*��·�յ���ߵ���δ�ҵ���ֹͣ����*/
					BFindFlag=0;			
				}
				else
				{
					/*B��ȡ��һ��linkʧ��,Linkʧ������������*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			if((AFindFlag==0)&&(BFindFlag==0))
			{
				/*A��B���������Ҳ���û���ҵ��Է�����������ʧ��*/
				chReturnValue=0;
				return chReturnValue;
			}
			else
			{
				/*��ʱAFindFlag��BFindFlag������һ��Ϊ2����������*/
			}

		}
		
		/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
		chReturnValue=0;
		return chReturnValue;

	}
	else /*if (pLODBStru->Dir==CONVERDIR)*/
	{
		/*A,B���ط��������*/
		AFindDisplacementSum=pLODAStru->Off;
		BFindDisplacementSum=pLODBStru->Off;
		for (k=0;k<LINKINDEXNUM;k++)
		{
			/*�˴�ѭ����Ϊ�˼�����ѭ���������ܳ���LINKINDEXNUM*/
			if(AFindFlag==2)
			{
				/*A��ǰ����һ��link*/
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pACurrentLinkStru->wId, EMAP_CONVER_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1==chFunRtn )
				{
					/*pACurrentLinkStruָ������Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pACurrentLinkStru->wId==pLODBStru->Lnk)
					{
						/*A�ҵ���B*/

						/*���ҳɹ�*/
						AFindFlag=1;
						AFindDisplacementSum+=(pACurrentLinkStru->dwLength-pLODBStru->Off);

						/*��ʱA�ط����ҵ���B��ƫ����Ϊ��*/
						*Displacement=AFindDisplacementSum;

						/*���ҳɹ������ؽ��*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*���Aû���ҵ�B���ۼ�ƫ��������������*/
						AFindDisplacementSum+=pACurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*��·�յ���ߵ���δ�ҵ���ֹͣ����*/
					AFindFlag=0;
				}
				else/*( 0==chReturnValue )*/
				{
					/*A��ȡ��һ��linkʧ��,Linkʧ������������*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			/*B��ǰ����һ��link*/
			if(BFindFlag==2){
				chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pBCurrentLinkStru->wId, EMAP_CONVER_DIR, PointNum, pPointStru,&CurrentLinkId);
				if( 1== chFunRtn)
				{
					/*pACurrentLinkStruָ������Link*/
					CurrentLinkIndex=dsuLinkIndex[CurrentLinkId];
					pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+CurrentLinkIndex;

					if(pBCurrentLinkStru->wId==pLODAStru->Lnk)
					{
						/*B�ҵ���A*/

						/*���ҳɹ�*/
						BFindFlag=1;
						BFindDisplacementSum+=(pBCurrentLinkStru->dwLength-pLODAStru->Off);

						/*��ʱB�ط����ҵ���A��ƫ����Ϊ��*/
						*Displacement=-BFindDisplacementSum;

						/*���ҳɹ������ؽ��*/
						chReturnValue=1;
						return chReturnValue;
					}
					else
					{
						/*���Bû���ҵ�A���ۼ�ƫ��������������*/
						BFindDisplacementSum+=pBCurrentLinkStru->dwLength;
					}
				}
				else if( 0xff==chFunRtn )
				{
					/*��·�յ���ߵ���δ�ҵ���ֹͣ����*/
					BFindFlag=0;			
				}
				else /*( 0==chReturnValue )*/
				{
					/*B��ȡ��һ��linkʧ��,Linkʧ������������*/
					chReturnValue=0;
					return chReturnValue;
				}
			}

			if((AFindFlag==0)&&(BFindFlag==0))
			{
				/*A��B���������Ҳ���û���ҵ��Է�����������ʧ��*/
				chReturnValue=0;
				return chReturnValue;
			}
			else
			{
				/*��ʱAFindFlag��BFindFlag������һ��Ϊ2����������*/
			}

		}

		/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
		chReturnValue=0;
		return chReturnValue;

	}
}

/*
* ���������� ��A��B����λ�ú�A�㷽��õ�����λ�ơ�
* ��������� const LOD_STRU * pLODAStru, A��λ��
*            const LOD_STRU * pLODBStru, B��λ��
*            UINT8 PointNum,             ��������
*            const POINT_STRU * pPointStru, ������Ϣ�ṹ��
* ��������� INT32 *Displacement��       ABλ�ƣ����з����������ű�ʾ����
             ABλ����LinkA����Ϊ���ա�AB������LinkA������ͬ��λ��ֵȡ��������λ��ֵȡ����
* ����ֵ��   1,�ɹ�
*            0,ʧ��
* ʹ��ע�⣺ע��˴�ֻ����A�ķ�����в��ң�δ��עB��ķ��򡣣�������ͨ��Ŀʹ�ã�
* author:qxt 20170811
*/
static UINT8 dsuDispCalcuByLOCExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
									  UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	UINT8 chReturnValue =0;     /*���ں�������ֵ*/
	UINT8 chFunRtn=0;     /*���ú�������ֵ*/
	UINT8 chFlag=1;           /*����ִ��ʧ�ܱ��*/
	UINT16 wCurrentLinkId = 0;     /*���ڲ��ҹ�������ʱ���浱ǰLinkID*/
	UINT16 wCurrentLinkIndex=0; /*���ҹ����м�¼Link Index��ʱ����*/

	UINT8 wAFindFlag=2;/*0������ʧ�ܡ�1�����ҳɹ���2,�������ҡ�*/
	UINT8 wBFindFlag=2;/*0������ʧ�ܡ�1�����ҳɹ���2,�������ҡ�*/
	INT32 dwAFindDisplacementSum=0;        /*A����B�����е�λ���ۼ�ֵ*/
	INT32 dwBFindDisplacementSum=0;        /*B����A������λ���ۼ�ֵ*/
	DSU_LINK_STRU *pACurrentLinkStru=NULL;    /*A��ǰLink*/
	DSU_LINK_STRU *pBCurrentLinkStru=NULL;    /*B��ǰLink*/
	UINT16 k=0;                        /*ѭ���ñ���*/
	UINT8 wAFindDir = 0;               /*���ڼ�¼ʵ�ʲ���A����B�����еĲ��ҷ���,��ʼֵΪA�ķ��� */
    UINT8 wBFindDir = 0;               /*���ڼ�¼ʵ�ʲ���B����A�����еĲ��ҷ���,��ʼֵΪA�ķ��� */
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

	/*Ԥ�ú�������ֵΪ0*/
	*Displacement=0;

	if ((NULL != pDsuEmapStru)&&(NULL!=pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*�ж�����A LinkID����Ч��*/
		if((pLODAStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16 == dsuLinkIndex[pLODAStru->Lnk]))
		{
			/*����LinkId��Ч,��ѯʧ��*/
			chFlag=0;
		}

		/*�ж�����B LinkID����Ч��*/
		if((pLODBStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16 == dsuLinkIndex[pLODBStru->Lnk]))
		{
			/*����LinkId��Ч,��ѯʧ��*/
			chFlag=0;
		}

		/*�ж��������linkAƫ��������Ч��*/
		if (pLODAStru->Off > LinkLengthExp(pDsuEmapStru,pLODAStru->Lnk))
		{
			/*��������쳣����ѯʧ��*/
			chFlag=0;
		}

		/*�ж��������linkBƫ��������Ч��*/
		if (pLODBStru->Off > LinkLengthExp(pDsuEmapStru,pLODBStru->Lnk))
		{
			/*��������쳣����ѯʧ��*/
			chFlag=0;
		}

		/*�ж�linkA���з������ȷ��*/
		if ((EMAP_SAME_DIR != pLODAStru->Dir) && (EMAP_CONVER_DIR != pLODAStru->Dir))
		{
			/*��������쳣����ѯʧ��*/
			chFlag=0;
		}
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chFlag = 0;		
	}	

	
	if (0!=chFlag)
	{
		/*�����������LinkA��LinkB����ͬһLink��*/
		if(pLODAStru->Lnk == pLODBStru->Lnk)
		{
			/*A,Bλ��ͬһlink����������������Ϣ*/
			*Displacement = pLODBStru->Off-pLODAStru->Off;

			if (EMAP_SAME_DIR == pLODAStru->Dir)
			{
				/*���账��*/
			}
			else
			{
				/*�����෴��ת��λ�Ʒ���*/
				*Displacement=-(*Displacement);
			}

			/*������ѯ�ɹ�*/
			chReturnValue = 1;
			
		}
		else/*A��B����ͬһlink*/
		{
			/*����LinkID��ȡ��ǰlink*/
			wCurrentLinkIndex=dsuLinkIndex[pLODAStru->Lnk];
			pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;
			wCurrentLinkIndex=dsuLinkIndex[pLODBStru->Lnk];
			pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

			/*��ʼ���ҷ���*/
			wAFindDir=pLODAStru->Dir;
			wBFindDir=pLODAStru->Dir;
			/*����A�ķ���ͬ���з���*/
			if (EMAP_SAME_DIR==pLODAStru->Dir)
			{
				/*���������*/
				/*�ۼ�λ��ֵ*/
				dwAFindDisplacementSum=pACurrentLinkStru->dwLength-pLODAStru->Off;
				dwBFindDisplacementSum=pBCurrentLinkStru->dwLength-pLODBStru->Off;
			}
			else /*if (pLODAStru->Dir==CONVERDIR)*/
			{
				/*A,B���ط��������*/
				dwAFindDisplacementSum=pLODAStru->Off;
				dwBFindDisplacementSum=pLODBStru->Off;
			}

			/*���һ�������ΪA��B���԰���A�ķ�����ǰ���ң�����ҵ��Է�����¼���ҵ���λ�ƣ����ؽ��*/
			for (k=0;k<LINKINDEXNUM;k++)
			{
				/*�˴�ѭ����Ϊ�˼�����ѭ���������ܳ���LINKINDEXNUM*/
				if(2==wAFindFlag)
				{
					/*A��ǰ����һ��link*/
					chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pACurrentLinkStru->wId, wAFindDir, PointNum, pPointStru,&wCurrentLinkId);
					if( 1==chFunRtn )
					{
						/*�����LinkΪ�߼�����仯��(��ѯ����Ϊ���Ҹ�link���յ�Ϊ�߼�����仯�� �� ��ѯ�������Ҹ�link�����Ϊ�߼�����仯)*/
						/* 0x55��ʾlink�յ�Ϊ�߼�����仯�㣬0xaa��ʾlink���Ϊ�߼�����仯��*/
						if(((EMAP_SAME_DIR==wAFindDir)&&(0x55==pACurrentLinkStru->wLogicDirChanged))||  
							((EMAP_CONVER_DIR==wAFindDir)&&(0xaa==pACurrentLinkStru->wLogicDirChanged)))
						{							 
							wAFindDir=~wAFindDir;/*�б仯�㣬�ı���ҷ���*/
						}

						/*pACurrentLinkStruָ������Link*/
						wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkId];
						pACurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

						if(pACurrentLinkStru->wId==pLODBStru->Lnk)
						{
							/*A�ҵ���B*/

							/*���ҳɹ�*/
							wAFindFlag=1;
							/*�ۼ�ƫ������������ֵ*/
							if (EMAP_SAME_DIR==wAFindDir)/*�����ҵ�B��ʱ�Ĳ�ѯ����Ϊ��ʱ*/
							{
								dwAFindDisplacementSum+=(pLODBStru->Off);
							}
							else/*�����ҵ�B��ʱ�Ĳ�ѯ����Ϊ��ʱ*/
							{
								dwAFindDisplacementSum+=(pACurrentLinkStru->dwLength-pLODBStru->Off);
							}
							/*��ʱA��A�ķ����ҵ���B��ƫ����Ϊ��*/
							*Displacement=dwAFindDisplacementSum;
                             chReturnValue = 1;
							/*���ҳɹ����˳�*/
							break;
							
						}
						else
						{
							/*Aû���ҵ�B���ۼ�ƫ��������������*/
							dwAFindDisplacementSum+=pACurrentLinkStru->dwLength;
						}
					}
					else if( 0xff==chFunRtn )
					{
						/*��·�յ���ߵ���δ�ҵ���A��ֹͣ����*/
						wAFindFlag=0;
					}
					else
					{
						/*A��ȡ��һ��linkʧ��,Linkʧ������������*/
						chReturnValue=0;
						break;
					}
				}

				/*B��ǰ����һ��link*/
				if(2==wBFindFlag){
					chFunRtn=dsuGetAdjacentLinkIDExp(pDsuEmapStru,pBCurrentLinkStru->wId,wBFindDir, PointNum, pPointStru,&wCurrentLinkId);
					if( 1== chFunRtn)
					{
						/*�����LinkΪ�߼�����仯��(��ѯ����Ϊ���Ҹ�link���յ�Ϊ�߼�����仯�� �� ��ѯ�������Ҹ�link�����Ϊ�߼�����仯)*/
						/* 0x55��ʾlink�յ�Ϊ�߼�����仯�㣬0xaa��ʾlink���Ϊ�߼�����仯��*/
						if(((EMAP_SAME_DIR==wBFindDir)&&(0x55==pBCurrentLinkStru->wLogicDirChanged))||  
							((EMAP_CONVER_DIR==wBFindDir)&&(0xaa==pBCurrentLinkStru->wLogicDirChanged)))
						{							 
							wBFindDir=~wBFindDir;/*�б仯�㣬�ı���ҷ���*/
						}
						/*pBCurrentLinkStruָ������Link*/
						wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkId];
						pBCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

						if(pBCurrentLinkStru->wId==pLODAStru->Lnk)
						{
							/*B�ҵ���A*/
							wBFindFlag=1;

							/*�ۼ�ƫ������������ֵ*/
							if (EMAP_SAME_DIR==wBFindDir)/*�����ҵ�B��ʱ�Ĳ�ѯ����Ϊ��ʱ*/
							{
								dwBFindDisplacementSum+=(pLODAStru->Off);
							}
							else/*�����ҵ�B��ʱ�Ĳ�ѯ����Ϊ��ʱ*/
							{
								dwBFindDisplacementSum+=(pBCurrentLinkStru->dwLength-pLODAStru->Off);
							}

							/*��ʱB��A�ķ����ҵ���A��ƫ����Ϊ��*/
							*Displacement=-dwBFindDisplacementSum;
                             chReturnValue = 1;
							/*���ҳɹ����˳�*/
							break;
						}
						else
						{
							/*���Bû���ҵ�A���ۼ�ƫ��������������*/
							dwBFindDisplacementSum+=pBCurrentLinkStru->dwLength;
						}
					}
					else if( 0xff==chFunRtn )
					{
						/*��·�յ���ߵ���δ�ҵ���ֹͣ����*/
						wBFindFlag=0;			
					}
					else
					{
						/*B��ȡ��һ��linkʧ��,Linkʧ������������*/
						chReturnValue=0;
						break;
					}
				}

				if((0==wAFindFlag)&&(0==wBFindFlag))
				{
					/*A��B���������Ҳ���û���ҵ��Է�����������ʧ��*/
					chReturnValue=0;
					break;
				}
				else
				{
					/*��ʱAFindFlag��BFindFlag������һ��Ϊ2����������*/
				}

			}/*forѭ��end*/

			/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
			/*���û�ҵ�����ʱchReturnValue=0*/
		}
	}/*��0!=chReturnValue��end*/

	return chReturnValue;
}


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
* ʹ��ע�⣺�Ͽ���෿��Ŀ�㷨�ǰ���B�ķ�����ң�ABλ����LinkB����Ϊ���գ�
            ������ͨ��Ŀ�������߼�����仯�㣬�㷨��Ϊ����A�ķ�����ң�ABλ����LinkA����Ϊ���ա�
* author:qxt 20170811
*/
UINT8 dsuDispCalcuByLOCExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
						   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue=dsuDispCalcuByLOCExp_HLT(pDsuEmapStru, pLODAStru, pLODBStru,PointNum,pPointStru,Displacement);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue=dsuDispCalcuByLOCExp_DFT(pDsuEmapStru, pLODAStru, pLODBStru,PointNum,pPointStru,Displacement);
	}
	else
	{
		rtnValue = 0;		
	}
	return rtnValue; 
}


/*
* �������ܣ�	�˺����Ĺ����ǲ�ѯ����λ�ü������link���С�
* ****************************************************************************************************
*        1          2              3                4                    5              6
*       -----��---------*--��--------------��--------------------��-------*-----��------------
*                       S                                                 E
*        ͼ����  ����Link�˵㡣*����ѯ�����λ�á�1��2��3...Linkʾ����
*        ��ͼ��ʾ��Link3��4Ϊ��������
*        �˴�������pLODEndStru���������ң�
*        S����pLODEndStru->Dir������ң��ҵ�E�򷵻سɹ����Ҳ����򷵻�ʧ�ܡ�
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
*               �Ͽ���෿��Ŀʹ��
*/
static UINT8 dsuGetLinkBtwLocExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
					   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
					   UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber)
{
	UINT16 chFindLinkNum = 0;   /*�ҵ���link����Ŀ*/
	UINT16 wCurrentLinkID = 0;  /*��ǰlink���*/
	UINT16 wNextLinkID = 0;     /*����link���*/
	UINT16 wFindLink[DSU_MAX_LINK_BETWEEN];/*�ҵ���link��������*/
	UINT16 i = 0;           /*ѭ���ñ���*/
	UINT8 chReturnValue = 0;/*��������ֵ*/
	UINT16 k;                        /*ѭ���ñ���*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*Link��������ֵԤ��0*/
	*wLinkNumber=0;

	/*�ж��������Link����Ч��*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF)
	    ||(pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�ж�������ݷ������ȷ��*/
	if ((pLODEndStru->Dir != EMAP_SAME_DIR) && (pLODEndStru->Dir != EMAP_CONVER_DIR))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		chReturnValue = 0;
		return chReturnValue;
	}

	if(pLODStartStru->Lnk==pLODEndStru->Lnk)
	{
		/*ʼ�����ն���ͬһ��Link�ϣ�*/

		/*link��Ϊ0*/
		*wLinkNumber=0;


		if (((pLODEndStru->Dir==EMAP_SAME_DIR)&&(pLODEndStru->Off>=pLODStartStru->Off))
			||((pLODEndStru->Dir==EMAP_CONVER_DIR)&&(pLODEndStru->Off<=pLODStartStru->Off)))
		{	
			/*��ѯȷ������������ܹ������յ�*/
			/*���ز�ѯ�ɹ����˳�*/
			chReturnValue = 1;
			return chReturnValue;	
		}
		else
		{
			/*���ز�ѯ�ɹ����˳�*/
			chReturnValue = 0;
			return chReturnValue;	
		}

	}	
	
	/*��ʼ��link��Ϊ��ǰlink*/
	wCurrentLinkID = pLODStartStru->Lnk;

	/*���ҵ��ĵ�ǰlink�������ն�linkʱ������whileѭ��*/
	for (k=0;k<LINKINDEXNUM;k++)
	{
		/*�˴�ѭ����Ϊ�˼�����ѭ���������ܳ���LINKINDEXNUM*/

		/*���ù��ܺ�������ѯ��ǰlink������link������ѯ��ǰlink������link*/
		chReturnValue=dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkID, pLODEndStru->Dir,  PointNum, pPointStru,&wNextLinkID);

		/*ͨ����������ֵ���ж��Ƿ���ȷ*/
		if (chReturnValue!=1)
		{
			/*��ѯʧ�ܣ����ز�ѯ����*/
			chFindLinkNum=0;
			chReturnValue = 0;
			return chReturnValue;
		}
		
		/*���ҵ�������link��Ϊ�µĵ�ǰlink*/
		wCurrentLinkID=wNextLinkID;
		
		if(wCurrentLinkID != pLODEndStru->Lnk)
		{
			if (chFindLinkNum>=DSU_MAX_LINK_BETWEEN)
			{
				/*����������������ʧ�ܣ��������ʵ��ִ��ʱ��Ӧ�ó���*/
				chFindLinkNum=0;
				chReturnValue = 0;
				return chReturnValue;
			}
			else
			{
				/*��ǰLink�����ڽ���Link�����¾ֲ����飬ͬʱ���ҵ���link��Ŀ��һ*/
				wFindLink[chFindLinkNum] = wCurrentLinkID;
				chFindLinkNum++;
			}
	
		}
		else
		{
			/*��ǰLink�ǽ���Link����ǰLink��д�����У��ҵ��ն�link���˳�ѭ��*/
			break;
		}


	}

	if(k==LINKINDEXNUM)
	{
		/*ѭ����Ϊk==LINKINDEXNUM������˵�����Ҵ�������LINKINDEXNUM�������󲻿��ܷ���*/
		chReturnValue=0;
		return chReturnValue;
	}

	/*�˳�ѭ�����ҵ��ն�link,����ҵ���link����*/
	for (i=0; i<chFindLinkNum; i++)
	{
		wLinkID[i] = wFindLink[i];
	}

	/*�ҵ���Link������ֵ*/
	*wLinkNumber=chFindLinkNum;

	/*���ز�ѯ�ɹ����˳�*/
	chReturnValue = 1;
	return chReturnValue;	
}

/*
* �������ܣ�	�˺����Ĺ����ǲ�ѯ����λ�ü������link���С�
* ****************************************************************************************************
*        1          2              3                4                    5              6
*       -----��---------*--��--------------��--------------------��-------*-----��------------
*                       S                                                 E
*        ͼ����  ����Link�˵㡣*����ѯ�����λ�á�1��2��3...Linkʾ����
*        ��ͼ��ʾ��Link3��4Ϊ��������
*        �˴�������pLODStartStru���������ң�
*        S����pLODStartStru->Dir������ң��ҵ�E�򷵻سɹ����Ҳ����򷵻�ʧ�ܡ�
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
*               �ڲ����ú�����������ͨ��Ŀʹ��
*  author: qxt 20170814
*/
static UINT8 dsuGetLinkBtwLocExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
									 UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
									 UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber)
{
	UINT16 chFindLinkNum = 0;   /*�ҵ���link����Ŀ*/
	UINT16 wCurrentLinkID = 0;  /*��ǰlink���*/
	UINT16 wNextLinkID = 0;     /*����link���*/
	UINT16 wFindLink[DSU_MAX_LINK_BETWEEN];/*�ҵ���link��������*/
	UINT16 i = 0;           /*ѭ���ñ���*/
	UINT8 chReturnValue = 0;/*��������ֵ*/
	UINT16 k=0;                        /*ѭ���ñ���*/
	UINT16 wCurrentLinkIndex = 0; /*��ǰLink Index */
	DSU_LINK_STRU *pCurrentLinkStru=NULL;/*����ѭ�����ҹ�������ʱ���浱ǰLink�ṹ��*/
	UINT8 wFindDir = 0;               /*���ڼ�¼ʵ�ʲ��������еĲ��ҷ���*/
	UINT8 chFlag=1;      /*����ִ��ʧ�ܱ�ʶλ*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;/*add by qxt 20161029*/
	/*Link��������ֵԤ��0*/
	*wLinkNumber=0;
	
	if ((NULL != pDsuEmapStru)&&(NULL !=pDsuEmapStru->dsuEmapIndexStru)&&(NULL!=pDsuEmapStru->dsuStaticHeadStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;/*add by qxt 20161029*/

		/*�ж��������Link����Ч��*/
		if((pLODStartStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16==dsuLinkIndex[pLODStartStru->Lnk])
			||(pLODEndStru->Lnk>LINKINDEXNUM)||(DSU_NULL_16==dsuLinkIndex[pLODEndStru->Lnk]))
		{
			/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
			chFlag = 0;
		}

		/*�ж�������ݷ������ȷ��*/
		if ((EMAP_SAME_DIR!=pLODStartStru->Dir) && (EMAP_CONVER_DIR!=pLODStartStru->Dir))
		{
			/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
			chFlag = 0;
		}
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chFlag = 0;
	}

	

	if (0!=chFlag)
	{
		/*ʼ�����ն���ͬһ��Link�ϣ�*/
		if(pLODStartStru->Lnk==pLODEndStru->Lnk)
		{			
     		/*link��Ϊ0*/
			*wLinkNumber=0;
			if (((EMAP_SAME_DIR==pLODStartStru->Dir)&&(pLODEndStru->Off>=pLODStartStru->Off))
				||((EMAP_CONVER_DIR==pLODStartStru->Dir)&&(pLODEndStru->Off<=pLODStartStru->Off)))
			{	
				/*��ѯȷ������������ܹ������յ�*/
				/*���ز�ѯ�ɹ����˳�*/
				chReturnValue = 1;
			}
			else
			{
				/*���ز�ѯ�ɹ����˳�*/
				chReturnValue = 0;
			}

		}
		else/*ʼ�����ն˲���ͬһ��Link�� start*/
		{
			/*��ʼ��link��Ϊ��ǰlink*/
			wCurrentLinkID = pLODStartStru->Lnk;

			/*����LinkID�����ݿ��в��ҵ�ǰlink��Ϣ*/
			wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkID];
			pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;
			/*����ʼ��ѯ��������Ϊ���ķ��� */
			wFindDir=pLODStartStru->Dir;

			/*���ҵ��ĵ�ǰlink�������ն�linkʱ������whileѭ��*//*�˴�ѭ����Ϊ�˼�����ѭ���������ܳ���LINKINDEXNUM*/
			for (k=0;k<LINKINDEXNUM;k++)
			{		
				/*���ù��ܺ�������ѯ��ǰlink������link������ѯ��ǰlink������link*/
				chReturnValue=dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkID,wFindDir,  PointNum, pPointStru,&wNextLinkID);

				/*ͨ����������ֵ���ж��Ƿ���ȷ*/
				if (chReturnValue!=1)
				{
					/*��ѯʧ�ܣ����ز�ѯ����*/
					chFindLinkNum=0;
					chReturnValue = 0;
					break;
				}


				/*�жϸ�Link�Ƿ����߼�����仯��(��ѯ����Ϊ���Ҹ�link���յ�Ϊ�߼�����仯�� �� ��ѯ�������Ҹ�link�����Ϊ�߼�����仯)*/
				if(((EMAP_SAME_DIR==wFindDir)&&(0x55==pCurrentLinkStru->wLogicDirChanged))||  
					((EMAP_CONVER_DIR==wFindDir)&&(0xaa==pCurrentLinkStru->wLogicDirChanged)))
				{							 
					wFindDir=~wFindDir;/*�б仯�㣬�ı���ҷ���*/
				}

				/*���ҵ�������link��Ϊ�µĵ�ǰlink*/
				wCurrentLinkID=wNextLinkID;

				/*pCurrentLinkStruָ������Link,����Link��Ϊ��ǰLink*/
				wCurrentLinkIndex=dsuLinkIndex[wCurrentLinkID];
				pCurrentLinkStru=dsuStaticHeadStru->pLinkStru+wCurrentLinkIndex;

				if(wCurrentLinkID != pLODEndStru->Lnk)
				{
					if (chFindLinkNum>=DSU_MAX_LINK_BETWEEN)
					{
						/*����������������ʧ�ܣ��������ʵ��ִ��ʱ��Ӧ�ó���*/
						chFindLinkNum=0;
						chReturnValue = 0;
						break;
					}
					else
					{
						/*��ǰLink�����ڽ���Link�����¾ֲ����飬ͬʱ���ҵ���link��Ŀ��һ*/
						wFindLink[chFindLinkNum] = wCurrentLinkID;
						chFindLinkNum++;
					}

				}
				else
				{
					/*��ǰLink�ǽ���Link����ǰLink��д�����У��ҵ��ն�link���˳�ѭ��*/
					break;
				}

			}/*forѭ������*/

			/*���Ҵ�������LINKINDEXNUM*/
			if((k!=LINKINDEXNUM)&&(0!=chReturnValue))
			{
				/*�˳�ѭ�����ҵ��ն�link,����ҵ���link����*/
				for (i=0; i<chFindLinkNum; i++)
				{
					wLinkID[i] = wFindLink[i];
				}

				/*�ҵ���Link������ֵ*/
				*wLinkNumber=chFindLinkNum;
				/*���ز�ѯ�ɹ�*/
				chReturnValue = 1;

			}		

		}/*ʼ�����ն˲���ͬһ��Link�� end*/		

	}/*(0!=chFlag) end*/	
	return chReturnValue;	
}

/*���ʽӿ�*/
UINT8 dsuGetLinkBtwLocExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						  UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						  UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue=dsuGetLinkBtwLocExp_HLT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,wLinkID,wLinkNumber);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue=dsuGetLinkBtwLocExp_DFT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,wLinkID,wLinkNumber);
	}
	else
	{
		rtnValue = 0; 
	}
	return rtnValue;
}

/*
* �������ܣ� �˺����Ĺ����ǽ��жϵ�ǰλ���Ƿ���ָ������Χ�ڡ�
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       �����㷨Ϊ������λ��DisplacementSC��DisplacementCE���������λ�Ʒ�����ͬ��˵��C��SE������
*
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
*  �Ͽ���෿ʹ��
*/
static UINT8 dsuCheckLocInAreaExp_DFT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU * pPointStru, 
						const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 chReturnValue = 0;/*��������ֵ*/
	INT32 DisplacementSC; /*start -> current λ��*/
	INT32 DisplacementCE; /*current -> end λ�� */
	INT32 DisplacementSE; /*start -> end λ�� */
	LOD_STRU CurrentLODStru; /*����һ���������洢��ǰLOD������ȡpLODEndStru���������ڼ���λ��ʱ��
							  ����ͳһ�ķ���������λ��*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;
		return chReturnValue;
	}


	/*Ĭ���ò���������*/
	*pLocInArea=0;

	/*�ж�������ݵ�ǰλ��link����������Ч��*/
	if((pCurrentLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pCurrentLODStru->Lnk] == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж�������ݵ�ǰλ��ƫ�����������Ч��*/
	if (pCurrentLODStru->Off > LinkLengthExp(pDsuEmapStru,pCurrentLODStru->Lnk))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж�������ݵ�ǰλ�÷������Ч��*/
	if ((EMAP_SAME_DIR != pCurrentLODStru->Dir) && (EMAP_CONVER_DIR != pCurrentLODStru->Dir))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	
	/*�ж�����������λ��link����������Ч��*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж�����������λ��linkƫ�����������Ч��*/
	if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж����������㷽�����Ч��*/
	if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�ж�������������յ�λ��link��ŵ���Ч��*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж���������յ�λ��linkƫ�����������Ч��*/
	if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж���������յ㷽�����Ч��*/
	if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}


	CurrentLODStru.Lnk=pCurrentLODStru->Lnk;
	CurrentLODStru.Off=pCurrentLODStru->Off;
	CurrentLODStru.Dir=pLODEndStru->Dir;

	/*����λ���ж�3���ϵ*/

	/*��ѯDisplacementSE*/
	chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,pLODStartStru, pLODEndStru,  PointNum,   pPointStru, &DisplacementSE);
	if(chReturnValue!=1)
	{
		/*λ�Ʋ�ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/*SE������ڣ���������ֵΪ1*/
		chReturnValue=1;
	}

	/*��ѯDisplacementSC*/
	chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,pLODStartStru, &CurrentLODStru,  PointNum,   pPointStru, &DisplacementSC);
	if(chReturnValue!=1)
	{
		/*λ�Ʋ�ѯʧ��*/
		*pLocInArea=0;
		chReturnValue=0;
		return chReturnValue;
	}

	/*��ѯDisplacementCE*/
	chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,&CurrentLODStru, pLODEndStru,  PointNum,   pPointStru, &DisplacementCE);
	if(chReturnValue!=1)
	{
		/*λ�Ʋ�ѯʧ��*/
		*pLocInArea=0;
		chReturnValue=0;
		return chReturnValue;
	}

	if((DisplacementSC==0)&&(DisplacementCE==0))
	{
		/*�����غ�*/
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}

	if(DisplacementSC==0)
	{
		/*ֻ�����������غ�*/
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}

	if(DisplacementCE==0)
	{
		/*��ǰλ���������յ�*/
		*pLocInArea=0;
		chReturnValue = 1;
		return chReturnValue;
	}

	/*�ж�DisplacementSC��DisplacementCE�����Ƿ���ͬ*/
	if( ((DisplacementSC>0)&&(DisplacementCE>0))||((DisplacementSC<0)&&(DisplacementCE<0)))
	{
		/*DisplacementSC��DisplacementCE������ͬ*/
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}
	else
	{
		/*DisplacementSC��DisplacementCE���Ų���ͬ����ǰλ�ò���������*/
		*pLocInArea=0;
		chReturnValue = 1;
		return chReturnValue;
	}

}

/*
* �������ܣ� �˺����Ĺ����ǽ��жϵ�ǰλ���Ƿ���ָ������Χ�ڡ�
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       ������Link�߼�����仯�㣬�����㷨��Ϊ������λ��DisplacementCS��DisplacementCE���������λ�Ʒ������죬˵��C��SE������
*
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
*author:qxt 20170811
*/
static UINT8 dsuCheckLocInAreaExp_HLT(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						const LOD_STRU *pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 chReturnValue = 0;/*��������ֵ*/
	UINT8 chFlag=1;     /*����ִ��ʧ�ܱ�ʶλ*/
	UINT8 chRtnCS = 0;/*����λ��CS��������ֵ*/
	UINT8 chRtnCE = 0;/*����λ��CE��������ֵ*/
	INT32 dwDisplacementCS=0; /*current -> start λ��*/
	INT32 dwDisplacementCE=0; /*current -> end λ�� */
	INT32 dwDisplacementSE=0; /*start -> end λ�� */
	LOD_STRU struCurrentLOD; /*����һ���������洢��ǰLOD������ȡpLODStartStru���������ڼ���λ��ʱ��
							  ����ͳһ�ķ���������λ��*/
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;

	/*Ĭ���ò���������*/
	*pLocInArea=0;

	if ((NULL != pDsuEmapStru)&&(NULL !=pDsuEmapStru->dsuEmapIndexStru))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;

		/*�ж�������ݵ�ǰλ��link����������Ч��*/
		if((pCurrentLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pCurrentLODStru->Lnk] == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
		/*�ж�������ݵ�ǰλ��ƫ�����������Ч��*/
		if (pCurrentLODStru->Off > LinkLengthExp(pDsuEmapStru,pCurrentLODStru->Lnk))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
		/*�ж�������ݵ�ǰλ�÷������Ч��*/
		if ((EMAP_SAME_DIR != pCurrentLODStru->Dir) && (EMAP_CONVER_DIR != pCurrentLODStru->Dir))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}

		/*�ж�����������λ��link����������Ч��*/
		if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
		/*�ж�����������λ��linkƫ�����������Ч��*/
		if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
		/*�ж����������㷽�����Ч��*/
		if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}

		/*�ж�������������յ�λ��link��ŵ���Ч��*/
		if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
		/*�ж���������յ�λ��linkƫ�����������Ч��*/
		if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
		/*�ж���������յ㷽�����Ч��*/
		if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
		{
			/*��ڲ����쳣����ѯʧ��*/
			chFlag = 0;
		}
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chFlag = 0;
	}

	
	if (0!=chFlag)
	{
		/*����λ���ж�3���ϵ*/
		struCurrentLOD.Lnk=pCurrentLODStru->Lnk;
		struCurrentLOD.Off=pCurrentLODStru->Off;
		struCurrentLOD.Dir=pLODStartStru->Dir;
		/*��ѯDisplacementSE*/
		chReturnValue= dsuDispCalcuByLOCExp(pDsuEmapStru,pLODStartStru, pLODEndStru,  PointNum,   pPointStru, &dwDisplacementSE);
		if(chReturnValue!=1)
		{
			/*λ�Ʋ�ѯʧ��*/
			chReturnValue = 0;
		}
		else/*SE�������*/
		{
			
			/*��ѯDisplacementCS*/
			chRtnCS= dsuDispCalcuByLOCExp(pDsuEmapStru,&struCurrentLOD,pLODStartStru,PointNum,   pPointStru, &dwDisplacementCS);

			/*��ѯDisplacementCE*/
			chRtnCE= dsuDispCalcuByLOCExp(pDsuEmapStru,&struCurrentLOD,pLODEndStru,PointNum, pPointStru, &dwDisplacementCE);

			if((1!=chRtnCS)||(1!=chRtnCE))
			{
				/*λ�Ʋ�ѯʧ��*/
				*pLocInArea=0;
				chReturnValue=0;
			}

			else
			{
				/*�����غ�*/
				if((0==dwDisplacementCS)&&(0==dwDisplacementCE))
				{
					*pLocInArea=1;
					chReturnValue = 1;
				}
                /*ֻ�����������غ�*/
				else if(0==dwDisplacementCS)
				{
					
					*pLocInArea=1;
					chReturnValue = 1;
				}

				/*��ǰλ���������յ�*/
				else if(0==dwDisplacementCE)
				{
					
					*pLocInArea=0;
					chReturnValue = 1;
				}

				/*�ж�DisplacementCS��DisplacementCS�����Ƿ�ͬ changed by qxt*/
				else if( ((dwDisplacementCS>0)&&(dwDisplacementCE<0))||((dwDisplacementCS<0)&&(dwDisplacementCE>0)))
				{
					/*DisplacementCS��DisplacementCE���Ų���ͬ*/
					*pLocInArea=1;
					chReturnValue = 1;
				}
				else
				{
					/*DisplacementCS��DisplacementCE������ͬ����ǰλ�ò���������*/
					*pLocInArea=0;
					chReturnValue = 1;
				}

			}		
		}/*SE������� end*/	
	}

	return chReturnValue;
}

/*���ʽӿ�*/
UINT8 dsuCheckLocInAreaExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
						   UINT8 PointNum, const DQU_POINT_STATUS_STRU * pPointStru, 
						   const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_HLT == cbtcVersionType)
	{
		rtnValue =dsuCheckLocInAreaExp_HLT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,pCurrentLODStru,pLocInArea);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_FAO == cbtcVersionType)|| (DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue =dsuCheckLocInAreaExp_DFT(pDsuEmapStru,pLODStartStru,pLODEndStru,PointNum,pPointStru,pCurrentLODStru,pLocInArea);
	}
	else
	{
		rtnValue = 0;
		
	}
	return rtnValue; 
}

/************************************************************************

�������ܣ�	�˺����Ĺ���������link��ID��Ϣ��ѯlink�ĳ�����Ϣ
��ڲ�����	const UINT16 wLinkID		link��ID	
���ڲ�����	��
����ֵ��	UINT32 dwLinkLength	��ѯʧ��: 0xffffffff; ��ѯ�ɹ�: ���ز��ҵ���link����                                                                
************************************************************************/
/*���ܺ�������ѯlink����*/
UINT32 LinkLengthExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 wLinkID)
{
	UINT32 dwLinkLength = 0;/*��������ֵ*/
	DSU_LINK_STRU *pstLinK;/*ָ��link��Ϣ�Ľṹ��ָ��*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		dwLinkLength = 0xffffffff;
		return dwLinkLength;
	}

	/*�ж������������Ч��*/
	if((wLinkID>LINKINDEXNUM)||(dsuLinkIndex[wLinkID] == 0xFFFF))
	{
		/*��Ч�������룬��ѯʧ�ܣ�����Ĭ��ֵ*/
		dwLinkLength = 0xffffffff;
		return dwLinkLength;
	}
	/*���ù������ҵ�link������Ϣ������*/
	pstLinK = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[wLinkID];
	dwLinkLength = pstLinK->dwLength;
	return dwLinkLength;
}

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
UINT8 dsuCheckLocInAreaQuickExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
	const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	UINT8 chReturnValue = 0;/*��������ֵ*/
	UINT16 i = 0;          /*ѭ���ñ���*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*Ĭ���ò���������*/
	*pLocInArea=0;

	/*�ж���������������Ч��*/
	if((pCurrentLODStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pCurrentLODStru->Lnk] == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж���������������Ч��*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	/*�ж���������������Ч��*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�����غϣ�������������*/
	if((pCurrentLODStru->Off==pLODStartStru->Off)&&(pCurrentLODStru->Off==pLODEndStru->Off)&&
	   (pCurrentLODStru->Lnk==pLODStartStru->Lnk)&&(pCurrentLODStru->Lnk==pLODEndStru->Lnk))
	{
		*pLocInArea=1;
		chReturnValue = 1;
		return chReturnValue;
	}

	/*�ж�C���Ƿ���S�����E������Lnk�ϣ�������SE������*/

	if(pLODEndStru->Dir==EMAP_SAME_DIR)
	{	
		/*��ǰ����Ϊ������*/

		if ((pCurrentLODStru->Lnk==pLODStartStru->Lnk)&&(pCurrentLODStru->Lnk==pLODEndStru->Lnk))
		{
			/*S,C,ELink��ͬ*/
			if ((pCurrentLODStru->Off>=pLODStartStru->Off)&&(pCurrentLODStru->Off<pLODEndStru->Off))
			{
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}

		else if(pCurrentLODStru->Lnk==pLODStartStru->Lnk)
		{
			if (pCurrentLODStru->Off>=pLODStartStru->Off)
			{
				/*��ǰC��Lnk��S����ͬ�ҵ�ǰC��Off���ڵ���S��Off����������*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}

		}
		else if(pCurrentLODStru->Lnk==pLODEndStru->Lnk)
		{
			if (pCurrentLODStru->Off<pLODEndStru->Off)
			{
				/*��ǰC��Lnk��E����ͬ�ҵ�ǰC��OffС��E��of����������*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}

		}
		else
		{
			/*�����㣬C����S���E�㲻��ͬLnk*/
		}
	}
	else if(pLODEndStru->Dir==EMAP_CONVER_DIR)
	{
		/*��ǰ����Ϊ������*/

		if ((pCurrentLODStru->Lnk==pLODStartStru->Lnk)&&(pCurrentLODStru->Lnk==pLODEndStru->Lnk))
		{
			if ((pCurrentLODStru->Off<=pLODStartStru->Off)&&(pCurrentLODStru->Off>pLODEndStru->Off))
			{
				/*��ǰC��Lnk��S����ͬ����������*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}
		else if(pCurrentLODStru->Lnk==pLODStartStru->Lnk)
		{
			if (pCurrentLODStru->Off<=pLODStartStru->Off)
			{
				/*��ǰC��Lnk��S����ͬ����������*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}
		else if(pCurrentLODStru->Lnk==pLODEndStru->Lnk)
		{
			if (pCurrentLODStru->Off>pLODEndStru->Off)
			{
				/*��ǰC��Lnk��E����ͬ����������*/
				*pLocInArea=1;
				chReturnValue = 1;
				return chReturnValue;
			}
			else
			{
				*pLocInArea=0;
				chReturnValue = 1;
				return chReturnValue;
			}
		}
		else
		{

			/*�����㣬C����S���E�㲻��ͬ*/
		}
	}
	else
	{
		/*��ڲ����쳣����ѯʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*�ж�C�㲻��S�����E������Lnk�ϣ�ѭ������C��Lnk�Ƿ���SE�м�ĳһ��Lnk���*/
	for(i=0;i<wLinkNumber;i++)
	{
		if(pCurrentLODStru->Lnk==wLinkID[i])
		{
			/*��ǰC��Lnk��SE�����ڵ�ĳһ��LnkId��ͬ��������������*/
			*pLocInArea=1;
			chReturnValue = 1;
			return chReturnValue;
		}
	}

	/*��ǰC����SE������һ��Lnk������ͬ�����ز���������*/
	*pLocInArea=0;
	chReturnValue = 1;
	return chReturnValue;

}

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
UINT8 dsuCheckLocInAreaQuickExp2(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
	UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
	const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea)
{
	LOD_STRU TempLODStartStru;
	LOD_STRU TempLODEndStru;
	LOD_STRU TempCurrentLODStru;


	memcpy(&TempLODStartStru,pLODStartStru,sizeof(LOD_STRU));
	memcpy(&TempLODEndStru,pLODEndStru,sizeof(LOD_STRU));
	memcpy(&TempCurrentLODStru,pCurrentLODStru,sizeof(LOD_STRU));

	/*������LOD�ķ���ȡ��*/
	if (pLODEndStru->Dir==EMAP_SAME_DIR)
	{
		TempLODStartStru.Dir=EMAP_CONVER_DIR;
		TempLODEndStru.Dir=EMAP_CONVER_DIR;
		TempCurrentLODStru.Dir=EMAP_CONVER_DIR;
	}
	else
	{
		TempLODStartStru.Dir=EMAP_SAME_DIR;
		TempLODEndStru.Dir=EMAP_SAME_DIR;
		TempCurrentLODStru.Dir=EMAP_SAME_DIR;
	}

	return dsuCheckLocInAreaQuickExp(pDsuEmapStru,&TempLODStartStru,&TempLODEndStru,wLinkNumber,wLinkID,&TempCurrentLODStru,pLocInArea);
}

/*****************************����Ϊ�¶�ƽ�������������Ҫ�õ��ĺ���**************************/
/************************************************************************

 	�������ܣ�	�˺����Ĺ����ǲ�ѯ��ǰGarde������Grade
	��ڲ�����	const UINT16 wCurrentGradeID	��ǰ�¶ȱ��
				const UCHAR chDirection			�������з���
				const UCHAR PointNum			MA��Χ�ڵ�����Ŀ
				const DQU_POINT_STATUS_STRU * pPointStru	MA��Χ�ڵ�����Ϣ		
	���ڲ�����	��
	����ֵ��	UINT16 wFindGradeID	��ѯʧ��: 0xffff; ��ѯ�ɹ�: �������ڵ��¶�ID                                                              
************************************************************************/
/*���ܺ���,��ѯ��ǰGarde������Grade*/
UINT16 FindAdjacentGradeIDExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wCurrentGradeID, const UCHAR chDirection,
						   const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru)
{
	UINT16 wFindGradeID = 0;/*��������ֵ*/
	DSU_GRADE_STRU *pstGrade=NULL;/*ָ���¶���Ϣ�Ľṹ��ָ��*/
	const DQU_POINT_STATUS_STRU * pstPointStrut=NULL;/*�洢������Ϣ�ľֲ�����*/
	UINT16 wPointId = 0;/*�洢�¶ȹ���������ID*/
	UCHAR i = 0;/*ѭ���ñ���*/
	
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 GRADEINDEXNUM=0;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*������Ч�����ز�ѯʧ��*/
		wFindGradeID = 0xffff;
		return wFindGradeID;
	}

	/*�ж��������Ч��*/
	if((wCurrentGradeID>GRADEINDEXNUM)||(dsuGradeIndex[wCurrentGradeID] == 0xFFFF))
	{
		/*������Ч�����ز�ѯʧ��*/
		wFindGradeID = 0xffff;
		return wFindGradeID;
	}	

	/*�ж�������ݷ������Ч��*/
	if ((EMAP_SAME_DIR != chDirection) && (EMAP_CONVER_DIR != chDirection))
	{
		/*������Ч�����ز�ѯʧ��*/
		wFindGradeID = 0xffff;
		return wFindGradeID;
	}
	
	/*���������з�����link���߼�������ͬʱ����ǰ�¶ȵ�ĩ�������¶Ƚ��в�ѯ*/
	if (EMAP_SAME_DIR == chDirection)
	{

		/*���ù�����ֱ���ҵ���ǰ�¶ȵ���Ϣ*/
		pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wCurrentGradeID];

		if ((0xffff == pstGrade->wTmnlJointSideGradeId)&&(0xffff == pstGrade->wTmnlJointMainGradeId))
		{
			/*��ǰ�¶��Ѿ�û�������¶ȣ���������0*/
			wFindGradeID = 0xffff;
		     return wFindGradeID;
		}
		/*�ڵ�ǰ�¶ȵ�ĩ�����ڲ����¶�Ϊ��ʱ����ĩ�����������¶ȼ�Ϊ����*/
		if (0xffff == pstGrade->wTmnlJointSideGradeId)
		{
			/*��ѯ�ɹ������������Ϣ*/
			wFindGradeID = pstGrade->wTmnlJointMainGradeId;
			return wFindGradeID;
		}
		else
		{
			/*�ڵ�ǰ�¶ȵ�ĩ�����ڲ����¶Ȳ�Ϊ��ʱ��������ʵʱ�ĵ���״̬���ж������¶�*/

			/*���������Ϣ*/
			pstPointStrut = pPointStru;

			/*����������Ϣ*/				
			wPointId = pstGrade->wTmnlRelatePointId;

			for (i=0; i<PointNum; i++)
			{
				if (wPointId == pstPointStrut->PointId)
				{
					/*����ʧȥ��ʾ,�����쳣����ѯʧ�ܣ�����Ĭ��ֵ���˳�*/
					if (EMAP_POINT_STATUS_LOSE == pstPointStrut->PointStatus)
					{
						wFindGradeID = 0xffff;
						return wFindGradeID;
					}
					else
					{
						/*�����ڶ�λ,��ĩ�����������¶ȼ�Ϊ����*/
						if (EMAP_POINT_STATUS_MAIN == pstPointStrut->PointStatus)
						{
							wFindGradeID = pstGrade->wTmnlJointMainGradeId;
							return wFindGradeID;								
						}
						else
						{
							/*�����ڷ�λ,��ĩ�����ڲ����¶ȼ�Ϊ����*/
							wFindGradeID = pstGrade->wTmnlJointSideGradeId;
							return wFindGradeID;		
						}							
					}
				}
				pstPointStrut++;
			}

			/*û���ҵ���صĵ�����Ϣ,��ѯʧ��,����Ĭ��ֵ*/
			wFindGradeID = 0xffff;
			return wFindGradeID;
		}				
	}/*end of  if (SAMEDIR == chDirection)*/
	else
	{
		/*���������з�����link���߼������෴ʱ����ǰ�¶ȵ�ʼ�˶������¶Ƚ��в�ѯ*/



		/*���ù�����ֱ���ҵ���ǰ�¶ȵ���Ϣ*/
		pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wCurrentGradeID];

		if ((0xffff == pstGrade->wOrgnJointSideGradeId)&&(0xffff == pstGrade->wOrgnJointMainGradeId))
		{
			/*��ǰ�¶��Ѿ�û�������¶ȣ���������0*/
			wFindGradeID = 0xffff;
			return wFindGradeID;
		}
		/*�ڵ�ǰ�¶ȵ�ʼ�����ڲ����¶�Ϊ��ʱ����ʼ�����������¶ȼ�Ϊ����*/
		if (0xffff == pstGrade->wOrgnJointSideGradeId)
		{
			/*��ѯ�ɹ��������¶�ID*/
			wFindGradeID = pstGrade->wOrgnJointMainGradeId;
			return wFindGradeID;
		}
		else
		{
			/*�ڵ�ǰ�¶ȵ�ʼ�����ڲ����¶Ȳ�Ϊ��ʱ��������ʵʱ�ĵ���״̬���ж������¶�*/

			/*���������Ϣ*/
			pstPointStrut = pPointStru;

			/*����������Ϣ*/
			wPointId = pstGrade->wOrgnRelatePointId;

			for (i=0; i<PointNum; i++)
			{
				if (wPointId == pstPointStrut->PointId)
				{
					/*����ʧȥ��ʾ,�����쳣����ѯʧ�ܣ�����Ĭ��ֵ���˳�*/
					if (EMAP_POINT_STATUS_LOSE == pstPointStrut->PointStatus)
					{
						wFindGradeID = 0xffff;
						return wFindGradeID;
					}
					else
					{
						/*�����ڶ�λ,��ʼ�����������¶ȼ�Ϊ����*/
						if(EMAP_POINT_STATUS_MAIN == pstPointStrut->PointStatus)
						{
							wFindGradeID = pstGrade->wOrgnJointMainGradeId;
							return wFindGradeID;								
						}
						else
						{
							/*�����ڷ�λ,��ʼ�����ڲ����¶ȼ�Ϊ����*/
							wFindGradeID = pstGrade->wOrgnJointSideGradeId;
							return wFindGradeID;		
						}							
					}
				}
				pstPointStrut++;
			}

			/*û���ҵ���صĵ�����Ϣ,��ѯʧ��,����Ĭ��ֵ*/
			wFindGradeID = 0xffff;
			return wFindGradeID;
		}
	}
}
/*
*�������ܣ���ѯĳ�������¶�ID
*���������UINT16 linkId,UINT32 offset�������
*���������UINT16 GradeID�����¶�ID
*����ֵ��  0����ִ��ʧ�ܣ�1�ɹ�
*/
static UINT8 dsuGetGradeIdbyLocExp_DFT(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId, const UINT32 offset, UINT16 *GradeID)
{
	UINT16 i=0,j=0;							/*ѭ����������*/
	DSU_GRADE_STRU* curGrade=NULL;          		/*��ǰ�¶���ʱ����*/
	UINT8 bReturnValue;						/*��������ֵ����*/
	DSU_LINK_STRU *pLinkStru=NULL;				/*Link��Ϣ�ṹ��ָ��*/
	
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if ((NULL != pDsuEmapStru) && (NULL != GradeID))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	if((linkId>LINKINDEXNUM)||((dsuLinkIndex[linkId]) == 0xFFFF))
	{
		/*��ڲ����쳣����ѯʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*��ȡ��ǰλ�õ�link��Ӧ��λ����Ϣ��*/
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

	/*��ǰlink�ġ�Link�¶���Ϣ���ԡ�Ϊ��ͳһ�¶�*/
	if(0 == pLinkStru->wGrade)
	{
		for(i=0;i<dsuDataLenStru->wGradeLen;i++)
		{
			/*��ʼ����ǰ�¶�*/
			curGrade = dsuStaticHeadStru->pGradeStru + i;
			/*�����ʼlink������link��ͬ*/
			if(linkId == curGrade->wOrgnLinkId)
			{
				/*����ƫ����>=��ǰ�¶����ƫ����*/
				if((offset>=curGrade->dwOrgnLinkOfst) && ((offset - curGrade->dwOrgnLinkOfst)<= curGrade->dwGradeLength))
				{
					/*���뵱ǰ�¶�ID*/
					*GradeID = curGrade->wId;
					bReturnValue = 1;
					return bReturnValue;
				}
				else
				{
					/*��������ִ��*/
				}
			}
			/*����ն�link������link��ͬ*/
			else if(linkId == curGrade->wTmnlLinkId)
			{
				/*����ƫ����<=��ǰ�¶��յ�ƫ����*/
				if(offset <= curGrade->dwTmnlLinkOfst)
				{
					/*���뵱ǰ�¶�ID*/
					*GradeID = curGrade->wId;
					bReturnValue = 1;
					return bReturnValue;
				}
				else
				{
					/*��������ִ��*/
				}
			}
			/*���м�link��ͬ*/
			else
			{
				j=0;
				while(0xffff !=(curGrade->wIncludeLinkId[j]))
				{
					if(linkId == (curGrade->wIncludeLinkId[j]))
					{
						*GradeID = curGrade->wId;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						j++;
					}
				}
			}
		}
		/*�������¶ȣ�û�ҵ��õ�*/
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	/*��ǰlink�ġ�Link�¶���Ϣ���ԡ�Ϊͳһ�¶�*/
	{
		/*ͳһ�¶Ȳ���Ҫʹ���¶�ID��Ϣ,��Ϊ��Ч*/
		*GradeID = 0xffff;
		bReturnValue = 1;
		return bReturnValue;
	}
}

/*
*�������ܣ���ѯĳ�������¶�ID
*���������UINT16 linkId,UINT32 offset�������
*���������UINT16 GradeID�����¶�ID
*����ֵ��  0����ִ��ʧ�ܣ�1�ɹ�
*/
static UINT8 dsuGetGradeIdbyLocExp_CPK(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId, const UINT32 offset, UINT16 *GradeID)
{
	UINT16 i = 0;							/*ѭ����������*/
	UINT8 bReturnValue = 0;					/*��������ֵ����*/
	DSU_LINK_STRU *pLinkStru = NULL;		/*Link��Ϣ�ṹ��ָ��*/
	DSU_GRADE_STRU *pGradeStru = NULL;		/*�¶���Ϣ�ṹ��ָ��*/

	DSU_GRADE_LINKIDX_STRU *pTempGradeLinkIdxStru = NULL;
	UINT16 tempGradeID = 0;

	UINT16 LINKINDEXNUM = 0;
	UINT16 *dsuLinkIndex = NULL;
	UINT16 *dsuGradeIndex = NULL;			/*�¶���Ϣ��������*/
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru = NULL;

	if ((NULL != pDsuEmapStru) && (NULL != pDsuEmapStru->dsuEmapIndexStru) && (NULL != pDsuEmapStru->dsuStaticHeadStru) && (NULL != GradeID))
	{
		/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
		LINKINDEXNUM = pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex = pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex = pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru = pDsuEmapStru->dsuStaticHeadStru;

		if ((linkId <= LINKINDEXNUM) && (DSU_NULL_16 != dsuLinkIndex[linkId]))
		{
			/*��ȡ��ǰλ�õ�link��Ӧ��λ����Ϣ��*/
			pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

			if((offset >= 0) && (offset <= pLinkStru->dwLength))
			{
				pTempGradeLinkIdxStru = g_dsuEmapExtendStru->pDsuGradeInLinkIdx;
				pTempGradeLinkIdxStru = pTempGradeLinkIdxStru + pLinkStru->wId;

				if (0 != pLinkStru->wGrade)/*��ǰlink�ġ�Link�¶���Ϣ���ԡ�Ϊͳһ�¶�*/
				{
					if (0 == pTempGradeLinkIdxStru->wGradeCount)
					{
						*GradeID = DSU_NULL_16;/*ͳһ�¶Ȳ���Ҫʹ���¶�ID��Ϣ,��Ϊ��Ч*/
						bReturnValue = 1;
					}
					else
					{
						/*�����֧*/
						bReturnValue = 0;
					}			
				}
				else/*��ǰlink�ġ�Link�¶���Ϣ���ԡ�Ϊ��ͳһ�¶�*/
				{
					if (1 == pTempGradeLinkIdxStru->wGradeCount)
					{
						tempGradeID = pTempGradeLinkIdxStru->wGradeId[0];
						*GradeID = tempGradeID;
						bReturnValue = 1;
					} 
					else if (1 < pTempGradeLinkIdxStru->wGradeCount)
					{
						for (i = 0; i < pTempGradeLinkIdxStru->wGradeCount; i++)
						{
							tempGradeID = pTempGradeLinkIdxStru->wGradeId[i];

							/*��ȡ��ǰ�¶ȶ�Ӧ����Ϣ��*/
							pGradeStru = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[tempGradeID];

							/*��һ��link���ж���¶�ʱ�����ڸ�link�м�ĳ�ε��¶ȱز����м�link����ֻ���жϸ�link��ÿ���¶ȵ��ס�βlink������Ĳ���linkId�Ƿ���ͬ���ɡ�*/
							if ((linkId == pGradeStru->wOrgnLinkId) && (linkId == pGradeStru->wTmnlLinkId))/*һ��link��Ӧ����¶ȣ����¶�λ��link���м�ĳ��*/
							{
								if ((offset >= pGradeStru->dwOrgnLinkOfst) && ((offset - pGradeStru->dwOrgnLinkOfst) <= pGradeStru->dwGradeLength))
								{
									*GradeID = tempGradeID;
									bReturnValue = 1;
									break;
								} 
							} 
							else if ((linkId != pGradeStru->wOrgnLinkId) && (linkId == pGradeStru->wTmnlLinkId))/*һ��link��Ӧ����¶ȣ����¶�λ��link��ʼ��*/
							{
								if (offset <= pGradeStru->dwTmnlLinkOfst)
								{
									*GradeID = tempGradeID;
									bReturnValue = 1;
									break;
								} 								
							}
							else if ((linkId == pGradeStru->wOrgnLinkId) && (linkId != pGradeStru->wTmnlLinkId))/*һ��link��Ӧ����¶ȣ����¶�λ��link���ն�*/
							{
								if (offset >= pGradeStru->dwOrgnLinkOfst)
								{
									*GradeID = tempGradeID;
									bReturnValue = 1;
									break;
								} 								
							}
							else
							{
								/*�����֧*/
								bReturnValue = 0;
							}

						}
						/*link��û���ҵ���Ӧ���¶�*/
						if(i == pTempGradeLinkIdxStru->wGradeCount)
						{		   
						   bReturnValue = 0;
						}
					}
					else
					{
						/*�����֧*/
						bReturnValue = 0;
					}
				}
			}
			else
			{
				/*����ƫ�����������Ϸ�����ѯʧ��*/
				bReturnValue = 0;
			}			
		} 
		else
		{
			/*����link����쳣����ѯʧ��*/
			bReturnValue = 0;
		}				
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
	}	 
	return bReturnValue;
}

/*
*�������ܣ���ѯĳ�������¶�ID
*���������UINT16 linkId,UINT32 offset�������
*���������UINT16 GradeID�����¶�ID
*����ֵ��  0����ִ��ʧ�ܣ�1�ɹ�
*�޸�˵������CPK/HLT��ѯĳ�������¶�ID�㷨�������Ż�����ͬʹ��dsuGetGradeIdbyLocExp_CPK��
*		  FAO/DG����ʹ��ԭ���㷨��
*         modify by lmy 20171221
*/
UINT8 dsuGetGradeIdbyLocExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 linkId, const UINT32 offset, UINT16 *GradeID)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType = 0;
	cbtcVersionType = GetCbtcSysType();

	if((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType))
	{
		rtnValue = dsuGetGradeIdbyLocExp_CPK(pDsuEmapStru, linkId, offset, GradeID);
	} 
	else if((DQU_CBTC_FAO == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue = dsuGetGradeIdbyLocExp_DFT(pDsuEmapStru, linkId, offset, GradeID);
	}
	else
	{
		rtnValue = 0;		
	}
	return rtnValue; 
}

/*************
*�������ܣ�����ĳ���¶�ƽ������¶�ֵ
*���������Point �����¶ȵ�
*��������� 
*����ֵ��  0����ִ��ʧ�� 1 �����¶ȵ�һ�� 2 �����¶ȵ�2�� 3 �����¶ȵ�3��
****************/
UINT8 dsuCalculateGradeValueExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst,FLOAT32* GradeVal)
{
	UINT8  funcRtn;
	UINT16 gradeId;             /*�������¶�ID*/
	UINT8  bReturnValue = 0;    /*��������ֵ*/
	UINT32 arcEndOfst  = 0;     /*�¶ȵ�һ���ֽ���������λ��*/
	UINT32 arcStartOfst = 0;    /*�¶ȵڶ����ֽ���������λ��*/
	UINT32 relLocation = 0;     /*��ǰ���������λ��*/
	FLOAT64 angleValue = 0;     /*�Ƕȱ仯ֵ*/
	FLOAT64 tempAngleValue = 0;     /*���������ʱֵ*/
	DSU_GRADE_STRU* curGrade=NULL;        /*��ǰ�¶���ʱ����*/
	DSU_GRADE_STRU  curGradeTmp;        /*��ǰ�¶���ʱ����*/
	DSU_GRADE_STRU* frontGrade=NULL;        /*��ǰ�¶�ǰ�������¶���ʱ����*/
	DSU_GRADE_STRU* rearGrade=NULL;       /*��ǰ�¶Ⱥ������¶���ʱ����*/
	FLOAT32 curGradeVal;
	FLOAT64 tempValue;              /*��ʱ��������������ʽ����ֵ*/
	UINT8 GradeTrend = 0;
	UINT8 RtnGetGradFrnt = 0;
	UINT8 RtnGetGradNxt = 0;

	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*������������¶�*/
	funcRtn = dsuGetGradeIdbyLocExp(pDsuEmapStru,LinkId,ofst,&gradeId);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*������������¶����ֽ���������λ��*/
	funcRtn = dsuDividPointDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,&arcEndOfst,&arcStartOfst);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*������������¶����ֽ���������λ��*/
	funcRtn = dsuGetDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,ofst,&relLocation);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}

	/*Ϊ�˷�����㣬�Էֽ�����Ծ���������´���*/
	if(0xFFFFFFFF == arcEndOfst)
	{
		arcEndOfst = 0;
	}

	if(0xffff != gradeId)
	{
		/*��ʼ����ǰ�¶�*/
		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[gradeId];
		/*����Ϊ�����¶�ֵΪ��*/
		if(0x55 == curGrade->wInclineDir)
		{
			curGradeVal = (FLOAT32)(1.0*curGrade->wGradValue)/10000;
		}
		else
			/*���򷴣��¶�ֵΪ��*/
		{
			curGradeVal = (FLOAT32)(-1.0*curGrade->wGradValue)/10000;
		}

		/*��ȡǰ�������¶�*/
		if(0xffff != curGrade->wOrgnJointMainGradeId)
		{
			frontGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wOrgnJointMainGradeId];
		}
		else
		{
			/*��·�յ�����ͳһ�¶�,���ٽ����¶�ƽ��*/
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*���سɹ�*/
			bReturnValue = 2;
			return bReturnValue;
		}

		/*��ȡ���������¶�*/
		if(0xffff != curGrade->wTmnlJointMainGradeId)
		{
			rearGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wTmnlJointMainGradeId];
		}
		else
		{
			/*��·�յ�����ͳһ�¶�,���ٽ����¶�ƽ��*/
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*���سɹ�*/
			bReturnValue = 2;
			return bReturnValue;
		}
	}
	else
	{	
		/*��ȡ��ǰ��ͳһ�¶�ֵ,��Ϊ0���ٽ�������ͷ����ж�*/
		curGradeVal = dsuStaticHeadStru->pConGradeStru->wGrade;

		/*��ǰΪͳһ�¶�ʱ,Ϊ����������,�ڸô���ֵ*/
		curGradeTmp.wId = 0xFFFF;
		curGradeTmp.dwRadius = 0;
		curGradeTmp.wInclineDir = 0x55;
		curGradeTmp.wGradValue = curGradeVal;
		curGrade = &curGradeTmp;


		/*ʹ��ǰ�ĳ�ʼ��*/
		frontGrade = NULL;
		rearGrade = NULL;

		/*��ȡǰ�������¶�*/
		RtnGetGradFrnt = dsuGetGradeFrntExp(pDsuEmapStru,LinkId, &frontGrade);

		/*��ȡ���������¶�*/
		RtnGetGradNxt = dsuGetGradeRearExp(pDsuEmapStru,LinkId, &rearGrade);

		/*��ǰ�����������¶Ȳ�ѯʧ��*/
		if((0==RtnGetGradFrnt)||(0==RtnGetGradNxt))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}

		/*20141122��ӣ�����ѯǰ��ͺ����¶�ʱ,���ֵ�ǰ�¶�����·�յ�,���ٽ���ƽ��*/
		if((2==RtnGetGradFrnt)||(2==RtnGetGradNxt))
		{
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*���سɹ�*/
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*
	1 û�зֽ���ʱ�򣬼������¶��뵱ǰ�¶Ȳ�С��ǧ��֮20
	2 ��ǰ�㴦�ڷֽ������ʾ�Ļ�����
	*/
	if((relLocation >= arcEndOfst) && (relLocation <= arcStartOfst))
	{

		* GradeVal = (FLOAT32)(10000*curGradeVal);
		/*���سɹ�*/
		bReturnValue = 2;
		return bReturnValue;
	}
	/*��ǰ���ڵ�һ���ֽ�㻡����*/
	else if(relLocation < arcEndOfst)
	{
		if(NULL != frontGrade)
		{
			/*����Ƕȱ仯ֵ*/
			if(0 == frontGrade->dwRadius)
			{
				/*���ʰ뾶Ϊ0���Ƕȱ仯ֵΪ0*/
				angleValue = 0;
			}
			else
			{
				/*�Ƕȱ仯ֵ*/
				angleValue = (FLOAT32)((arcEndOfst + relLocation)/(1.0*frontGrade->dwRadius));
			}
			/*�¶������£�ȡ��ֵ*/
			if(0x55 == frontGrade->wInclineDir)
			{
				tempValue = (1.0*frontGrade->wGradValue)/10000;
			}
			/*����ȡ��ֵ*/
			else 
			{
				tempValue = (-1.0*frontGrade->wGradValue)/10000;
			}
			/*if�����жϽǶȱ仯ֵ������ֵ���Ǽ���ֵ*/
			if(dquGradeTrendExp(pDsuEmapStru,frontGrade->wId,curGrade->wId,&GradeTrend))
			{
				/*�¶ȼ���*/
				if(1 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue - angleValue;
				}
				/*�¶�����*/
				else if(2 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue + angleValue;
				}
				/*�¶Ȳ��仯*/
				else
				{
					tempAngleValue = tempValue ;
				}

				/*ת��ΪFLOAT32����*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*���سɹ�*/
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
				/*���ɴ�*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*ת��ΪFLOAT32����*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*���سɹ�*/
			bReturnValue = 1;
			return bReturnValue;
		}
	}
	/*�ڵ����ֽ����*/
	else
	{
		if(NULL != rearGrade)
		{
			/*����Ƕȱ仯ֵ*/
			if(0 == curGrade->dwRadius)
			{
				/*���ʰ뾶Ϊ0���Ƕȱ仯ֵΪ0*/
				angleValue = 0;
			}
			else
			{
				/*�Ƕȱ仯ֵ*/
				angleValue = (FLOAT32)((relLocation - arcStartOfst)/(1.0*curGrade->dwRadius));
			}

			if(0x55 == curGrade->wInclineDir)
			{
				tempValue = (1.0*curGrade->wGradValue)/10000;
			}
			else
			{
				tempValue = (-1.0*curGrade->wGradValue)/10000;
			}

			/*if�����жϽǶȱ仯ֵ������ֵ���Ǽ���ֵ*/
			if(dquGradeTrendExp(pDsuEmapStru,curGrade->wId,rearGrade->wId,&GradeTrend))
			{
				/*�¶�ֵ����*/
				if(1 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue - angleValue;				
				}
				/*�¶�ֵ����*/
				else if(2 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue + angleValue;
				}
				/*�¶�ֵ���仯*/
				else
				{
					tempAngleValue = tempValue;
				}

				/*ת��ΪFLOAT32����*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*���سɹ�*/
				bReturnValue = 3;
				return bReturnValue;
			}
			else
			{
				/*���ɴ�*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*ת��ΪFLOAT32����*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*���سɹ�*/
			bReturnValue = 3;
			return bReturnValue;
		}
	}	
}

UINT8 dsuCalculateGradeValueExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst,FLOAT32* GradeVal)
{
	UINT8  funcRtn;
	UINT16 gradeId;             /*�������¶�ID*/
	UINT8  bReturnValue = 0;    /*��������ֵ*/
	UINT32 arcEndOfst  = 0;     /*�¶ȵ�һ���ֽ���������λ��*/
	UINT32 arcStartOfst = 0;    /*�¶ȵڶ����ֽ���������λ��*/
	UINT32 relLocation = 0;     /*��ǰ���������λ��*/
	FLOAT64 angleValue = 0;     /*�Ƕȱ仯ֵ*/
    FLOAT64 tempAngleValue = 0;     /*���������ʱֵ*/
    DSU_GRADE_STRU* curGrade=NULL;        /*��ǰ�¶���ʱ����*/
    DSU_GRADE_STRU  curGradeTmp;        /*��ǰ�¶���ʱ����*/
	DSU_GRADE_STRU* frontGrade=NULL;        /*��ǰ�¶�ǰ�������¶���ʱ����*/
	DSU_GRADE_STRU* rearGrade=NULL;       /*��ǰ�¶Ⱥ������¶���ʱ����*/
	FLOAT32 curGradeVal;
	FLOAT64 tempValue;              /*��ʱ��������������ʽ����ֵ*/
	UINT8 GradeTrend = 0;
	UINT8 RtnGetGradFrnt = 0;
	UINT8 RtnGetGradNxt = 0;

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*������������¶�*/
	funcRtn = dsuGetGradeIdbyLocExp(pDsuEmapStru,LinkId,ofst,&gradeId);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*������������¶����ֽ���������λ��*/
	funcRtn = dsuDividPointDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,&arcEndOfst,&arcStartOfst);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*������������¶����ֽ���������λ��*/
	funcRtn = dsuGetDistanceFromOriginExp(pDsuEmapStru,gradeId,LinkId,ofst,&relLocation);
	if(0 == funcRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}

	/*Ϊ�˷�����㣬�Էֽ�����Ծ���������´���*/
	if(0xFFFFFFFF == arcEndOfst)
	{
		arcEndOfst = 0;
	}
	
	if(0xffff != gradeId)
	{
		/*��ʼ����ǰ�¶�*/
		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[gradeId];
		/*����Ϊ�����¶�ֵΪ��*/
		if(0x55 == curGrade->wInclineDir)
		{
			curGradeVal = (FLOAT32)(1.0*curGrade->wGradValue)/10000;
		}
		else
		/*���򷴣��¶�ֵΪ��*/
		{
			curGradeVal = (FLOAT32)(-1.0*curGrade->wGradValue)/10000;
		}
		
		/*��ȡǰ�������¶�*/
		if(0xffff != curGrade->wOrgnJointMainGradeId)
		{
			frontGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wOrgnJointMainGradeId];
		}
		/*else
		{
			*GradeVal = (FLOAT32)(10000*curGradeVal);
			bReturnValue = 2;
			return bReturnValue;
		}*/
		
		/*��ȡ���������¶�*/
		if(0xffff != curGrade->wTmnlJointMainGradeId)
		{
			rearGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wTmnlJointMainGradeId];
		}
		/*else
		{
			*GradeVal = (FLOAT32)(10000*curGradeVal);
			bReturnValue = 2;
			return bReturnValue;
		}*/
	}
	else
	{	
		/*��ȡ��ǰ��ͳһ�¶�ֵ,��Ϊ0���ٽ�������ͷ����ж�*/
		curGradeVal = dsuStaticHeadStru->pConGradeStru->wGrade;
		
		/*��ǰΪͳһ�¶�ʱ,Ϊ����������,�ڸô���ֵ*/
		curGradeTmp.wId = 0xFFFF;
		curGradeTmp.dwRadius = 0;
		curGradeTmp.wInclineDir = 0x55;
		curGradeTmp.wGradValue = (UINT16)curGradeVal;
		curGrade = &curGradeTmp;
		

		/*ʹ��ǰ�ĳ�ʼ��*/
		frontGrade = NULL;
		rearGrade = NULL;
			
		/*��ȡǰ�������¶�*/
		RtnGetGradFrnt = dsuGetGradeFrntExp(pDsuEmapStru,LinkId, &frontGrade);
		
		/*��ȡ���������¶�*/
		RtnGetGradNxt = dsuGetGradeRearExp(pDsuEmapStru,LinkId, &rearGrade);
		
		/*��ǰ�����������¶Ȳ�ѯʧ��*/
		if((0==RtnGetGradFrnt)||(0==RtnGetGradNxt))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}

		/*20141122��ӣ�����ѯǰ��ͺ����¶�ʱ,���ֵ�ǰ�¶�����·�յ�,���ٽ���ƽ��*/
		if((2==RtnGetGradFrnt)||(2==RtnGetGradNxt))
		{
			* GradeVal = (FLOAT32)(10000*curGradeVal);
			/*���سɹ�*/
			bReturnValue = 2;
			return bReturnValue;
		}
	}
	
	/*
	 1 û�зֽ���ʱ�򣬼������¶��뵱ǰ�¶Ȳ�С��ǧ��֮20
	 2 ��ǰ�㴦�ڷֽ������ʾ�Ļ�����
	*/
	if((relLocation >= arcEndOfst) && (relLocation <= arcStartOfst))
	{

		* GradeVal = (FLOAT32)(10000*curGradeVal);
		/*���سɹ�*/
	    bReturnValue = 2;
	    return bReturnValue;
	}
	/*��ǰ���ڵ�һ���ֽ�㻡����*/
	else if(relLocation < arcEndOfst)
	{
		if(NULL != frontGrade)
		{
			/*����Ƕȱ仯ֵ*/
			if(0 == frontGrade->dwRadius)
			{
			  /*���ʰ뾶Ϊ0���Ƕȱ仯ֵΪ0*/
			  angleValue = 0;
			}
			else
			{
				/*�Ƕȱ仯ֵ*/
			   angleValue = (FLOAT32)((arcEndOfst + relLocation)/(1.0*frontGrade->dwRadius));
			}
			/*�¶������£�ȡ��ֵ*/
			if(0x55 == frontGrade->wInclineDir)
			{
				tempValue = (1.0*frontGrade->wGradValue)/10000;
			}
			/*����ȡ��ֵ*/
			else 
			{
				tempValue = (-1.0*frontGrade->wGradValue)/10000;
			}
			/*if�����жϽǶȱ仯ֵ������ֵ���Ǽ���ֵ*/
			if(dquGradeTrendExp(pDsuEmapStru,frontGrade->wId,curGrade->wId,&GradeTrend))
			{
				/*�¶ȼ���*/
				if(1 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue - angleValue;
				}
				/*�¶�����*/
				else if(2 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue + angleValue;
				}
				/*�¶Ȳ��仯*/
				else
				{
					tempAngleValue = tempValue ;
				}

				/*ת��ΪFLOAT32����*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*���سɹ�*/
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
				/*���ɴ�*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*ת��ΪFLOAT32����*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*���سɹ�*/
			bReturnValue = 1;
			return bReturnValue;
		}
	}
	/*�ڵ����ֽ����*/
	else
	{
		if(NULL != rearGrade)
		{
			/*����Ƕȱ仯ֵ*/
			if(0 == curGrade->dwRadius)
			{
			  /*���ʰ뾶Ϊ0���Ƕȱ仯ֵΪ0*/
			  angleValue = 0;
			}
			else
			{
				/*�Ƕȱ仯ֵ*/
			   angleValue = (FLOAT32)((relLocation - arcStartOfst)/(1.0*curGrade->dwRadius));
			}

			if(0x55 == curGrade->wInclineDir)
			{
				tempValue = (1.0*curGrade->wGradValue)/10000;
			}
			else
			{
				tempValue = (-1.0*curGrade->wGradValue)/10000;
			}
			
			/*if�����жϽǶȱ仯ֵ������ֵ���Ǽ���ֵ*/
			if(dquGradeTrendExp(pDsuEmapStru,curGrade->wId,rearGrade->wId,&GradeTrend))
			{
				/*�¶�ֵ����*/
				if(1 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue - angleValue;				
				}
				/*�¶�ֵ����*/
				else if(2 == GradeTrend)
				{
					/*����ƽ������¶�ֵ*/
					tempAngleValue = tempValue + angleValue;
				}
				/*�¶�ֵ���仯*/
				else
				{
					tempAngleValue = tempValue;
				}
			
				/*ת��ΪFLOAT32����*/
				*  GradeVal = (FLOAT32)(tempAngleValue*10000);
				/*���سɹ�*/
				bReturnValue = 3;
				return bReturnValue;
			}
			else
			{
				/*���ɴ�*/
				bReturnValue = 0;
				return bReturnValue;
			}
		}
		else
		{
			/*ת��ΪFLOAT32����*/
			*  GradeVal = (FLOAT32)(curGradeVal);
			/*���سɹ�*/
			bReturnValue = 3;
			return bReturnValue;
		}
	}	
}

/*************
*���ʽӿ�
*�������ܣ�����ĳ���¶�ƽ������¶�ֵ
*���������Point �����¶ȵ�
*��������� 
*����ֵ��  0����ִ��ʧ�� 1 �����¶ȵ�һ�� 2 �����¶ȵ�2�� 3 �����¶ȵ�3��
*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726
****************/
UINT8 dsuCalculateGradeValueExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst,FLOAT32 *GradeVal)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue=dsuCalculateGradeValueExp_FAO(pDsuEmapStru,LinkId,ofst, GradeVal);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType)  )
	{
		rtnValue=dsuCalculateGradeValueExp_CPK(pDsuEmapStru,LinkId,ofst, GradeVal);
	}
	else
	{
		rtnValue = 0;		
	}
	return rtnValue; 
}

/*************
*�������ܣ�����ĳ�¶�ƽ���󣬷ֳɵļ������ֵķֽ�������ľ���
*���������gradeId �¶�ID
*���������UINT32* endPointDistance UINT32* startPointDistance
*����ֵ��  0����ִ��ʧ��   1�ɹ�
��ע��     �������Ҫƽ������ĵ㣬��Ϊ��Чֵffffffff  ����ֵΪ1
****************/
UINT8 dsuDividPointDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32* endPointDistance,UINT32* startPointDistance)
{
	DSU_GRADE_STRU* curGrade=NULL;        /*��ǰ�¶���ʱ����*/
	DSU_GRADE_STRU* frontGrade=NULL;      /*ǰ���¶�*/
	DSU_GRADE_STRU* rearGrade=NULL;       /*�������¶�*/
	UINT16 gradeValue = 0;           /*�¶Ȳ�ֵ*/
	UINT32 arcLength = 0;			 /*����*/
	UINT8 bReturnValue = 0;			 /*��������ֵ*/
	UINT8 RtnGetGradFrnt = 0;		 /*��ȡǰ���¶Ƚ��*/
	UINT8 RtnGetGradNxt = 0;		 /*��ȡ�����¶Ƚ��*/

	DSU_GRADE_STRU curGradeTmp;		/*��ǰ�¶���ʱ����*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 GRADEINDEXNUM=0;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*��������¶ȱ�Ų�Ϊ��,�����¶ȱ��д���*/
	if(0xffff != GradeId)
	{
		if((GradeId>GRADEINDEXNUM)||((dsuGradeIndex[GradeId]) == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}
		
		/*��ʼ����ǰ�¶�*/
		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeId];
		
		/*ǰ���������¶�*/
		if(curGrade->wOrgnJointMainGradeId != 0xFFFF)
		{
			/*��ʼ��ǰ�������¶�*/
			frontGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wOrgnJointMainGradeId];
		}
		/*ǰ��û�������¶�*/
		else
		{
			/*�յ���Ϊ��Чֵ*/
			* endPointDistance = 0xFFFFFFFF;
			frontGrade = NULL;
		}
		
		/*�����������¶�*/
		if(curGrade->wTmnlJointMainGradeId != 0xFFFF)
		{
			/*��ʼ�����������¶�*/
			rearGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[curGrade->wTmnlJointMainGradeId];
		}
		/*����û�������¶�*/
		else
		{
			/*�����Ϊ��Чֵ*/
			* startPointDistance = 0xFFFFFFFF;
			rearGrade = NULL;
		}
	}
	else
	/*��ǰ�����¶�Ϊͳһ�¶�*/
	{
		memset(&curGradeTmp, 0, sizeof(curGradeTmp));

		/*������Ϣ,��д��ǰ�¶���Ϣ*/
		curGradeTmp.wInclineDir = EMAP_SAME_DIR;
		curGradeTmp.wGradValue =  dsuStaticHeadStru->pConGradeStru->wGrade;
		curGradeTmp.dwRadius = 0;

		curGrade = &curGradeTmp;

		/*ʹ��ǰ�ĳ�ʼ��*/
		frontGrade = NULL;
		rearGrade = NULL;
		
		/*��ȡǰ�������¶�*/
		RtnGetGradFrnt = dsuGetGradeFrntExp(pDsuEmapStru,linkId, &frontGrade);
		
		/*��ȡ���������¶�*/
		RtnGetGradNxt = dsuGetGradeRearExp(pDsuEmapStru,linkId, &rearGrade);
		
		/*��ǰ�����������¶Ȳ�ѯʧ��*/
		if((0==RtnGetGradFrnt)||(0==RtnGetGradNxt))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
		
	/**************����linkEndId ��  endOfst******************/
	/*ǰ���������¶�*/
	if(NULL != frontGrade)
	{
		/*�����߰뾶��Ч*/
		if(frontGrade->dwRadius == 0xFFFFFFFF)
		{
			/*�յ���Ϊ��Чֵ*/
			* endPointDistance = 0xFFFFFFFF;
		}
		else
		{
			/*�����¶Ȳ�ֵ���������»�����*/
			if((frontGrade->wInclineDir) == (curGrade->wInclineDir))
			{
				gradeValue = abs((INT16)curGrade->wGradValue - (INT16)frontGrade->wGradValue);
			}
			else
			{
				/*һ�����£�һ������*/
				gradeValue = curGrade->wGradValue + frontGrade->wGradValue;
			}
			/*���㻡��*/
			arcLength = gradeValue*((frontGrade->dwRadius)/20000);
			* endPointDistance = arcLength;
		}
	}
	else
	{
		/*�յ���Ϊ��Чֵ*/
		* endPointDistance = 0xFFFFFFFF;
	}
	/**************�������linkEndId ��  endOfst******************/
		

	/**************����linkStartId ��  startOfst******************/
	/*�����������¶�*/
	if(NULL != rearGrade)
	{
		/*�����߰뾶��Ч*/
		if(rearGrade->dwRadius == 0xFFFFFFFF)
		{
			/*�յ���Ϊ��Чֵ*/
			* startPointDistance = 0xFFFFFFFF;
		}
		else
		{
			/*�����¶Ȳ�ֵ���������»�����*/
			if((rearGrade->wInclineDir) == (curGrade->wInclineDir))
			{
				gradeValue = abs((INT16)curGrade->wGradValue - (INT16)rearGrade->wGradValue);
			}
			else
			{
				/*һ�����£�һ������*/
				gradeValue = curGrade->wGradValue + rearGrade->wGradValue;
			}
			/*���㻡��*/
			arcLength = gradeValue*((curGrade->dwRadius)/20000);
			* startPointDistance = curGrade->dwGradeLength - arcLength;
		}
	}
	else
	{
		/*�����Ϊ��Чֵ*/
		* startPointDistance = 0xFFFFFFFF;
	}
	/**************�������linkStartId ��  startOfst******************/

	bReturnValue = 1;
	return bReturnValue;
}

/*************
*�������ܣ������¶���ĳһ�㼰�¶�id������õ�����¶����ľ���
*���������gradeId �¶�ID
           UINT16 linkId UINT32 Ofst
*���������UINT32 *length  ����
          
*����ֵ��  0����ִ��ʧ��   1�ɹ�  
****************/
UINT8 dsuGetDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32 Ofst,UINT32* length)
{
	UINT16 j=0;                 /*ѭ������*/
	DSU_GRADE_STRU* curGrade=NULL;   /*�¶Ƚṹ����ʱ����*/
	DSU_LINK_STRU * plink=NULL;      /*link�ṹ��ָ����ʱ����*/
	UINT8 bReturnValue;         /*��������ֵ*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 GRADEINDEXNUM=0;
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	if(0xffff != GradeId)
	{
		if((GradeId>GRADEINDEXNUM)||((dsuGradeIndex[GradeId]) == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}

		curGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeId];
		if(0 == curGrade->dwOrgnLinkOfst)
		{
			* length = 0;
		}
		else
		{
			* length = 1;
		}
		/*������ʼlink��*/
		if((linkId == curGrade->wOrgnLinkId) && (Ofst >= curGrade->dwOrgnLinkOfst))
		{
			* length += Ofst - curGrade->dwOrgnLinkOfst;
			bReturnValue = 1;
			 return bReturnValue;
		}
		else
		{
			plink = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[curGrade->wOrgnLinkId];
			* length += plink->dwLength - curGrade->dwOrgnLinkOfst;
			j=0;
			/*�ȴ��м�link������*/
			while(0xFFFF != curGrade->wIncludeLinkId[j])
			{
				if(j>=GRADE_INCLUDE_LINK_NUM)
				{
					/*���ݴ����˳� ����������ô�����link*/
					bReturnValue = 0;
					return bReturnValue;
				}
				plink = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[curGrade->wIncludeLinkId[j]];
				/*�����м�ĳlink��*/
				if((linkId == curGrade->wIncludeLinkId[j]) && (Ofst <= plink->dwLength))
				{
					* length += Ofst;
					bReturnValue = 1;
					return bReturnValue;
				}
				else
				{ 
					/*������м�link�ĳ����ۼ�����*/
					* length += plink->dwLength;
					j++;
				}
			}
			/*�м�linkû�ҵ�����ĩ��link*/
			if((linkId == curGrade->wTmnlLinkId) && (Ofst <= curGrade->dwTmnlLinkOfst))
			{
				* length += Ofst;
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
				bReturnValue = 0;
				return bReturnValue;
			}
		}
	}
	else
	{
		* length = Ofst;
		bReturnValue = 1;
		return bReturnValue;	
	}
}

/************************************************************************

 	�������ܣ�	�˺����Ĺ����ǲ�ѯ��������λ�ü������¶ȡ�������ѯ�����
				�յ㷶Χ�����е���·�¶ȣ�����������¶ȣ�������£����涨
				����Ϊ��������Ϊ���������Ĺ��ܼ�Ϊ������Χ���¶ȵ���Сֵ��
				������������¶ȵ�ǧ������

	��ڲ�����	const LOD_STRU * pLODStartStru		��ѯ���λ�úͷ���ʵ��Ϊ�г���βλ�úͷ���
				const LOD_STRU * pLODEndStru		��ѯ�յ�λ�úͷ���
				const UCHAR PointNum				MA�еĵ�������
				const POINT_STRU * pPointStru		������Ϣ�ṹ��
	���ڲ�����	FLOAT64 * Gradient					�г���β��MA�յ㷶Χ������¶ȵ�ǧ����������Ϊ��������Ϊ����
	����ֵ��	UCHAR bReturnValue					 	
				0����ѯʧ�ܣ�
 				1����ѯ��ȷ��
     ע���ڼ���Ĺ����У��õ����ֵ������ֵǰ����/10�Ĳ�������˷���
	     ���¶�ֵΪǧ��ֵ                                                     
************************************************************************/
/*14 ��λ�ü�����¶Ȳ�ѯ*/
static UCHAR dsuGradientGetExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient)
{
	UINT16 wCurrentGradeID = 0;/*��ǰ���¶ȱ��*/
	UINT16 wNextGradeID = 0;/*��ǰ�¶ȵ������¶ȱ��*/
	UCHAR bReturnValue = 0;/*��������ֵ*/
	UINT8 bLocGradeReturn = 0;/*��������ȡ��ǰλ�õ��¶���Ϣ������ֵ*/
	UCHAR rstGradeNxt = 0;  /*��ȡ����link�¶ȵķ���ֵ*/
	UCHAR rstGetIdNxt = 0;	/*��ȡ����link�ķ���ֵ*/
	DSU_GRADE_STRU *pstGrade=NULL;/*ָ���¶���Ϣ�Ľṹ��ָ��*/
	DSU_GRADE_STRU stGrade; /*ָ���¶���Ϣ�Ľṹ�����*/
	UINT16 i=0;  
	UINT16 wCurrentLinkId;	/*��ǰ�¶ȵ�link��ID*/

	FLOAT32 StartPointGradeVal;
	FLOAT32 EndPointGradeVal;
	UINT16 StartGradeId;
	UINT16 EndGradeId;
	UINT8  StartPointGradeRtn;
	UINT8  EndPointGradeRtn;
	FLOAT32 Coefficient = 1;          /*�¶�ϵ��������Ϊ1 ����Ϊ-1*/
	FLOAT32 CurGradeVal;
	FLOAT32 minGradeVal;
	FLOAT32 maxGradeVal;

	UINT16  StartLinkId;
	UINT32  StartOfst;
	UINT16  EndLinkId;
	UINT32  EndOfst;

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16	 LINKINDEXNUM=0;
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж�����������Link����Ч��*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж�����������linkƫ��������Ч��*/
	if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж����������㷽�����Ч��*/
	if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж��������Link����Ч��*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж���������յ�linkƫ��������Ч��*/
	if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж���������յ㷽�����Ч��*/
	if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}


	/*����Ϊ����ϵ��ȡ1*/
	if(0x55 == pLODStartStru->Dir)
	{
		Coefficient = 1;
		StartLinkId = pLODStartStru->Lnk;
		StartOfst = pLODStartStru->Off;
		EndLinkId = pLODEndStru->Lnk;
		EndOfst = pLODEndStru->Off;
	}
	/*����Ϊ����ϵ��ȡ-1 ����յ����λ��*/
	else 
	{
		Coefficient = -1;
		StartLinkId = pLODEndStru->Lnk;
		StartOfst = pLODEndStru->Off;
		EndLinkId = pLODStartStru->Lnk;
		EndOfst = pLODStartStru->Off;
	}

	/*****************���¼��������յ��¶�ֵ*********************/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,StartLinkId,StartOfst,&StartGradeId);
	/*��������ʧ�ܣ����ش���*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	StartPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,StartLinkId,StartOfst,&StartPointGradeVal);
	/*��������ʧ�ܣ����ش���*/
	if (0 == StartPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�յ��¶�ֵ����*/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,EndLinkId,EndOfst,&EndGradeId);
	/*��������ʧ�ܣ����ش���*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	EndPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,EndLinkId,EndOfst,&EndPointGradeVal);
	/*��������ʧ�ܣ����ش���*/
	if (0 == EndPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*****************�����յ��¶�ֵ�������*********************/


	/****���������ͬһ�¶���****/
	if((StartGradeId == EndGradeId)&&
		(0xffff != StartGradeId)&&(0xffff != EndGradeId))
	{
		/*��ͬһ���ֽ����*/
		if(StartPointGradeRtn == EndPointGradeRtn)
		{
			/*������С���Ǹ�*/
			if(EndPointGradeVal < StartPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = EndPointGradeVal;
			}
		}
		/*����ͬһ�ֽ���ڣ��迼���м�ֽ���¶�ֵ*/
		else
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*��ǰ�¶�ֵ������Ϊ��������Ϊ��*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}

			/*ȡ����Сֵ*/
			if((EndPointGradeVal<=CurGradeVal)&& (EndPointGradeVal<=StartPointGradeVal))
			{
				minGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal<=CurGradeVal)&& (StartPointGradeVal<=EndPointGradeVal))
			{
				minGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = CurGradeVal;
			}
			/*ȡ���ֵ*/
			if((EndPointGradeVal>=CurGradeVal)&& (EndPointGradeVal>=StartPointGradeVal))
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal>=CurGradeVal)&& (StartPointGradeVal>=EndPointGradeVal))
			{
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				maxGradeVal = CurGradeVal;
			}

		}
	}
	/*****����ͬһ�¶�*****/
	else
	{
		if(0xffff != StartGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*��ǰ�¶�ֵ������Ϊ��������Ϊ��*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*ȡ�õ�ǰ�¶ȶ����ǰ���������С�¶�ֵ*/
			if(1 == StartPointGradeRtn)
			{
				if(CurGradeVal>StartPointGradeVal)
				{
					minGradeVal = StartPointGradeVal;
					maxGradeVal = CurGradeVal;
				}
				else
				{
					minGradeVal = CurGradeVal;
					maxGradeVal = StartPointGradeVal;
				}
			}
			else if(2 == StartPointGradeRtn)
			{
				minGradeVal = CurGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
		}
		else
		{
			/*��ʼ�¶�Ϊͳһ�¶�,���������Сֵ���г�ʼ��ֵ*/
			minGradeVal = StartPointGradeVal;
			maxGradeVal = StartPointGradeVal;
		}

		if(0xffff != EndGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[EndGradeId];
			/*�ն������¶�ֵ������Ϊ��������Ϊ��*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*ȡ�õ�ǰ�¶ȶ����ǰ���������С�¶�ֵ*/
			if(1 == EndPointGradeRtn)
			{
				/*��ĩ�˵����ҵ���С����¶�ֵ*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*����ִ��*/
				}
			}
			else if(2 == EndPointGradeRtn)
			{
				/*��ĩ�˵����ҵ���С����¶�ֵ*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*����ִ��*/
				}
			}
			else
			{
				/*������ֵ*/
				if((EndPointGradeVal>= maxGradeVal)&&(EndPointGradeVal>= CurGradeVal))
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal>= maxGradeVal)&&(CurGradeVal>= EndPointGradeVal))
				{
					maxGradeVal = CurGradeVal;
				}
				else
				{
					/*����ִ��*/
				}

				/*�����Сֵ*/
				if((EndPointGradeVal<= minGradeVal)&&(EndPointGradeVal<= CurGradeVal))
				{
					minGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal<= minGradeVal)&&(CurGradeVal<=EndPointGradeVal ))
				{
					minGradeVal = CurGradeVal;
				}
				else
				{
					/*����ִ��*/
				}
			}
		}
		else
		{
			/*�յ��¶�Ϊͳһ�¶�*/
			/*��ĩ�˵����ҵ���С����¶�ֵ*/
			if(maxGradeVal<EndPointGradeVal)
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if(minGradeVal>EndPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
			}
			else
			{
				/*����ִ��*/
			}
		}


		/*�赱ǰ�¶�idΪ��ʼ�¶�id*/
		wCurrentGradeID = StartGradeId;
		/*��¼��ǰlink��id��Ϣ*/
		wCurrentLinkId = StartLinkId;

		/*ѭ����ȡ����յ����¶���Ϣ���ȶԵõ����,��Сֵ*/
		for(i=0;i<dsuDataLenStru->wGradeLen;i++)
		{
			if(0xffff != wCurrentGradeID)
			{
				/*���¶ȱ��в�ѯ��ǰ�¶ȵ�ǰ���¶�*/
				wNextGradeID = FindAdjacentGradeIDExp(pDsuEmapStru,wCurrentGradeID, 0x55, PointNum, pPointStru);

				/*��ȡǰ���¶�*/
				if (0xffff != wNextGradeID)
				{
					/*����ѯ�õ����¶ȹ���ID��Ϊ��,���ù������ҵ���ǰ���¶���Ϣ*/
					pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wNextGradeID];	
					/*��ȡ��ǰ�����¶ȵ��յ�link��Ϣ*/
					wCurrentLinkId = pstGrade->wTmnlLinkId;
				}				
				else
				{
					/*�����¶ȱ��л�ȡ�����յ�����¶�Ϊ��Ч,�����Ϊ��·�յ�,������ͳһ�¶ȱ��е���Ϣ*/



					/*��link���в�ѯǰ���¶��Ƿ���ͳһ�¶ȱ���,*/
					rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);

					if((1 == rstGradeNxt)&&(NULL == pstGrade))
					{
						/*��ǰ���¶���ͳһ�¶ȱ���,���ȡͳһ�¶ȱ���Ϣ,�����¶ȱȽ�*/
						stGrade.wInclineDir = EMAP_SAME_DIR;
						stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
						pstGrade = &stGrade;

						/*�����¶�Ϊͳһ�¶�,�����Ϊ��Ч*/
						wNextGradeID = 0xffff;
					}
					else
					{
						/*��ǰ���¶�δ��ͳһ�¶ȱ���,��ǰ��ѯӦΪ��·�յ�*/
						/*��·�յ�(�ҵ���·�յ�Ҳδ���ҵ��յ��¶�ID)*/
						bReturnValue = 0;
						return bReturnValue;
					}
					/*���µ�ǰlink��ID��Ϣ�Ա�ͳһ�¶ȱ��ϵ�Ĳ�ѯ*/
					rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

					if(1 != rstGetIdNxt)
					{
						/*��ѯ��·linkʧ��*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
			}
			else
			{
				/*��ѯ��ǰlink��ʼ�������¶�*/
				rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);

				if(1 == rstGradeNxt)
				{
					if(NULL == pstGrade)
					{
						/*��ǰ���¶���ͳһ�¶ȱ���,���ȡͳһ�¶ȱ���Ϣ,�����¶ȱȽ�*/
						stGrade.wInclineDir = EMAP_SAME_DIR;
						stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
						pstGrade = &stGrade;

						/*�����¶�Ϊͳһ�¶�,�����Ϊ��Ч*/
						wNextGradeID = 0xffff;
					}
					else
					{
						/*����ʵ�ʲ�ѯ�õ�pstGrade���¶ȱ��е��¶�ֵ���д���*/
						/*��¼��ǰlink�������¶�ID*/
						wNextGradeID = pstGrade->wId;
					}

					/*Ϊ�������¶ȱ��еĲ�ѯ����ȫ,����ͳһ�¶ȱ��еĴ�������ѭ��*/
					if(i>=1)
					{
						i--;
					}
				}
				else
				{
					/*��·�յ�(�ҵ���·�յ�Ҳδ���ҵ��յ��¶�ID)*/
					bReturnValue = 0;
					return bReturnValue;
				}

				if (wCurrentLinkId == EndLinkId)
				{
					/*�鵽�ն��¶�*/
					break;
				}

				/*20141122��ӣ����µ�ǰlink��ID��Ϣ�Ա�ͳһ�¶ȱ��ϵ�Ĳ�ѯ*/
				rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

				if(1 != rstGetIdNxt)
				{
					/*��ѯ��·linkʧ��*/
					bReturnValue = 0;
					return bReturnValue;
				}
			}

			/*���µ�ǰ�¶�ID��Ϣ�Ա��¶ȱ��ϵ�Ĳ�ѯ*/
			wCurrentGradeID = wNextGradeID;

			if( ((wNextGradeID == EndGradeId) && ((0xffff != wNextGradeID) && (0xffff != EndGradeId))) ||
				((wCurrentLinkId == EndLinkId) && (0xffff == wNextGradeID)) )
			{
				/*�鵽�ն��¶�*/
				break;
			}

			if(0x55 == pstGrade->wInclineDir)
			{
				/*����и�����¶Ȼ��С���¶ȣ��滻��ǰֵ*/
				if(minGradeVal>=pstGrade->wGradValue)
				{
					minGradeVal = pstGrade->wGradValue;
				}
				if(maxGradeVal<=pstGrade->wGradValue)
				{
					maxGradeVal = pstGrade->wGradValue;
				}
			}
			else
			{
				/*����и�����¶Ȼ��С���¶ȣ��滻��ǰֵ*/
				if(minGradeVal>=-1.0*pstGrade->wGradValue)
				{
					minGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
				}
				if(maxGradeVal<=-1.0*pstGrade->wGradValue)
				{
					maxGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
				}
			}
		}

		if(i>=dsuDataLenStru->wGradeLen)
		{
			/*û�ҵ�ĩ���¶�*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}

	/*�������з���ȷ������¶�ֵ*/
	if(0x55 == pLODStartStru->Dir)
	{
		* Gradient = minGradeVal/10;
		bReturnValue = 1;
		return bReturnValue;
	}
	else
	{
		* Gradient = (FLOAT32)(-1.0 * maxGradeVal/10);
		bReturnValue = 1;
		return bReturnValue;
	}

}

static UCHAR dsuGradientGetExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient)
{
	UINT16 wCurrentGradeID = 0;/*��ǰ���¶ȱ��*/
	UINT16 wNextGradeID = 0;/*��ǰ�¶ȵ������¶ȱ��*/
	UCHAR bReturnValue = 0;/*��������ֵ*/
	UINT8 bLocGradeReturn = 0;/*��������ȡ��ǰλ�õ��¶���Ϣ������ֵ*/
	UCHAR rstGradeNxt = 0;  /*��ȡ����link�¶ȵķ���ֵ*/
	UCHAR rstGetIdNxt = 0;	/*��ȡ����link�ķ���ֵ*/
    DSU_GRADE_STRU *pstGrade=NULL;/*ָ���¶���Ϣ�Ľṹ��ָ��*/
	DSU_GRADE_STRU stGrade; /*ָ���¶���Ϣ�Ľṹ�����*/
	UINT16 i=0;  
	UINT16 wCurrentLinkId;	/*��ǰ�¶ȵ�link��ID*/

	FLOAT32 StartPointGradeVal;
	FLOAT32 EndPointGradeVal;
	UINT16 StartGradeId;
	UINT16 EndGradeId;
	UINT8  StartPointGradeRtn;
	UINT8  EndPointGradeRtn;
	FLOAT32 Coefficient = 1;          /*�¶�ϵ��������Ϊ1 ����Ϊ-1*/
	FLOAT32 CurGradeVal;
	FLOAT32 minGradeVal;
	FLOAT32 maxGradeVal;

	UINT16  StartLinkId;
	UINT32  StartOfst;
	UINT16  EndLinkId;
	UINT32  EndOfst;

	DSU_LINK_STRU *pLinkStru=NULL;				/*Link��Ϣ�ṹ��ָ��*/
    DSU_LINK_STRU *pLinkStruEnd=NULL;		   /*Link��Ϣ�ṹ��ָ�� add by zlj 20161024*/
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16	 LINKINDEXNUM=0;
	UINT16	*dsuLinkIndex=NULL;
	UINT16	*dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж�����������Link����Ч��*/
	if((pLODStartStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODStartStru->Lnk] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж�����������linkƫ��������Ч��*/
	if (pLODStartStru->Off > LinkLengthExp(pDsuEmapStru,pLODStartStru->Lnk))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж����������㷽�����Ч��*/
	if ((EMAP_SAME_DIR != pLODStartStru->Dir) && (EMAP_CONVER_DIR != pLODStartStru->Dir))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж��������Link����Ч��*/
	if((pLODEndStru->Lnk>LINKINDEXNUM)||(dsuLinkIndex[pLODEndStru->Lnk] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж���������յ�linkƫ��������Ч��*/
	if (pLODEndStru->Off > LinkLengthExp(pDsuEmapStru,pLODEndStru->Lnk))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�ж���������յ㷽�����Ч��*/
	if ((EMAP_SAME_DIR != pLODEndStru->Dir) && (EMAP_CONVER_DIR != pLODEndStru->Dir))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}


	/*����Ϊ����ϵ��ȡ1*/
	if(0x55 == pLODStartStru->Dir)
	{
		Coefficient = 1;
		StartLinkId = pLODStartStru->Lnk;
		StartOfst = pLODStartStru->Off;
		EndLinkId = pLODEndStru->Lnk;
		EndOfst = pLODEndStru->Off;
	}
	/*����Ϊ����ϵ��ȡ-1 ����յ����λ��*/
	else 
	{
		Coefficient = -1;
		StartLinkId = pLODEndStru->Lnk;
		StartOfst = pLODEndStru->Off;
		EndLinkId = pLODStartStru->Lnk;
		EndOfst = pLODStartStru->Off;
	}

	/*****************���¼��������յ��¶�ֵ*********************/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,StartLinkId,StartOfst,&StartGradeId);
	/*��������ʧ�ܣ����ش���*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	StartPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,StartLinkId,StartOfst,&StartPointGradeVal);
	/*��������ʧ�ܣ����ش���*/
	if (0 == StartPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*�յ��¶�ֵ����*/
	bLocGradeReturn = dsuGetGradeIdbyLocExp(pDsuEmapStru,EndLinkId,EndOfst,&EndGradeId);
	/*��������ʧ�ܣ����ش���*/
	if (0 == bLocGradeReturn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	EndPointGradeRtn =  dsuCalculateGradeValueExp(pDsuEmapStru,EndLinkId,EndOfst,&EndPointGradeVal);
	/*��������ʧ�ܣ����ش���*/
	if (0 == EndPointGradeRtn)
	{
		bReturnValue = 0;
		return bReturnValue;
	}
	/*****************�����յ��¶�ֵ�������*********************/

	
	/****���������ͬһ�¶���****/
	if((StartGradeId == EndGradeId)&&
	   (0xffff != StartGradeId) && (0xffff != EndGradeId))
	{
		/*��ͬһ���ֽ����*/
		if(StartPointGradeRtn == EndPointGradeRtn)
		{
			/*������С���Ǹ�*/
			if(EndPointGradeVal < StartPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = EndPointGradeVal;
			}
		}
		/*����ͬһ�ֽ���ڣ��迼���м�ֽ���¶�ֵ*/
		else
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*��ǰ�¶�ֵ������Ϊ��������Ϊ��*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}

			/*ȡ����Сֵ*/
			if((EndPointGradeVal<=CurGradeVal)&& (EndPointGradeVal<=StartPointGradeVal))
			{
				minGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal<=CurGradeVal)&& (StartPointGradeVal<=EndPointGradeVal))
			{
				minGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = CurGradeVal;
			}
			/*ȡ���ֵ*/
			if((EndPointGradeVal>=CurGradeVal)&& (EndPointGradeVal>=StartPointGradeVal))
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if((StartPointGradeVal>=CurGradeVal)&& (StartPointGradeVal>=EndPointGradeVal))
			{
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				maxGradeVal = CurGradeVal;
			}

		}
	}
    /*****����ͬһ�¶�*****/
	else
	{
		if(0xffff != StartGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[StartGradeId];
			/*��ǰ�¶�ֵ������Ϊ��������Ϊ��*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*ȡ�õ�ǰ�¶ȶ����ǰ���������С�¶�ֵ*/
			if(1 == StartPointGradeRtn)
			{
				if(CurGradeVal>StartPointGradeVal)
				{
					minGradeVal = StartPointGradeVal;
					maxGradeVal = CurGradeVal;
				}
				else
				{
					minGradeVal = CurGradeVal;
					maxGradeVal = StartPointGradeVal;
				}
			}
			else if(2 == StartPointGradeRtn)
			{
				minGradeVal = CurGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
			else
			{
				minGradeVal = StartPointGradeVal;
				maxGradeVal = StartPointGradeVal;
			}
		}
		else
		{
			/*��ʼ�¶�Ϊͳһ�¶�,���������Сֵ���г�ʼ��ֵ*/
			minGradeVal = StartPointGradeVal;
			maxGradeVal = StartPointGradeVal;
		}
		
		if(0xffff != EndGradeId)
		{
			pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[EndGradeId];
			/*�ն������¶�ֵ������Ϊ��������Ϊ��*/
			if(0x55 == pstGrade->wInclineDir)
			{
				CurGradeVal = (FLOAT32)(1.0 * pstGrade->wGradValue);
			}
			else
			{
				CurGradeVal = (FLOAT32)(-1.0 * pstGrade->wGradValue);
			}
			/*ȡ�õ�ǰ�¶ȶ����ǰ���������С�¶�ֵ*/
			if(1 == EndPointGradeRtn)
			{
				/*��ĩ�˵����ҵ���С����¶�ֵ*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*����ִ��*/
				}
			}
			else if(2 == EndPointGradeRtn)
			{
				/*��ĩ�˵����ҵ���С����¶�ֵ*/
				if(maxGradeVal<EndPointGradeVal)
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if(minGradeVal>EndPointGradeVal)
				{
					minGradeVal = EndPointGradeVal;
				}
				else
				{
					/*����ִ��*/
				}
			}
			else
			{
				/*������ֵ*/
				if((EndPointGradeVal>= maxGradeVal)&&(EndPointGradeVal>= CurGradeVal))
				{
					maxGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal>= maxGradeVal)&&(CurGradeVal>= EndPointGradeVal))
				{
					maxGradeVal = CurGradeVal;
				}
				else
				{
					/*����ִ��*/
				}

				/*�����Сֵ*/
				if((EndPointGradeVal<= minGradeVal)&&(EndPointGradeVal<= CurGradeVal))
				{
					minGradeVal = EndPointGradeVal;
				}
				else if((CurGradeVal<= minGradeVal)&&(CurGradeVal<=EndPointGradeVal ))
				{
					minGradeVal = CurGradeVal;
				}
				else
				{
					/*����ִ��*/
				}
			}
		}
		else
		{
			/*�յ��¶�Ϊͳһ�¶�*/
			/*��ĩ�˵����ҵ���С����¶�ֵ*/
			if(maxGradeVal<EndPointGradeVal)
			{
				maxGradeVal = EndPointGradeVal;
			}
			else if(minGradeVal>EndPointGradeVal)
			{
				minGradeVal = EndPointGradeVal;
			}
			else
			{
				/*����ִ��*/
			}
		}
		

		/*�赱ǰ�¶�idΪ��ʼ�¶�id*/
		wCurrentGradeID = StartGradeId;
		/*��¼��ǰlink��id��Ϣ*/
		wCurrentLinkId = StartLinkId;

		pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[wCurrentLinkId];

		pLinkStruEnd = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[EndLinkId];/*add by zlj 20161024*/
		/*��ǰlink�ġ�Link�¶���Ϣ���ԡ�Ϊ��ͳһ�¶�*/
		/*if(0 == pLinkStru->wGrade)*//*changed by zlj 20161024*/
        if((0 == pLinkStru->wGrade)||(0 == pLinkStruEnd->wGrade))
		{

			/*ѭ����ȡ����յ����¶���Ϣ���ȶԵõ����,��Сֵ*/
			for(i=0;i<dsuDataLenStru->wGradeLen;i++)
			{
				if(0xffff != wCurrentGradeID)
				{
					/*���¶ȱ��в�ѯ��ǰ�¶ȵ�ǰ���¶�*/
					wNextGradeID = FindAdjacentGradeIDExp(pDsuEmapStru,wCurrentGradeID, 0x55, PointNum, pPointStru);
					
					/*��ȡǰ���¶�*/
					if (0xffff != wNextGradeID)
					{
						/*����ѯ�õ����¶ȹ���ID��Ϊ��,���ù������ҵ���ǰ���¶���Ϣ*/
						pstGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[wNextGradeID];	
						/*��ȡ��ǰ�����¶ȵ��յ�link��Ϣ*/
						wCurrentLinkId = pstGrade->wTmnlLinkId;
					}				
					else
					{
						/*�����¶ȱ��л�ȡ�����յ�����¶�Ϊ��Ч,�����Ϊ��·�յ�,������ͳһ�¶ȱ��е���Ϣ*/
						
						/*��link���в�ѯǰ���¶��Ƿ���ͳһ�¶ȱ���,*/
						rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);
						
						if((1 == rstGradeNxt)&&(NULL == pstGrade))
						{
							/*��ǰ���¶���ͳһ�¶ȱ���,���ȡͳһ�¶ȱ���Ϣ,�����¶ȱȽ�*/
							stGrade.wInclineDir = EMAP_SAME_DIR;
							stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
							pstGrade = &stGrade;
							
							/*�����¶�Ϊͳһ�¶�,�����Ϊ��Ч*/
							wNextGradeID = 0xffff;
						}
						else
						{
							/*��ǰ���¶�δ��ͳһ�¶ȱ���,��ǰ��ѯӦΪ��·�յ�*/
							/*��·�յ�(�ҵ���·�յ�Ҳδ���ҵ��յ��¶�ID)*/
							bReturnValue = 0;
							return bReturnValue;
						}
						/*���µ�ǰlink��ID��Ϣ�Ա�ͳһ�¶ȱ��ϵ�Ĳ�ѯ*/
						rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

						if(1 != rstGetIdNxt)
						{
							/*��ѯ��·linkʧ��*/
							bReturnValue = 0;
							return bReturnValue;
						}
					}
				}
				else
				{
					/*��ѯ��ǰlink��ʼ�������¶�*/
					rstGradeNxt = dsuGetGradeRearExp(pDsuEmapStru,wCurrentLinkId, &pstGrade);
					
					if(1 == rstGradeNxt)
					{
						if(NULL == pstGrade)
						{
							/*��ǰ���¶���ͳһ�¶ȱ���,���ȡͳһ�¶ȱ���Ϣ,�����¶ȱȽ�*/
							stGrade.wInclineDir = EMAP_SAME_DIR;
							stGrade.wGradValue = dsuStaticHeadStru->pConGradeStru->wGrade;
							pstGrade = &stGrade;

							/*�����¶�Ϊͳһ�¶�,�����Ϊ��Ч*/
							wNextGradeID = 0xffff;
						}
						else
						{
							/*����ʵ�ʲ�ѯ�õ�pstGrade���¶ȱ��е��¶�ֵ���д���*/
							/*��¼��ǰlink�������¶�ID*/
							wNextGradeID = pstGrade->wId;
						}
						
						/*Ϊ�������¶ȱ��еĲ�ѯ����ȫ,����ͳһ�¶ȱ��еĴ�������ѭ��*/
						if(i>=1)
						{
							i--;
						}
					}
					else
					{
						/*��·�յ�(�ҵ���·�յ�Ҳδ���ҵ��յ��¶�ID)*/
						bReturnValue = 0;
						return bReturnValue;
					}

					if (wCurrentLinkId == EndLinkId)
					{
						/*�鵽�ն��¶�*/
						break;
					}
					/*20141122��ӣ����µ�ǰlink��ID��Ϣ�Ա�ͳһ�¶ȱ��ϵ�Ĳ�ѯ*/
					rstGetIdNxt = dsuGetAdjacentLinkIDExp(pDsuEmapStru,wCurrentLinkId, 0x55, PointNum, pPointStru, &wCurrentLinkId);

					if(1 != rstGetIdNxt)
					{
						/*��ѯ��·linkʧ��*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				
				/*���µ�ǰ�¶�ID��Ϣ�Ա��¶ȱ��ϵ�Ĳ�ѯ*/
				wCurrentGradeID = wNextGradeID;
				
				if( ((wNextGradeID == EndGradeId)&&((0xffff != wNextGradeID)&&(0xffff != EndGradeId)))||
					((wCurrentLinkId == EndLinkId)&&(0xffff == wNextGradeID)) )
				{
					/*�鵽�ն��¶�*/
					break;
				}
			
				if(0x55 == pstGrade->wInclineDir)
				{
					/*����и�����¶Ȼ��С���¶ȣ��滻��ǰֵ*/
					if(minGradeVal>=pstGrade->wGradValue)
					{
						minGradeVal = pstGrade->wGradValue;
					}
					if(maxGradeVal<=pstGrade->wGradValue)
					{
						maxGradeVal = pstGrade->wGradValue;
					}
				}
				else
				{
					/*����и�����¶Ȼ��С���¶ȣ��滻��ǰֵ*/
					if(minGradeVal>=-1.0*pstGrade->wGradValue)
					{
						minGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
					}
					if(maxGradeVal<=-1.0*pstGrade->wGradValue)
					{
						maxGradeVal = (FLOAT32)(-1.0*pstGrade->wGradValue);
					}
				}
			}
        }
		else
		{
			/*�ҵ�ͳһ�¶�*/
			if(fabs(minGradeVal-maxGradeVal)<0.001f)
			{
				*Gradient = minGradeVal/10;
				bReturnValue = 1;
				return bReturnValue;
			}
			else
			{
			    /*���ɴ��֧*/
				bReturnValue = 0;
				return bReturnValue;
			}

		}

		if(i>=dsuDataLenStru->wGradeLen)
		{
			/*û�ҵ�ĩ���¶�*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
	
	/*�������з���ȷ������¶�ֵ*/
	if(0x55 == pLODStartStru->Dir)
	{
		*Gradient = minGradeVal/10;
		bReturnValue = 1;
	    return bReturnValue;
	}
	else
	{
		*Gradient = (FLOAT32)(-1.0 * maxGradeVal/10);
		bReturnValue = 1;
	    return bReturnValue;
	}

}


/*���ʽӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
UCHAR dsuGradientGetExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
	const UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, FLOAT32 *Gradient)
{
	UINT8 cbtcVersionType=0;
	UCHAR bReturnValue = 0;/*��������ֵ*/
	cbtcVersionType=GetCbtcSysType();
	
	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		bReturnValue=dsuGradientGetExp_FAO(pDsuEmapStru,pLODStartStru, pLODEndStru, PointNum,pPointStru,Gradient);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType)||(DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		bReturnValue=dsuGradientGetExp_CPK(pDsuEmapStru,pLODStartStru, pLODEndStru, PointNum,pPointStru,Gradient);
	}

	else
	{
		bReturnValue = 0;		
	}
	return bReturnValue;
}


/*
�������ܣ��жϺ�һ�¶ȵ��¶�ֵ���ǰһ���¶ȵ��¶�ֵ�����ӣ����仯���Ǽ��� 
���������UINT16 fistGradeID,UINT16 secondGradeID
���������GradeTrend �¶� 
			1�� ����  
			2�� ����
			0�� ���仯
  ����ֵ��0ʧ�� 1�ɹ�
*/
UINT8 dquGradeTrendExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 fistGradeID,UINT16 secondGradeID,UINT8 *GradeTrend)
{
	UINT8 bReturnValue = 0;	
	INT32 signedFirstGrade=0;/*���з��ŵ��¶�ֵ--ǰһ���¶�*/
	INT32 signedSecondGrade=0;/*���з��ŵ��¶�ֵ--��һ���¶�*/
	DSU_GRADE_STRU *fistGrade=NULL;/*ָ���¶���Ϣ�Ľṹ��ָ��*/
	DSU_GRADE_STRU *secondGrade=NULL;/*ָ���¶���Ϣ�Ľṹ��ָ��*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 GRADEINDEXNUM=0;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		GRADEINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->GRADEINDEXNUM;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}


	if(0xffff != fistGradeID)
	{
		if((fistGradeID>GRADEINDEXNUM)||((dsuGradeIndex[fistGradeID]) == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}
		
		/*��ʼ���¶�*/
		fistGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[fistGradeID];
			
		/*0x55=EMAP_SAME_INC_LINE��ʾ����,�¶�ֵΪ����
		  0xaa=EMAP_CONVER_INC_LINE��ʾ���£��¶�ֵΪ����*/
	
		/*ǰһ���¶�*/
		if (fistGrade->wInclineDir == EMAP_SAME_INC_LINE) 
		{
			signedFirstGrade=(INT32)fistGrade->wGradValue;
		}
		else if (fistGrade->wInclineDir == EMAP_CONVER_INC_LINE)
		{
			signedFirstGrade=(-1)*((INT32)(fistGrade->wGradValue));
		}
		else
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
	else
	{
		signedFirstGrade = dsuStaticHeadStru->pConGradeStru->wGrade;
	}	

	if(0xffff != secondGradeID)
	{
		if((secondGradeID>GRADEINDEXNUM)||((dsuGradeIndex[secondGradeID]) == 0xFFFF))
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}
		
		/*��ʼ���¶�*/
		secondGrade = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[secondGradeID];
		
		/*��һ���¶�*/
		if (secondGrade->wInclineDir == EMAP_SAME_INC_LINE) 
		{
			signedSecondGrade=(INT32)secondGrade->wGradValue;
		}
		else if (secondGrade->wInclineDir == EMAP_CONVER_INC_LINE)
		{
			signedSecondGrade=(-1)*((INT32)(secondGrade->wGradValue));
		}
		else
		{
			/*��ڲ����쳣����ѯʧ��*/
			bReturnValue = 0;
			return bReturnValue;
		}
	}
	else
	{
		signedSecondGrade = dsuStaticHeadStru->pConGradeStru->wGrade;
	}	
	
	if((0xffff != fistGradeID)&&(0xffff != secondGradeID)&&
		(secondGradeID != fistGrade->wTmnlJointMainGradeId)&&(secondGradeID != fistGrade->wTmnlJointSideGradeId))
	{
	    /*��ڲ����쳣����ѯʧ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*
	�ú�һ���¶ȵ��¶�ֵ��ȥǰһ���¶ȵ��¶�ֵ
	���Ϊ�����¶�ֵ����
	���Ϊ�����¶�ֵ����
	���Ϊ�㣺�¶�ֵ���仯
	*/

	if (signedSecondGrade-signedFirstGrade > 0)
	{
		*GradeTrend = 2;
		bReturnValue = 1;
		return bReturnValue;
	}
	else if (signedSecondGrade-signedFirstGrade < 0)
	{
		*GradeTrend = 1;
		bReturnValue = 1;
		return bReturnValue;
	}
	else
	{
		*GradeTrend = 0;
		bReturnValue = 1;
		return bReturnValue;
	}

}

/*
�������ܣ���ȡǰ����¶�
���������UINT16 linkId,��ǰlink
���������gradeFrnt �¶� 
			Null��  ��·�յ��ǰ���¶�Ϊͳһ�¶�  
			��Null��ǰ���¶����¶ȱ��пɲ�
  ����ֵ��0ʧ�� 1�ɹ� 2��·�յ�
*/
UINT8 dsuGetGradeFrntExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeFrnt)
{	
	UINT16 GradeIDOrg = 0;					/*��¼ǰ���¶ȵ�ID*/
	UINT16 GradeID = 0;						/*��¼��ǰ�¶ȵ�ID*/
	UINT8  bReturnValue = 0;				/*��������ֵ*/
	DSU_LINK_STRU *pLinkStru=NULL;				/*Link��Ϣ�ṹ��ָ��*/
	DSU_LINK_STRU *pLinkStruOrg=NULL;       		/*link����ʱ����*/
	DSU_GRADE_STRU* gradeFrntTmp=NULL;			/*��ʱ�¶ȱ���*/
	UINT16 i = 0;							/*ѭ����������*/
	UINT8 RstGetOrg = 0;					/*��ѯǰ���¶�Id����ֵ*/
	UINT8 RstGetCur = 0;					/*��ѯ��ǰ�¶�Id����ֵ*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж�����������Link����Ч��*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*����ǰlinkλ�����ڵ�link���¶ȱ��п��Բ�ѯ��,���ȡ��Ӧ������;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wOrgnJointMainLkId)
		{
			pLinkStruOrg = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wOrgnJointMainLkId];

			/*������link���¶Ȳ�Ϊͳһ�¶�*/
			if(0 == pLinkStruOrg->wGrade)
			{
				/*��ѯ������link��Ӧ���¶�ID*/
				RstGetOrg = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruOrg->wId, pLinkStruOrg->dwLength, &GradeIDOrg);

				/*��ѯ��ǰlink��Ӧ���¶�ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, 0, &GradeID);

				if((0 == RstGetOrg)||(0 == RstGetCur))
				{
					/*��ѯʧ��*/
					bReturnValue = 0;
					return bReturnValue;
				}

				/*����link�뵱ǰlink���¶�ID��ͬ*/
				if(GradeIDOrg != GradeID)
				{	
					if(0xffff != GradeIDOrg)
					{				
						/*��ʼ��ǰ�������¶�*/
						gradeFrntTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDOrg];
						*gradeFrnt = gradeFrntTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*���ݳ���(link���в�Ϊͳһ�¶�,��ѯ�õ�Ϊͳһ�¶�)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruOrg;
				}
			}
			else
			{
				/*������link���¶���Ϊͳһ�¶�*/
				*gradeFrnt = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*����ǰlink������link��Ϊ��·�յ�,�����Ϊ��Чֵ*/
			*gradeFrnt = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*��ѭ�����δ���ҵ���Ӧ�¶�ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeFrnt = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}
}

UINT8 dsuGetGradeFrntExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeFrnt)
{	
	UINT16 GradeIDOrg = 0;					/*��¼ǰ���¶ȵ�ID*/
	UINT16 GradeID = 0;						/*��¼��ǰ�¶ȵ�ID*/
	UINT8  bReturnValue = 0;				/*��������ֵ*/
	DSU_LINK_STRU *pLinkStru=NULL;				/*Link��Ϣ�ṹ��ָ��*/
	DSU_LINK_STRU *pLinkStruOrg=NULL;       		/*link����ʱ����*/
	DSU_GRADE_STRU* gradeFrntTmp=NULL;			/*��ʱ�¶ȱ���*/
	UINT16 i = 0;							/*ѭ����������*/
	UINT8 RstGetOrg = 0;					/*��ѯǰ���¶�Id����ֵ*/
	UINT8 RstGetCur = 0;					/*��ѯ��ǰ�¶�Id����ֵ*/
	
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж�����������Link����Ч��*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	
	/*����ǰlinkλ�����ڵ�link���¶ȱ��п��Բ�ѯ��,���ȡ��Ӧ������;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];
	
	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wOrgnJointMainLkId)
		{
			pLinkStruOrg = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wOrgnJointMainLkId];
			
			/*������link���¶Ȳ�Ϊͳһ�¶�*/
			if(0 == pLinkStruOrg->wGrade)
			{
				/*��ѯ������link��Ӧ���¶�ID*/
				RstGetOrg = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruOrg->wId, pLinkStruOrg->dwLength, &GradeIDOrg);
				
				/*��ѯ��ǰlink��Ӧ���¶�ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, 0, &GradeID);
				
				if((0 == RstGetOrg)||(0 == RstGetCur))
				{
					/*��ѯʧ��*/
					bReturnValue = 0;
					return bReturnValue;
				}
				
				/*����link�뵱ǰlink���¶�ID��ͬ*/
				if(GradeIDOrg != GradeID)
				{	
					if(0xffff != GradeIDOrg)
					{				
						/*��ʼ��ǰ�������¶�*/
						gradeFrntTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDOrg];
						*gradeFrnt = gradeFrntTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*���ݳ���(link���в�Ϊͳһ�¶�,��ѯ�õ�Ϊͳһ�¶�)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruOrg;
				}
			}
			else
			{
				/*������link���¶���Ϊͳһ�¶�*/
				*gradeFrnt = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*����ǰlink������link��Ϊ��·�յ�,�����Ϊ��Чֵ*/
			*gradeFrnt = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}
	
	/*��ѭ�����δ���ҵ���Ӧ�¶�ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeFrnt = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*���ɴ��֧*/
		bReturnValue=0;
		return bReturnValue;
	}
}
/*���ʽӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
UINT8 dsuGetGradeFrntExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU **gradeFrnt)
{
	UINT8 rtnValue = 0;/*��������ֵ*/
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue=dsuGetGradeFrntExp_FAO(pDsuEmapStru,linkId, gradeFrnt);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || ( DQU_CBTC_HLT == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType))
	{
		rtnValue=dsuGetGradeFrntExp_CPK(pDsuEmapStru,linkId, gradeFrnt);
	}
	else
	{
		rtnValue = 0;
	}
	return rtnValue; 
}

/*
�������ܣ���ȡ������¶�
���������UINT16 linkId,��ǰlink
���������gradeRear �¶� 
			Null��  ��·�յ������¶�Ϊͳһ�¶�  
			��Null�������¶����¶ȱ��пɲ�
  ����ֵ��0ʧ�� 1�ɹ� 2��·�յ�
*/
UINT8 dsuGetGradeRearExp_FAO(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeRear)
{
	UINT16 GradeIDNxt = 0;					/*��¼�����¶ȵ�ID*/
	UINT16 GradeID = 0;						/*��¼��ǰ�¶ȵ�ID*/
	UINT8  bReturnValue = 0;				/*��������ֵ*/
	DSU_LINK_STRU* pLinkStru=NULL;				/*Link��Ϣ�ṹ��ָ��*/
	DSU_LINK_STRU* pLinkStruNxt=NULL;			/*link����ʱ����*/
	DSU_GRADE_STRU* gradeRearTmp=NULL;			/*�¶���ʱ����*/
	UINT16 i = 0;							/*ѭ����������*/
	UINT8 RstGetNxt = 0;					/*��ѯǰ���¶�Id����ֵ*/
	UINT8 RstGetCur = 0;					/*��ѯ��ǰ�¶�Id����ֵ*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж�����������Link����Ч��*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*����ǰlinkλ�����ڵ�link���¶ȱ��п��Բ�ѯ��,���ȡ��Ӧ������;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];

	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wTmnlJointMainLkId)
		{
			pLinkStruNxt = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wTmnlJointMainLkId];

			/*������link���¶Ȳ�Ϊͳһ�¶�*/
			if(0 == pLinkStruNxt->wGrade)
			{
				/*��ѯ������link��Ӧ���¶�ID*/
				RstGetNxt = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruNxt->wId, 0, &GradeIDNxt);

				/*��ѯ��ǰlink��Ӧ���¶�ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, pLinkStru->dwLength, &GradeID);

				if((0 == RstGetNxt)||(0 == RstGetCur))
				{
					/*��ѯʧ��*/
					bReturnValue = 0;
					return bReturnValue;
				}

				/*����link�뵱ǰlink���¶�ID��ͬ*/
				if(GradeIDNxt != GradeID)
				{
					if(0xffff != GradeIDNxt)
					{
						/*��ʼ�����������¶�*/
						gradeRearTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDNxt];
						*gradeRear = gradeRearTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*�����ݳ���(link���в�Ϊͳһ�¶�,��ѯ�õ�Ϊͳһ�¶�)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruNxt;
				}
			}
			else
			{
				/*������link���¶���Ϊͳһ�¶�*/
				*gradeRear = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*����ǰlink������link��Ϊ��·�յ�*/
			*gradeRear = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*��ѭ�����δ���ҵ���Ӧ�¶�ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeRear = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}

}

UINT8 dsuGetGradeRearExp_CPK(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeRear)
{
	UINT16 GradeIDNxt = 0;					/*��¼�����¶ȵ�ID*/
	UINT16 GradeID = 0;						/*��¼��ǰ�¶ȵ�ID*/
	UINT8  bReturnValue = 0;				/*��������ֵ*/
	DSU_LINK_STRU* pLinkStru=NULL;				/*Link��Ϣ�ṹ��ָ��*/
	DSU_LINK_STRU* pLinkStruNxt=NULL;			/*link����ʱ����*/
	DSU_GRADE_STRU* gradeRearTmp=NULL;			/*�¶���ʱ����*/
	UINT16 i = 0;							/*ѭ����������*/
	UINT8 RstGetNxt = 0;					/*��ѯǰ���¶�Id����ֵ*/
	UINT8 RstGetCur = 0;					/*��ѯ��ǰ�¶�Id����ֵ*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	UINT16* dsuLinkIndex=NULL;
	UINT16* dsuGradeIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	if (NULL != pDsuEmapStru)
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuLinkIndex=pDsuEmapStru->dsuEmapIndexStru->dsuLinkIndex;
		dsuGradeIndex=pDsuEmapStru->dsuEmapIndexStru->dsuGradeIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		bReturnValue = 0;
		return bReturnValue;
	}

	/*�ж�����������Link����Ч��*/
	if((linkId>LINKINDEXNUM)||(dsuLinkIndex[linkId] == 0xFFFF))
	{
		/*���������Ч�������쳣����ѯʧ�ܣ����ش���*/
		bReturnValue = 0;
		return bReturnValue;
	}
	
	/*����ǰlinkλ�����ڵ�link���¶ȱ��п��Բ�ѯ��,���ȡ��Ӧ������;*/	
	pLinkStru = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[linkId];
	
	for(i=0;i<dsuDataLenStru->wLinkLen;i++)
	{
		if(0xffff != pLinkStru->wTmnlJointMainLkId)
		{
			pLinkStruNxt = dsuStaticHeadStru->pLinkStru + dsuLinkIndex[pLinkStru->wTmnlJointMainLkId];
			
			/*������link���¶Ȳ�Ϊͳһ�¶�*/
			if(0 == pLinkStruNxt->wGrade)
			{
				/*��ѯ������link��Ӧ���¶�ID*/
				RstGetNxt = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStruNxt->wId, 0, &GradeIDNxt);

				/*��ѯ��ǰlink��Ӧ���¶�ID*/
				RstGetCur = dsuGetGradeIdbyLocExp(pDsuEmapStru,pLinkStru->wId, pLinkStru->dwLength, &GradeID);
				
				if((0 == RstGetNxt)||(0 == RstGetCur))
				{
					/*��ѯʧ��*/
					bReturnValue = 0;
					return bReturnValue;
				}
				
				/*����link�뵱ǰlink���¶�ID��ͬ*/
				if(GradeIDNxt != GradeID)
				{
					if(0xffff != GradeIDNxt)
					{
						/*��ʼ�����������¶�*/
						gradeRearTmp = dsuStaticHeadStru->pGradeStru + dsuGradeIndex[GradeIDNxt];
						*gradeRear = gradeRearTmp;
						bReturnValue = 1;
						return bReturnValue;
					}
					else
					{
						/*�����ݳ���(link���в�Ϊͳһ�¶�,��ѯ�õ�Ϊͳһ�¶�)*/
						bReturnValue = 0;
						return bReturnValue;
					}
				}
				else
				{
					pLinkStru = pLinkStruNxt;
				}
			}
			else
			{
				/*������link���¶���Ϊͳһ�¶�*/
				*gradeRear = NULL;
				bReturnValue = 1;
				return bReturnValue;
			}
		}
		else
		{
			/*����ǰlink������link��Ϊ��·�յ�*/
			*gradeRear = NULL;
			bReturnValue = 2;
			return bReturnValue;
		}
	}

	/*��ѭ�����δ���ҵ���Ӧ�¶�ID*/
	if(i == dsuDataLenStru->wLinkLen)
	{
		*gradeRear = NULL;
		bReturnValue = 0;
		return bReturnValue;
	}
	else
	{
		/*���ɴ��֧*/
		bReturnValue=0;
		return bReturnValue;
	}
}
/*���ʽӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
UINT8 dsuGetGradeRearExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU **gradeRear)
{
	UINT8 rtnValue = 0;
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if (DQU_CBTC_FAO == cbtcVersionType)
	{
		rtnValue=dsuGetGradeRearExp_FAO(pDsuEmapStru,linkId, gradeRear);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType)||(DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		rtnValue=dsuGetGradeRearExp_CPK(pDsuEmapStru,linkId, gradeRear);
	}
	else
	{
		rtnValue = 0;
	}
	return rtnValue; 
}

/**
�������ܣ���ȡ��ǰLINK�ľ�̬���ٸ���
���������
@linkId����ǰlink
�����������
����ֵ����̬���ٸ���
*/
UINT8 GetStcLmtSpdCntByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId)
{
	UINT8 ret = 0;

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 LINKINDEXNUM=0;
	if ((NULL != pDsuEmapStru) && (NULL !=pDsuLimitStcLinkIdx))
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		ret = 0;
	}

	if(linkId <= LINKINDEXNUM)
	{
		if (pDsuLimitStcLinkIdx[linkId].linkId == linkId)
		{
		    /*����linkId��Ӧ��̬���ٱ�����ٸ���*/
			ret = pDsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount;
		}
		else
		{
			ret = 0;
		}
		
	}
	else
	{
		ret = 0;
	}

	return ret;
}

/**
�������ܣ���ȡ�ƶ�LINK�ĵ�ָ���±�ľ�̬��ʱ���ٵĶ���ָ��
���������
@linkId����ǰLink���
@idx����ӦLink�ĵڼ������٣��±�ȡֵ��Χ0-9
����ֵ��
@NULL����ȡʧ�� 
@�ǿգ���Ӧ���ٽṹ��ָ��
*/
DSU_STATIC_RES_SPEED_STRU *GetStcLmtSpdInfoByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId,const UINT8 idx)
{
	DSU_STATIC_RES_SPEED_STRU *pStcLmtSpdInfo = NULL;

	UINT16 LINKINDEXNUM=0;
	UINT16* dsuStaticResSpeedIndex=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	if ( (NULL != pDsuEmapStru) && (NULL != pDsuLimitStcLinkIdx) )
	{
		LINKINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->LINKINDEXNUM;
		dsuStaticResSpeedIndex=pDsuEmapStru->dsuEmapIndexStru->dsuStaticResSpeedIndex;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		pStcLmtSpdInfo = NULL;
	}

	if ((idx >= LINK_MAX_LIMIT_SPD) || (idx < 0))
	{
		return pStcLmtSpdInfo;
	}
	if(linkId <= LINKINDEXNUM)
	{
		if(idx < pDsuLimitStcLinkIdx[linkId].limitStcSpdInfoCount)
		{
			/*��linkId��Ӧ��̬���ٱ�ĵ�idx��̬����ȡ����*/
			pStcLmtSpdInfo = dsuStaticHeadStru->pStaticResSpeedStru + dsuStaticResSpeedIndex[pDsuLimitStcLinkIdx[linkId].limitStcSpdInfoIdBuf[idx]];
		}
	}
	else
	{
		pStcLmtSpdInfo = NULL;
	}

	return pStcLmtSpdInfo;
}

/*
�������ܣ����࿪����תΪ�������
���������multiPointId �࿪������
          multiPointState �࿪����״̬

���������wVirtualPointIds ��ֺ���������� 
          wVirtualPointStates ��ֺ��������״̬
����ֵ������ֵ����ֺ�����������, 0ʧ��  

�����㷨���࿪���������������� Nλ��R1λ��L1λ��R2λ��L2λ
            ����Ӧ�������е������� 0��  1��   2��   3��   4
		    ����Ӧ�Ķ࿪����״̬�� 1��  2��   3��   4��   5
		   ��˵� ����=�࿪����״̬multiPointState-1 ʱ������Ӧ�����������ԭֵ�������������״̬ȡ������λ<=>��λ��

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

		   ��ͼ1ʵ����������1��״̬Ϊ3ʱ���Ƴ�ͼ2���ⵥ������ΪLλ����ʱ101Ϊ��Ч���������״̬Ϊ3������ֵ�������Ϊ2������
		   wVirtualPointIds[0]=102;
		   wVirtualPointStates[0]=2;
		   wVirtualPointIds[1]=101;
		   wVirtualPointStates[1]=3;
author:������Ŀ add by qxt 20170622
*/
UINT8 ConvertToVirtualSwitchesExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 multiPointId,const UINT8 multiPointState,UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM])
{
	UINT8 chReturnValue = 0;/*��������ֵ*/
	UINT16 i = 0,j=0,k=0;          /*ѭ���ñ���*/
	DSU_MULTI_SWITCH_STRU* pMultiPointStru=NULL; /*�࿪������ʱ������ṹ��*/

	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 MULTIPOINTINDEXNUM=0;
	UINT16 *dsuMultiPointIndex=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
    DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;
	
	/*���������ʼ��Ϊ��Чֵ65535*/
	for(i=0;i<MAX_VIRTUAL_SWITCH_NUM;i++)
	{
		wVirtualPointIds[i]=DSU_NULL_16;
		wVirtualPointStates[i]=DSU_NULL_8;
	}

	if ((NULL != pDsuEmapStru)&&(NULL!=pDsuEmapStru->dsuEmapIndexStru))
	{
		MULTIPOINTINDEXNUM=pDsuEmapStru->dsuEmapIndexStru->MULTIPOINTINDEXNUM;
		dsuMultiPointIndex=pDsuEmapStru->dsuEmapIndexStru->dsuMultiPointIndex;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*�ж���������������Ч��*/
		if ((multiPointId<=MULTIPOINTINDEXNUM)&&(dsuMultiPointIndex[multiPointId]!=DSU_NULL_16)&&(multiPointState<=MAX_VIRTUAL_SWITCH_NUM)&&(multiPointState>=1))
		{
			/*�����࿪�����*/
			for (j=0;j<dsuDataLenStru->wMultiPointLen;j++)
			{
				pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+j;
				if ((pMultiPointStru->wId==multiPointId)&&(multiPointState<=pMultiPointStru->wType))
				{
					/*���࿪����״̬��1ʱ���赥��������ΪNλ�����R1λ��������ͬһ����,Ϊ��ֹ���������ͬ���������������ʱ����������Ӧ��1��ʼ*/
					if (1==multiPointState)
					{
						for(k=1;k<pMultiPointStru->wType;k++)
						{
							/*��������Ÿ�ֵ*/
							wVirtualPointIds[k-1]=pMultiPointStru->wSwitchIds[k];
							/*�������״̬��ֵ���������״ֻ̬��Nλ��ͬ������λ����״̬��ȡ��(2=>1;1=>2*/
							wVirtualPointStates[k-1]=0x03&~pMultiPointStru->wSwitchStates[k];
						}
					}
					else
					{

						for(k=1;k<pMultiPointStru->wType;k++)
						{
							/*��������Ÿ�ֵ*/
							wVirtualPointIds[k-1]=pMultiPointStru->wSwitchIds[k];
							/*�������״̬��ֵ*/
							if (k==(multiPointState-1))
							{
								wVirtualPointStates[k-1]=pMultiPointStru->wSwitchStates[k];
							}
							/*������multiPointState-1֮ǰ����������Ϊ��Ч��*/
							else if (k<(multiPointState-1))
							{
								wVirtualPointStates[k-1]=EMAP_POINT_STATUS_LOSE;
							}
							/*������multiPointState-1֮����������״̬�÷�*/
							else
							{  
								/*����״̬ȡ��(2=>1;1=>2)*/
								wVirtualPointStates[k-1]=0x03&~pMultiPointStru->wSwitchStates[k];

							}

						}

					}
													
					chReturnValue=pMultiPointStru->wType-1;
				}
			}
		}
		else
		{
			/*��ڲ����쳣����ѯʧ��*/
			chReturnValue = 0;

		}		    
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;	
	}
	return chReturnValue;

}

/*
�������ܣ����������תΪ�࿪����
���������wVirtualPointIds[] ��������� 
          wVirtualPointStates[] �������״̬
		  wVirtualPointNum    �����������
���������multiPointId ��Ӧ�࿪������
          multiPointState ��Ӧ�࿪����״̬
����ֵ��0ʧ�� 1�ɹ�

ע�������������������wVirtualPointIds��״̬wVirtualPointStates��������Ч�������������Ҫ���롣��
           
		 		      /L                                                 L
		             /                                                /
		            /                                                /
		           /                                                /
		---|------/-----------\------ N      =====>        ------------------ N
		          1          2 \                                    \
							    \                                    \
		                          R                                      R
		         ͼ1.���ⵥ������                             ͼ2.ʵ����������
		 
		 ��ͼ1���������1��״̬Ϊ2ʱ���Ƴ�ͼ2�࿪�����״̬ΪLλ����ʱͼһ�������2Ϊ��Ч�ģ����������������Ϊ���£�
         wVirtualPointIds[0]=1;
		 wVirtualPointStates[0]=2;
         wVirtualPointNum=1;
author:������Ŀ add by qxt 20170622
*/
UINT8 ConvertToMultiSwitchExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointNum,UINT16 *multiPointId,UINT8 *multiPointState)
{
	UINT8 chReturnValue = 0;      /*��������ֵ*/
	UINT16 i= 0,j=0,k=0;          /*ѭ���ñ���*/
	UINT8 IdCount=0;              /* ���ڼ�������������������������е�������ȵĸ�����*/
	UINT8 StateCount=0;            /*�������״̬��ȵĸ���������ֻ����һ��*/
	UINT8 tempPointState=0;        /*�࿪����״̬*/ 
	DSU_MULTI_SWITCH_STRU *pMultiPointStru=NULL; /*�࿪������ʱ������ṹ��*/
	/*ȫ�ֱ����Ĵ��ε����漰��ȫ�ֱ���*/
	UINT16 *dsuMultiPointIndex=NULL;
	DSU_DATA_LEN_STRU *dsuDataLenStru=NULL;
	DSU_STATIC_HEAD_STRU *dsuStaticHeadStru=NULL;

	/*���������ʼ��Ϊ��Чֵ*/
	*multiPointId=DSU_NULL_16;
	*multiPointState=DSU_NULL_8;

	if ((NULL != pDsuEmapStru)&&(NULL!=pDsuEmapStru->dsuEmapIndexStru))
	{
		dsuMultiPointIndex=pDsuEmapStru->dsuEmapIndexStru->dsuMultiPointIndex;
		dsuDataLenStru=pDsuEmapStru->dsuDataLenStru;
		dsuStaticHeadStru=pDsuEmapStru->dsuStaticHeadStru;

		/*�ж���������������Ч��*/
		if (wVirtualPointNum<=MAX_VIRTUAL_SWITCH_NUM)
		{
			/*�����࿪�����*/
			for (i=0;i<dsuDataLenStru->wMultiPointLen;i++)
			{
				IdCount=0;
				StateCount=0;
				pMultiPointStru=dsuStaticHeadStru->pMultiPointStru+i;
				
				for(k=0;k<wVirtualPointNum;k++)
				{
					for(j=0;j<pMultiPointStru->wType;j++)
					{
						if (wVirtualPointIds[k]==pMultiPointStru->wSwitchIds[j])
						{
							IdCount++;
							if (wVirtualPointStates[k]==pMultiPointStru->wSwitchStates[j])
							{
								tempPointState=j; /*�࿪�����״̬=�������������ֵ+1*/
		
								StateCount++;  /*�������״̬��ȵĸ���ֻ����һ����*/
							}
						}
					}
                  
				}
				/*�ҵ�����Ӧ�Ķ࿪���������������
				�ٵ�����ĵ�����Nλ��Ҳ��R1λ���ĵ���ʱ���ҵ�����ȵ������������IdCount����������������������wVirtualPointNum+1������ΪNλ��Rλ����ͬ�����ţ�����ʱ����һ�Σ���
                  ������ĵ���û��Nλ��Ҳ��R1λ���ĵ���ʱ���ҵ�����ȵ������������IdCount����������������������wVirtualPointNum��
				�������������״̬�ͱ����������״̬��ȵĸ���ֻ����һ������StateCount=1
				*/
				if (((wVirtualPointNum==IdCount)||(wVirtualPointNum+1==IdCount))&&(1==StateCount))
				{
					*multiPointId=pMultiPointStru->wId;  /*�࿪������*/
					*multiPointState=tempPointState+1;    /*�࿪����״̬(����ֵ+1)*/
					chReturnValue=1;
					break;
				}
			}

		}
		else
		{	
			/*��ڲ����쳣����ѯʧ��*/
			chReturnValue = 0;			
		}	
	}
	else
	{
		/*���ӵ�ͼ����ָ��Ϊ��*/
		chReturnValue = 0;	
	}
	return chReturnValue;


}
