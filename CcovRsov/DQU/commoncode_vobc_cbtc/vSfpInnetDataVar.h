/************************************************************************
* �ļ���	��  vSfpInnetDataVar.h
* ��Ȩ˵��	��   
* �汾��	��  v1.0  
* ����ʱ��	��  2009.11.12
* ����		��	����ܡ�����
* ��������	��  VOBC��������ϵͳ��ͨ��ʹ��SFPЭ��ʱ�����ݿ��ļ�
* ʹ��ע��	��  ���ļ�����VOBC�����ļ�,���������ά��(��л�����æ�ṩ����)
				��ͷ�ļ���������Ա����
* �޸ļ�¼	��	��
************************************************************************/
#ifndef _V_SFP_INNET_DATA_VAR_H_
#define _V_SFP_INNET_DATA_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonTypes.h"

/*ATP��ATO��MMI��ͨ��ʱ,SFPЭ��ʹ�õ����ݿ�����:
*�ݶ�����ATP�ֱ���ZC1��ZC2,ZC1Ϊ����
*ATOΪCI3,MMIΪCI4,CCOVΪCI5(CCOV��ΪԤ��,����ͨ�ŵ�����).ZC��CIͨ��ʱZCΪ����
*�ܽ����ϵͳIDΪ:����ATP�ֱ�Ϊ0x5501,0x5502;ATOΪ0x7103;MMIΪ0x7104;CCOVΪ0x7105*/
extern const UINT8 vSfpInnetDataBuf[];

#ifdef __cplusplus
}
#endif

#else

#endif

