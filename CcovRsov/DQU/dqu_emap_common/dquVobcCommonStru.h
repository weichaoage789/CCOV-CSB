
/************************************************************************
*
* �ļ���   ��dquVobcCommonStru.h
* ��Ȩ˵�� ��  �������ؿƼ��Ƽ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.08
* ����     ��  �����
* �������� ��VOBCӦ�ù��ýṹ���ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef DQU_VOBC_COMMON_STRUCT_H
#define DQU_VOBC_COMMON_STRUCT_H


#include "CommonTypes.h"



#ifdef __cplusplus
extern "C" {
#endif


/*λ�ýṹ��*/
typedef struct
{
	UINT16 Lnk;/*Link*/
	UINT32 Off;/*Offset*/
	UINT8 Dir;/*����*/
}LOD_STRU;


/*������Ϣ�����������ID��״̬*/
typedef struct{
	UCHAR   PointType;				/*��������ͣ����Կ��ǲ���*/
	UINT16	PointId;				/*�����ID*/
	UINT16	Lnk;					/*�ϰ����������κţ�D_ OBSTACLEL��*/
	UINT32	Off;					/*�ϰ�����������ƫ������D_ OBSTACLEO��*/
	UCHAR	PointStatus;			/*�����״̬*/
	UCHAR	PointExpectedStatus;	/*����Ӧ�õ�״̬*/
}DQU_POINT_STATUS_STRU;


/*��ʱ���ٽṹ��*/
typedef struct
{
	UINT16 TsrSpd;/*����ֵ*/
	UINT16 TsrHeadLnk;/*TSR���Link*/
	UINT32 TsrHeadOff;/*TSR���Offset*/
	UINT16 TsrTailLnk;/*TSR�յ�Link*/
	UINT32 TsrTailOff;/*TSR�յ�Offset*/
}TSR_STRU;

#ifdef __cplusplus
}
#endif

#endif
