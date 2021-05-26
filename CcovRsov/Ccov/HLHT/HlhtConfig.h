/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  HlhtConfig.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _HLHT_CONFIG_H
#define _HLHT_CONFIG_H


/*�ļ���Ϣ*/
typedef struct SingFileInfo
{
	char	 file_path[30];		    								/* �����ļ�·�� */
	unsigned char File_LineNum[2];					/* �ļ�������·��� */
	unsigned char File_SubSystem[2];	    		/* ��ϵͳ��·�������� (0x0001 ָ ATP,0x0002 ָ ATO) */
	unsigned char File_DataVer[4];						/* 16�ֽڵ����ݰ汾�� ����V-DSU�յ������µ����ݰ汾�ţ����汾�������ʱ����Ҫ��·���ݰ汾�ţ� */
	unsigned char File_SuccessFlag;					/* �ļ�����״̬��־ 0x00:����(��һ���ļ�(��汾��)�������) 0x01:������(���ڴ��䵱ǰ�ļ�(��汾��)) */
}SingleFileInfo;

/* �ļ����Խṹ�� */
typedef struct
{
	unsigned int DataLength;						/* ��֡���ĵĳ�����Ϣ*/
	unsigned short SubSystemType;						/*��ϵͳ������Ϣ*/
	unsigned char File_TrainGroup[2];				/* �ļ������г������ */
	unsigned char File_IsApply;						/* �Ƿ������ļ����� ����֡���� 0x01:���ݰ汾������֡ 0x02:������������֡ 0x03:ȫ�����ݰ汾������֡ */
	unsigned short File_MaxPackLength;				/* �������ʹ�С(���ֽ�Ϊ��λ) */
	unsigned char File_SuccessFlag;					/* �ļ�����״̬��־ 0x01:����(��һ���ļ�(��汾��)�������) 0x02:������(���ڴ��䵱ǰ�ļ�(��汾��)) 0x03:����ʧ��*/
	unsigned char File_FileNum;						/* ��Ҫ�����ļ����� */
	unsigned int File_Size;							/* �ļ���С(���ֽ�Ϊ��λ) */
	unsigned short File_ReadTimes;					/* �ļ���Ҫ��ȡ���� */
	unsigned short File_ReadCurTimes;					/* �ļ��Ѷ�ȡ������0��ʼ���� */
	unsigned int		 File_FramTotalNum;					/* �ļ�������֡�� */
	unsigned int		 File_FramNum;						/* V-DSU�����ļ�֡��� */
	unsigned int		 File_FileContentNum;				/* �����ļ������ֽ��� */
	char		 File_FileContent[10240]; 			/* �����ļ����� */
	SingleFileInfo	Single_FileInfo[50]; 	/*��֡�ļ���Ϣ�ṹ��*/
	unsigned char		 File_Md5Code[16];					/* �ļ����ɵ�MD5�� */
	unsigned int 		 File_CrcCode;
	unsigned int       CalcrcCode;						/*���ؼ����CRC*/
	unsigned char        File_LoadDataAre;					/* 	�Ƿ�����VDSU��NDSU�����������أ�	0x55:�������ߡ��Գ��ߡ������ص�����	0xaa:������ */
	unsigned char        Vdsu_status;						/*	VDSU����ĵ�״̬�Լ�ȫ����������״̬��
														Bit0:Ϊ1��ʾAϵ����������0��ʾ�������ϣ�
														Bit1��Ϊ1��ʾBϵ����������0��ʾ�������ϣ�
														Bit2��Ϊ1��ʾAϵȫ�����ݸ��³ɹ���Ϊ0��ʾδ�ɹ���
														Bit3��Ϊ1��ʾBϵȫ�����ݸ��³ɹ���Ϊ0��ʾδ�ɹ���
														0��Ĭ��ֵ*/

														/*������ϵͳ�ظ�����Ϣ*/
	unsigned int      FramNum;  							/*�����֡���*/
	unsigned char       singalPakageFlag;
	unsigned char       wholePackageFlag;
}CCOV_FileInfo;


typedef struct
{
	unsigned int AtpCrcCode;
	unsigned char AtpIsFlag;
	unsigned int AtoCrcCode;
	unsigned char AtoIsFlag;
}SUBSYS_CRC;

typedef struct
{
	unsigned char LineNum;								/*�յ�ATS��������һ�����е���·���*/
	unsigned char NdsuToCcovFsVer[4];					/*NDSU���͸�CCOV����Ҫ���µ����ݰ汾��*/
	unsigned char NdsuToCcovFsMd5[16];					/*NDSU���͸�CCOV����Ҫ���µ�����Md5��*/

	unsigned char CcovFsSta;							/*CCOV������·�Ų��ұ������ݽ��
													0x01:���ݴ���;0x02���ݲ�����;Ĭ��ֵ0xff*/

	unsigned char HeatStartCmd;							/*0x55:��ʼ�����ȼ���;0xff:Ĭ��ֵ*/
	unsigned char CcovLoadSta;							/*0x01:�ȼ��سɹ�;0X02:�ȼ�����;0x03:�ȼ���ʧ��;0xffĬ��ֵ*/
	unsigned char SwitchStartCmd;						/*���ݿ�ʼ�л�����:0x55��ʼ�л�;0xaa���л�;0xffĬ��ֵ*/

	unsigned char CcovSwitchSta;						/*CCOV�����л�״̬:0x55�л��ɹ�;0xaa�л�ʧ��;0xffĬ��ֵ*/
}SWITCH_STRUCT;

/*1.����ϵͳ��CCOV����֡�ṹ��
  2.CCOV��VDSU����֡�ṹ��*/
typedef struct
{
	unsigned int FramSn;											/*֡���*/
	unsigned char singalPakageFlag;									/*�������³ɹ���־:0x00:���³ɹ�0x01:���³�ʱ0x02:����У��ʧ��0xff:��Чֵ*/
	unsigned char wholePackageFlag;									/*�������ݻظ�ȷ��,0x00:���ճɹ�;0x01:������;0x02:����ʧ��;0xff:��Ч*/
	unsigned int FileSize;										/*���յ����ļ��ֽ���*/
}ASK_STRUCT;

/*1.CCOV������ϵͳ�ظ����ݰ汾�ź���������ṹ��
* 2.VDSU��CCOV�ظ������ݰ汾�ź���������ṹ��*/
typedef struct
{
	unsigned int transFramLength;									/*����֡����,������������*/
	unsigned short trainGroup;										/*�г������*/
	unsigned char dataFramType;										/*����֡����,0x01���ݰ汾������֡;0x02������������֡*/
	unsigned char dataVerState;										/*���ݰ汾�ž���״̬0x01:����;0x02:��ȡ��;0x03:��ȡʧ��*/
	unsigned short lineNum;											/*�ظ������������·���*/
	unsigned short subSystemType;									/*�������ݵ�����ϵͳ����,0x0001:ATP��������;0x0002:ATO��������;����Ԥ��*/
	unsigned int framTotalNum;									/*�ļ�������֡��*/
	unsigned int FramNum;											/*�ļ�����֡���*/
	unsigned int fileContentLength;								/*��֡�ļ����ݵĳ���*/
	unsigned char dataContentBuf[1024];	/*��Ž��յ�������������ʱ����*/
	unsigned char Md5CheckCode[16]; 							/*MD5�ļ�У����*/
	unsigned int crcCode;											/*CRCУ����*/
}REPLY_STRUCT;

#endif  /*_HLHT_CONFIG_H*/
