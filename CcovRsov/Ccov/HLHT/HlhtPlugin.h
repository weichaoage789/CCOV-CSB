/**
  ******************************************************************************
  * @copyright	:
  * @file		        :  HlhtPlugin.h
  * @brief	        :
  * @version	    :  V1.0.0
  * @author       :  Huayu.Hu
  * @date		    :  2020.04.25
  ******************************************************************************
  * @history	    :
  ******************************************************************************
  */

#ifndef _CCOV_HLHT_H
#define _CCOV_HLHT_H

#include "HlhtConfig.h"


#define FS_STORAGE_DIRECTORY    (char *)"/Fsdir"     /*�����ļ�·��*/

/*��������֡�ظ�����*/
#define DATAVER_REPLY_TYPE				0x01			/*���ݰ汾������֡�ظ�����*/
#define DATACONTENT_REPLY_TYPE 			0x02			/*����������������֡�ظ�����*/
#define AllDATAVER_REPLY_TYPE			0x03			/*ȫ�����ݰ汾������֡�ظ�����*/

/*��������֡����*/
#define DATAVER_ASK_TYPE				0x01			/*���ݰ汾������֡����*/
#define DATACONTENT_ASK_TYPE 			0x02			/*����������������֡����*/
#define AllDATAVER_ASK_TYPE				0x03			/*ȫ�����ݰ汾������*֡����*/
#define CONFIRM_TYPE					0x04			/*ȷ�ϻظ�֡����*/
#define DATAVERANDMD5_ASK_TYPE			0x05		/*���ݰ汾�ź�MD5������֡����*/
#define NDSU2ATP_DATAVERANDMD5_REPLY_TYPE 0x06		/*NDSU��ATP���ݰ汾�ź�MD5�ظ�֡����*/
#define DATAVERANDMD5_REPLY_TYPE		0x07		/*���ݰ汾�ź�MD5�ظ�֡����*/

#define DOWNLOAD_FROM_VDSU_LENGTH 		1024

/*����׼��������*/
#define DATA_DOWNLOAD_SUCESS			0x01			/*�������سɹ�*/
#define DATA_DOWNLOADING				0x02			/*����������*/
#define DATA_DOWNLOAD_FAULT				0x03			/*��������ʧ��*/
/*���ݴ�����״̬*/
#define DATA_TRANSLATE_SUCESS			0x00			/*�����������������سɹ�*/
#define DATA_TRANSLATEING				0x01			/*�����������سɹ�*/
#define DATA_TRANSLATE_FALUT			0x02			/*������������������ʧ��*/

/*��ϵͳ��������׶�*/
#define DATA_VERSION_ASK   				0x01			/*���ݰ汾������*/
#define DATA_CONTENT_ASK			    0x02			/*����������������*/
#define DATA_CONTENT_CONFIRM			0x03			/*�������ع�����*/


/*����ϵͳ���ݸ������*/
#define UPDATA_SUCCED					0x00			/*���ݸ��³ɹ�*/
#define SINGAL_FRAM_CHECK_FAULT			0x01			/*��֡У��ʧ��*/


/*����ϵͳ��������*/
#define ATP_SUBSYSTEM_TYPE				0x0001	 /*����ϵͳΪATP����ϵͳ����*/
#define ATO_SUBSYSTEM_TYPE				0x0002	 /*����ϵͳΪATO����ϵͳ����*/
#define CCOV_SUBSYSTEM_TYPE				0x0003   /*����ϵͳΪCCOV����ϵͳ����*/



class CHLHTCtrl
{
private:
	CHLHTCtrl();

public:
	~CHLHTCtrl() {}

	static CHLHTCtrl &Getinstance()
	{
		static CHLHTCtrl instance;
		return instance;
	}

public:
	void SubsysDataHandle(unsigned char *data, unsigned char systype);

private:
	CCOV_FileInfo _Atp1FInfo;
	CCOV_FileInfo _Atp2FInfo;
	CCOV_FileInfo _Atp3FInfo;
	CCOV_FileInfo _Ato1FInfo;
	CCOV_FileInfo _Ato2FInfo;

	/* ATP�������سɹ���־��0x01�����سɹ���0x02�������У�0x03������ʧ�� */
	unsigned char _AtpDLoadState;

	/* ATO�������سɹ���־��0x01�����سɹ���0x02�������У�0x03������ʧ�� */
	unsigned char _AtoDLoadState;
};


#endif  /*_CCOV_HLHT_H*/