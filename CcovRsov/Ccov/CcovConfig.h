/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  CcovConfig.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_CONFIG_H
#define _CCOV_CONFIG_H


/* ATP����CCOV����Ϣ */
typedef struct
{
	unsigned char CurLineNum;					/* ��ǰ��·��� */
	unsigned char NextLineNum;					/* ��һ��·��� */
	unsigned char NextLineVer[4];				/* ��һ��·���ݰ汾�� */
	unsigned char NextLineMd5[16];			/* ��һ��·����MD5�� */
	unsigned char VobcProperties; 				/* ����VOBC�����ԣ�55:���ƶ� AA:�ȴ��� */
	unsigned char arc_id; 								/* ���ƶ�ʹ�õ�ArcnetID */
	unsigned short LocalDevName;				/* �����豸���� */
	unsigned short TcpCtrlMsgLen;				/* TCP���Ʊ��ĳ��� */
	unsigned char TcpCtrlMsg[200];			/* TCP���Ʊ��� */
	unsigned char IsZeroSpeed;					/* �Ƿ����٣�0xaa��ʾ�����٣�0x55��ʾ���� */
	unsigned char IsLocation;						/* �Ƿ���λ�ã�0xaa��ʾ��λ�ã�0x55��ʾ��λ�� */
	unsigned char PreviewResults;				/* Ԥ������0xaa��ʾԤ��ʧ�ܣ�0x55��ʾԤ��ɹ� */
}ATP_TO_CCOV_STRU;


/* CCOV����ATP����Ϣ */
typedef struct
{
	unsigned char CurlineNum;
	unsigned char NextLienNum;
	unsigned char NextLineVer[4];
	unsigned char IsSwitchAvailable;
	unsigned char MMI_Show_Restart;		/*֪ͨMMI����:  0xaa-��ʾ������;  0x55-��ʾ����*/
	unsigned char UpdateRecv;					/*��������:  0xaa-��ʾ������; 0x55-��ʾ����*/
	unsigned char StateRecv;						/*״̬����:  0xaa-��ʾδ����; 0x55-��ʾ����*/
}CCOV_TO_ATP_STRU;


/*�������ؼ����Լ��л����ƽṹ��*/
typedef struct
{
	unsigned char NdsuAskCtrl;				/*0x55��ʼ����У����Ϣ,0XAA�Ѿ���ȡ��У����Ϣ,0xff����������*/
	unsigned char VdsuDownCtrl;			/*0x55��������,0xaa�Ѿ��������;0xff����������*/
	unsigned char DataSwitchCtrl;			/*0x55�л��ɹ�,0xaa�л�ʧ��,0xffĬ��ֵ*/
	unsigned char DataLoadCtrl;				/*0x55�������,0XAA�������,0xff���������*/
	unsigned char LoadSta;						/*0x00����ʧ��;0x01���سɹ�;0x02������;0xffδ֪״̬*/
}HEATE_LOAD_CTRL_STRUCT;


#endif  /*_CCOV_CONFIG_H*/