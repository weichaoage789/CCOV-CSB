/************************************************************************
* �ļ���	��  vSfpInnetDataVar.c
* ��Ȩ˵��	��   
* �汾��	��  v1.0  
* ����ʱ��	��  2009.11.12
* ����		��	����ܡ�����
* ��������	��  VOBC��������ϵͳ��ͨ��ʹ��SFPЭ��ʱ�����ݿ��ļ�
* ʹ��ע��	��  ���ļ�����VOBC�����ļ�,���������ά��(��л�����æ�ṩ����)
				��C�ļ��ж������,ͷ�ļ����ṩextern�������˵���
* �޸ļ�¼	��	��
************************************************************************/
#include "CommonTypes.h"

/*ATP��ATO��MMI��ͨ��ʱ,SFPЭ��ʹ�õ����ݿ�����:
*����ATP�ֱ���ZC1��ZC2,ZC1Ϊ����;
*ATOΪCI3,MMIΪCI4,CCOVΪCI5(CCOV��ΪԤ��,����ͨ�ŵ�����).ZC��CIͨ��ʱZCΪ����
*�ܽ����ϵͳIDΪ:����ATP�ֱ�Ϊ0x5501,0x5502;ATOΪ0x7103;MMIΪ0x7104;CCOVΪ0x7105*/
const UINT8 vSfpInnetDataBuf[] = 
{
	   
	0,4,/*��ͬ�豸ͨ�Ź�ϵ����¼����*/
    /*��¼�ĸ�ʽ����:*/ 
/*                   ����  ���𷽷�������  ���𷽱���   ���𷽱���  ���𷽽���Ack          ���淽  ���淽��������  ���淽����  ���淽����  ��ͨ�Ź�ϵ�гɹ�����           */
/*                   �豸����    ͨ������    �������ڳ���   �����ʱ   ���������ʱ��       �豸����    ͨ������      ���ڳ���    �����ʱ   һ������������С������                               */
	/*ATP->ATO*/		 0x55,		 0x55,		 0x01,0x1D,   0x01,0x1D,   0x01,0x1D,			 0x71,	 0x55,			 0x00,0xC8,  0x04,0xB0,  0x02,				
	/*AOM->TIAS*/		 0x15,		 0x55,		 0x00,0xC8,   0x00,0xC8,   0x00,0xC8,			 0x03,	 0x55,			 0x00,0xC8,  0x00,0xC8,  0x02,				
	/*AOM->ATP*/		 0x14,		 0x55,		 0x00,0xC8,   0x00,0xC8,   0x00,0xC8,			 0x15,	 0x55,			 0x00,0xC8,  0x00,0xC8,  0x02,	 
	/*TRDP-ATO*/		 0x72,		 0x55,		 0x00,0x64,   0x00,0xC8,   0x00,0x64,			 0x71,	 0x55,			 0x00,0xC8,  0x00,0xC8,  0x02,

    0,2,/*��ͬ�豸ͨ�Ź�ϵ����¼����*/
    /*��¼�ĸ�ʽ����:*/ 
/*                   ����  ����      ���𷽷�������  ���𷽱���  ���𷽱��� ���𷽽���Ack    ���淽  ���淽      ���淽��������  ���淽����  ���淽����  ��ͨ�Ź�ϵ�гɹ�����           */
/*                   �豸���� �豸ID         ͨ������    �������ڳ���  �����ʱ  ���������ʱ��  �豸���� �豸ID         ͨ������      ���ڳ���    �����ʱ   һ������������С������     */
/*AOM1->AOM2*/       0x56,   1,               0x55,      0x00,0xC8,  0x00,0xC8,   0x00,0xC8,       0x56,   2,             0x55,      0x00,0xC8,  0x00,0xC8,  0x02,           
/*ZC1->ZC2*/         0x55,   1,               0x55,      0x00,0xC8,  0x00,0xC8,   0x00,0xC8,       0x55,   2,             0x55,      0x00,0xC8,  0x00,0xC8,  0x02                       

};

