#include "GlobalDefine.h"

#include <string.h>
#include "dfsDataRead.h"
#include "dsuRpFunc.h"
#include "dquDataTypeDefine.h"

#define DSURP_PROTCL_SFP   ((unsigned char)1)
#define DSURP_PROTCL_RP    ((unsigned char)2)
#define DSURP_PROTCL_RSR   ((unsigned char)3)

/*�ṹ���ڲ����������ܺ�*/
#define DSU_IP_INFO_STRU_SIZE 22   
#define DSU_COMM_PROTCL_STRU_SIZE 3
#define DSU_CCOV_COMM_INFO_STRU_SIZE 12
/*Դ��FAO��34��Ϊ(34+30)��BY LJ,20170109*/
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO (34+30)
#define DSU_ZCDSU_COMM_INFO_STRU_SIZE (34)


DSU_BOOK_IP_INFO_STRU DsuBookIpInfoStru;     /*��ʼ��֮�����ڱ����ʼ��֮������ݵı���*/

											 /*
											 * ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
											 * ���������unsigned char* FSName�� �ļ�������ָ���ַ
											 pDsuIpInfoStru�� �ֽ�����ͷָ��
											 * ���������pDsuIpInfoStru�� ��ʼ���������
											 * �� �� ֵ��1���ɹ�
											 *           0��ʧ�ܣ������д���
											 *�ϲ�˵������CPK���룬BY LJ,20161210
											 */
static unsigned char dsuIpInit_CPK(char*FSName)
{
	unsigned char chReturnValue = 1; /*����ֵ*/
	unsigned char TempRet;
	unsigned char* pData = nullptr;       /*���ڶ�ȡ���ݵ�ָ��*/
	unsigned int dataLen = 0;        /*���ݳ���*/
	unsigned int dataOff = 0;        /*����ƫ����*/
	unsigned int i;
	unsigned int j;

	/*��Ч�Լ��*/
	if (nullptr == FSName)
	{
		chReturnValue = 0;
	}
	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ����*/
	TempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData, &dataLen);
	if (TempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.IpInfoStruLen = (unsigned short)(dataLen / DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pIpInfoStru = (DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pIpInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pIpInfoStru, 0, sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName = ShortFromChar(pData + dataOff);
			dataOff += 2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;
		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}

	/*��ʼ���豸������ͨ��Э���ϵ��*/
	/*��ȡ����*/
	TempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData, &dataLen);
	if (TempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CommProtclStruLen = (unsigned short)(dataLen / DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCommProtclStru = (DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCommProtclStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pCommProtclStru, 0, sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.CommProtclStruLen; i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType = pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType = pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType = pData[dataOff];
			dataOff++;

		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}


	/*��ʼ��CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	TempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData, &dataLen);
	if (TempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen = (unsigned short)(dataLen / DSU_CCOV_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCcovCommInfoStru = (DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pCcovCommInfoStru, 0, sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;

		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}

	/*��ʼ��ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	TempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData, &dataLen);
	if (TempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.ZcdsuCommInfoLen = (unsigned short)(dataLen / DSU_ZCDSU_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pZcdsuCommInfoStru = (DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pZcdsuCommInfoStru, 0, sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.ZcdsuCommInfoLen; i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum = ShortFromChar(pData + dataOff);
			dataOff += 2;

			for (j = 0; j<DSU_MAX_ZCDSU_COMM_OBJ_NUM; j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j] = ShortFromChar(pData + dataOff);
				dataOff += 2;

			}

		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{

		}
	}

	/*����ִ��*/
	return chReturnValue;
}

/*
* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
* ���������unsigned char* FSName�� �ļ�������ָ���ַ
pDsuIpInfoStru�� �ֽ�����ͷָ��
* ���������pDsuIpInfoStru�� ��ʼ���������
* �� �� ֵ��1���ɹ�
*           0��ʧ�ܣ������д���
*�ϲ�˵����FAO��HLT���ã�BY LJ,20161210
* ��������:201701
* ������:zlj
*/
static unsigned char dsuIpInit_DFT(char*FSName)
{
	unsigned char chReturnValue = 1; /*����ֵ*/
	unsigned char tempRet;
	unsigned char* pData = nullptr;       /*���ڶ�ȡ���ݵ�ָ��*/
	unsigned int dataLen = 0;        /*���ݳ���*/
	unsigned int dataOff = 0;        /*����ƫ����*/
	unsigned int i;
	unsigned int j;
	unsigned char tmpDsuMaxZcDsuCommObjNum = 0;/*��ʱ��¼ZCDSU��Ŀ�����*/
	unsigned char cbtcVersionType = 0;

	/*��Ч�Լ��*/
	if (nullptr == FSName)
	{
		chReturnValue = 0;
	}

	cbtcVersionType = GetCbtcSysType();

	/*��ʼ��ip��ַ�ṹ���*/
	/*��ȡ����*/
	tempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData, &dataLen);
	if (tempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.IpInfoStruLen = (unsigned short)(dataLen / DSU_IP_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pIpInfoStru = (DSU_IP_INFO_STRU*)malloc(sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pIpInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pIpInfoStru, 0, sizeof(DSU_IP_INFO_STRU)*(DsuBookIpInfoStru.IpInfoStruLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
		{
			DsuBookIpInfoStru.pIpInfoStru[i].DevName = ShortFromChar(pData + dataOff);
			dataOff += 2;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			memcpy(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed, pData + dataOff, DSURP_LEN_IP_ADDR);
			dataOff += DSURP_LEN_IP_ADDR;

			DsuBookIpInfoStru.pIpInfoStru[i].SfpBluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;
			DsuBookIpInfoStru.pIpInfoStru[i].SfpRedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pIpInfoStru[i].RpBluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;
			DsuBookIpInfoStru.pIpInfoStru[i].RpRedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pIpInfoStru[i].RsspBluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;
			DsuBookIpInfoStru.pIpInfoStru[i].RsspRedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;
		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}



	/*��ʼ���豸������ͨ��Э���ϵ��*/
	/*��ȡ����*/
	tempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_COMM_PROTCL_ID, &pData, &dataLen);
	if (tempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CommProtclStruLen = (unsigned short)(dataLen / DSU_COMM_PROTCL_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCommProtclStru = (DSU_COMM_PROTCL_STRU*)malloc(sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCommProtclStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pCommProtclStru, 0, sizeof(DSU_COMM_PROTCL_STRU)*(DsuBookIpInfoStru.CommProtclStruLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.CommProtclStruLen; i++)
		{
			DsuBookIpInfoStru.pCommProtclStru[i].EmitterType = pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].ReceptorType = pData[dataOff];
			dataOff++;

			DsuBookIpInfoStru.pCommProtclStru[i].CommuType = pData[dataOff];
			dataOff++;

		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}


	/*��ʼ��CCOVͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	tempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_CCOV_COMM_ID, &pData, &dataLen);
	if (tempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}

	DsuBookIpInfoStru.CcovCommInfoLen = (unsigned short)(dataLen / DSU_CCOV_COMM_INFO_STRU_SIZE);  /*�����������ݵĸ���*/
	DsuBookIpInfoStru.pCcovCommInfoStru = (DSU_CCOV_COMM_INFO_STRU*)malloc(sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pCcovCommInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pCcovCommInfoStru, 0, sizeof(DSU_CCOV_COMM_INFO_STRU)*(DsuBookIpInfoStru.CcovCommInfoLen));
		dataOff = 0;
		/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.CcovCommInfoLen; i++)
		{
			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_VOBC = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_WGB = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_TimeServer = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].DeviceName_NMS = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_BluePort = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pCcovCommInfoStru[i].NMS_RedPort = ShortFromChar(pData + dataOff);
			dataOff += 2;

		}

		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}



	/*��ʼ��ZCDSUͨ�Ź�ϵ���ñ���Ϣ*/
	/*��ȡ����*/
	tempRet = dquGetConfigData((char*)FSName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_ZCDSU_COMM_ID, &pData, &dataLen);
	if (tempRet != 1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
	}


	DsuBookIpInfoStru.ZcdsuCommInfoLen = (unsigned short)(dataLen / DSU_ZCDSU_COMM_INFO_STRU_SIZE_FAO);  /*�����������ݵĸ���*/

	DsuBookIpInfoStru.pZcdsuCommInfoStru = (DSU_ZCDSU_COMM_INFO_STRU*)malloc(sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen)); /*Ϊ�ṹ������ռ�*/
	if (DsuBookIpInfoStru.pZcdsuCommInfoStru == nullptr)
	{
		/*�ռ����ʧ��*/
		chReturnValue = 0;
	}
	else
	{
		/*����ִ��*/
		memset(DsuBookIpInfoStru.pZcdsuCommInfoStru, 0, sizeof(DSU_ZCDSU_COMM_INFO_STRU)*(DsuBookIpInfoStru.ZcdsuCommInfoLen));
		dataOff = 0;

		tmpDsuMaxZcDsuCommObjNum = DSU_MAX_ZCDSU_COMM_OBJ_NUM_FAO;  /*�����������ݵĸ���*/

																	/*ͨ��ѭ�������ݽ������ṹ������У�˳����ṹ�嶨�����Ӧ*/
		for (i = 0; i<DsuBookIpInfoStru.ZcdsuCommInfoLen; i++)
		{
			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName = ShortFromChar(pData + dataOff);
			dataOff += 2;

			DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum = ShortFromChar(pData + dataOff);
			dataOff += 2;

			for (j = 0; j<tmpDsuMaxZcDsuCommObjNum; j++)
			{
				DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf[j] = ShortFromChar(pData + dataOff);
				dataOff += 2;

			}

		}
		if (dataOff != dataLen)
		{
			/*���ݳ��Ȳ��ǽ������ݵ���������˵�����쳣*/
			chReturnValue = 0;
		}
		else
		{
			/*����ִ��*/
		}
	}

	/*����ִ��*/
	return chReturnValue;
}


/*��������: ���ʽӿ�
* �� �� ֵ��1���ɹ��� 0��ʧ��
*/
unsigned char dsuIpInit(char*FSName)
{
	unsigned char rtnValue = 0;
	unsigned char cbtcVersionType = 0;

	/*��Ч�Լ��*/
	if (nullptr != FSName)
	{
		cbtcVersionType = GetCbtcSysType();

		if (DQU_CBTC_CPK == cbtcVersionType)
		{

			rtnValue = dsuIpInit_CPK(FSName);
		}
		else if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType))
		{
			rtnValue = dsuIpInit_DFT(FSName);
		}
		else
		{
			rtnValue = 0;
		}
	}
	else
	{
		rtnValue = 0;
	}

	return rtnValue;
}

/*
* �����������ú������ڻ�ȡ�����ͨ�Žڵ��������Ϣ���ú���ר��������㡣
* ���������unsigned short LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           unsigned short DestDevName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������DSU_RP_LINK_INFO_STRU* pRpLnkInfo,����������ͨ�Žڵ���Ϣ,���в�����Ӧ���
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
unsigned char dsuRpGetLnkInfo(unsigned short LocalName, unsigned short DestName, DSU_RP_LINK_INFO_STRU* pRpLnkInfo)
{
	unsigned char chReturnValue;/*��������ֵ*/
	unsigned short i;/*ѭ����*/

	unsigned char TempEmitterType;/*�������豸����*/
	unsigned char TempReceptorType;/*�������豸����*/

	unsigned char TempCommuType = 0;

	DSU_COMM_PROTCL_STRU *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/

	DSU_IP_INFO_STRU *pTempIpInfoStru;
	unsigned short FindIPCount = 0;  /*���ڱ����ҵ���IP��ַ������FindIPCountΪ2ʱ�����ҽ���*/

							 /*��ȡ����������豸����*/
	TempEmitterType = dsuRpGetDevType(LocalName);
	TempReceptorType = dsuRpGetDevType(DestName);

	pTempCommProtclStru = DsuBookIpInfoStru.pCommProtclStru;
	for (i = 0; i<DsuBookIpInfoStru.CommProtclStruLen; i++)
	{
		if ((pTempCommProtclStru->EmitterType == TempEmitterType)
			&& (pTempCommProtclStru->ReceptorType == TempReceptorType))
		{

			TempCommuType = pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i == DsuBookIpInfoStru.CommProtclStruLen)
	{
		/*û�в��ҵ���Ҫ��ͨ������*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*ͨ��ѭ����������IP��ַ*/
	pTempIpInfoStru = DsuBookIpInfoStru.pIpInfoStru;
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		/*����Դ�豸*/
		if (pTempIpInfoStru->DevName == LocalName)
		{
			/*��ֵIP��ַ*/
			memcpy(pRpLnkInfo->LocalIPBlue, pTempIpInfoStru->IpAddrBlue, sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->LocalIPRed, pTempIpInfoStru->IpAddrRed, sizeof(pTempIpInfoStru->IpAddrRed));

			/*����ͨ�ŵ�Э�������ѱ����ҵ�*/
			/*����Э�����Ͳ�ͬ��ȡ��ͬ�˿ں�*/
			if (TempCommuType == DSURP_PROTCL_RP)
			{
				/*��ǰ�豸ͨ��ʹ��RPЭ��*/
				pRpLnkInfo->LocalPortBlue = pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->LocalPortRed = pTempIpInfoStru->RpRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_RSR)
			{
				/*��ǰ�豸ͨ��ʹ��RpЭ��*/
				pRpLnkInfo->LocalPortBlue = pTempIpInfoStru->RsspBluePort;
				pRpLnkInfo->LocalPortRed = pTempIpInfoStru->RsspRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_SFP)
			{
				/*��ǰ�豸ͨ��ʹ��SFPЭ��*/
				/* �˴�����sfpЭ��ĺ���������ô˺���������ʵ������ʱ�˺��������ߵ���*/
				pRpLnkInfo->LocalPortBlue = pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->LocalPortRed = pTempIpInfoStru->SfpRedPort;

			}
			else
			{
				/*���ݴ���*/
				chReturnValue = 0;
				return chReturnValue;
			}


			/*�ҵ���IP��ַ����1*/
			FindIPCount++;
		}
		else
		{
			/*��ǰIP�豸Name������LocalDevName����ͬ��ʲôҲ����*/
		}

		/*����Ŀ���豸*/
		if (pTempIpInfoStru->DevName == DestName)
		{
			/*��ֵIP��ַ*/
			memcpy(pRpLnkInfo->DestIPBlue, pTempIpInfoStru->IpAddrBlue, sizeof(pTempIpInfoStru->IpAddrBlue));
			memcpy(pRpLnkInfo->DestIPRed, pTempIpInfoStru->IpAddrRed, sizeof(pTempIpInfoStru->IpAddrRed));


			/*����ͨ�ŵ�Э�������ѱ����ҵ�*/
			/*����Э�����Ͳ�ͬ��ȡ��ͬ�˿ں�*/
			if (TempCommuType == DSURP_PROTCL_RP)
			{
				/*��ǰ�豸ͨ��ʹ��RPЭ��*/
				pRpLnkInfo->DestPortBlue = pTempIpInfoStru->RpBluePort;
				pRpLnkInfo->DestPortRed = pTempIpInfoStru->RpRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_RSR)
			{
				/*��ǰ�豸ͨ��ʹ��RpЭ��*/
				pRpLnkInfo->DestPortBlue = pTempIpInfoStru->RsspBluePort;
				pRpLnkInfo->DestPortRed = pTempIpInfoStru->RsspRedPort;

			}
			else if (TempCommuType == DSURP_PROTCL_SFP)
			{
				/*��ǰ�豸ͨ��ʹ��SFPЭ��*/
				/* �˴�����sfpЭ��ĺ���������ô˺���������ʵ������ʱ�˺��������ߵ���*/
				pRpLnkInfo->DestPortBlue = pTempIpInfoStru->SfpBluePort;
				pRpLnkInfo->DestPortRed = pTempIpInfoStru->SfpRedPort;

			}
			else
			{
				/*���ݴ���*/
				chReturnValue = 0;
				return chReturnValue;
			}


			/*�ҵ���IP��ַ����1*/
			FindIPCount++;
		}
		else
		{
			/*��ǰIP�豸Name������DestDevName����ͬ��ʲôҲ����*/
		}

		if (FindIPCount >= 2)
		{
			/*���ҵ�Ŀ���ԴIP��ַ���˳�ѭ��*/
			break;
		}
		else
		{
			/*δȫ�ҵ�Ŀ���IP��ַ����������*/
		}
		pTempIpInfoStru++;
	}

	if (FindIPCount<2)
	{
		/*Ŀ�����ԴIP��ַδ�ҵ�����������ʧ��*/
		chReturnValue = 0;
		return chReturnValue;
	}
	else
	{
		/*���ҵ���Ӧ��ip���豸��Ϣ*/
		chReturnValue = 1;
		return chReturnValue;
	}

}



/*
* ������������ȡ�豸����
* ���������unsigned short DevName �豸��
* ���������
* �� �� ֵ���豸����
*/
unsigned char dsuRpGetDevType(unsigned short DevName)
{
	unsigned char devType;
	devType = (unsigned char)(DevName >> 8);

	return devType;
}

/***********************************************************************
* ������   : dsuRpTypeId2DevName
* �������� : ����Type��ID ȡ�����Ӧ���豸��ʶ
* ������� :
*	������		����			�������		����
*  --------------------------------------------------------------
*	 Type		unsigned char			IN				����
*	 ID			unsigned char			IN				Id
*	 pDevName	unsigned short*			OUT				��ʶ
*
* ����ֵ   : ��
***********************************************************************/
void dsuRpTypeId2DevName(unsigned char Type, unsigned char Id, unsigned short* pDevName)
{
	*pDevName = Type;
	*pDevName = (*pDevName) * 0x100 + Id;
}

/*
* �����������ú������ڸ��ݱ����Լ��Է����豸���ͻ�ȡ��ʹ�á�
* ���������unsigned short LocalName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
*           unsigned short DestName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
* ���������unsigned char* protclTye Э������
* �� �� ֵ��1���ɹ�
*           0��ʧ��
*/
unsigned char dsurRpGetProtclType(unsigned short LocalName, unsigned short DestName, unsigned char* protclTye)
{
	unsigned char chReturnValue;/*��������ֵ*/
	unsigned short i;/*ѭ����*/

	unsigned char TempEmitterType;/*�������豸����*/
	unsigned char TempReceptorType;/*�������豸����*/

	DSU_COMM_PROTCL_STRU *pTempCommProtclStru;/*ͨ��Э�����ʱ����*/

											  /*��ȡ����������豸����*/
	TempEmitterType = dsuRpGetDevType(LocalName);
	TempReceptorType = dsuRpGetDevType(DestName);

	pTempCommProtclStru = DsuBookIpInfoStru.pCommProtclStru;
	for (i = 0; i<DsuBookIpInfoStru.CommProtclStruLen; i++)
	{
		if ((pTempCommProtclStru->EmitterType == TempEmitterType) && (pTempCommProtclStru->ReceptorType == TempReceptorType))
		{

			*protclTye = pTempCommProtclStru->CommuType;
			break;
		}
		pTempCommProtclStru++;
	}

	if (i == DsuBookIpInfoStru.CommProtclStruLen)
	{
		/*û�в��ҵ���Ҫ��ͨ������*/
		chReturnValue = 0;
		return chReturnValue;
	}

	chReturnValue = 1;
	return chReturnValue;
}

/*
* ������������ѯ�豸IP
* ���������unsigned short wLocalID���豸id
unsigned char chLocalType���豸����
* ���������unsigned int * RedIPInfo������ip
unsigned int * BlueIPInfo������ip
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
unsigned char CheckIPInfoOfObject(unsigned short wLocalID, unsigned char chLocalType, unsigned int * RedIPInfo, unsigned int * BlueIPInfo)
{
	unsigned char chReturnValue;/*��������ֵ*/
	unsigned short i;/*ѭ����*/
	unsigned short tempDevName; /*��ʱ����devName*/

	if (wLocalID>255)
	{
		/*�趨���������Ͳ�������Ҫ�����ش���*/
		chReturnValue = 0;
		return chReturnValue;
	}

	/*����dev����*/
	tempDevName = (unsigned short)(((chLocalType << 8) + wLocalID) & 0xffff);

	/*ѭ�������豸ip��ַ*/
	for (i = 0; i<DsuBookIpInfoStru.IpInfoStruLen; i++)
	{
		if (DsuBookIpInfoStru.pIpInfoStru[i].DevName == tempDevName)
		{
			/*���ҵ���ǰ�豸*/
			*RedIPInfo = LongFromChar(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrRed);
			*BlueIPInfo = LongFromChar(DsuBookIpInfoStru.pIpInfoStru[i].IpAddrBlue);
			break;
		}
		else
		{
			/*��������*/
		}
	}

	if (i == DsuBookIpInfoStru.IpInfoStruLen)
	{
		/*δ�ҵ��豸*/
		chReturnValue = 0;
	}
	else
	{
		/*���ҵ��豸*/
		chReturnValue = 1;
	}

	return chReturnValue;

}
/*
* �������������뱾ZC��ID�������ZC����ͨ�Ŷ�����Ϣ��������ZC��DSU��ATS��CI��ʱ�ӷ������ȡ�
* ���������unsigned short wLocalID�����豸id
unsigned char chLocalType���豸����
* ���������unsigned int * wObjectNum, ͨ�Ŷ�������
unsigned short *pwObjectInfo,ͨ�Ŷ�������Ϣ������ID��Ϣ��������Ϣ
* �� �� ֵ����ѯ�ɹ�������1�� 	��ѯʧ�ܣ�����0��
*/
unsigned char CheckCommObjectInfo(unsigned short wLocalID, unsigned char chLocalType, unsigned int * wObjectNum, unsigned short *pwObjectInfo)
{
	unsigned short localName;  /*���ݱ����豸id��type����������豸����*/
	unsigned short i;
	unsigned char bRet;

	if (wLocalID>255)
	{
		/*�趨���������Ͳ�������Ҫ�����ش���*/
		bRet = 0;
		return bRet;
	}

	/*�����豸type���豸id�����豸����*/
	localName = (chLocalType << 8) + (wLocalID & 0xff);

	/*ѭ�������豸*/
	for (i = 0; i<DsuBookIpInfoStru.ZcdsuCommInfoLen; i++)
	{
		if (DsuBookIpInfoStru.pZcdsuCommInfoStru[i].LocalDevName == localName)
		{
			/*�ҵ��豸�������������*/
			*wObjectNum = DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjNum;
			memcpy(pwObjectInfo, DsuBookIpInfoStru.pZcdsuCommInfoStru[i].CommObjBuf, (*wObjectNum) * 2);

			/*�������سɹ�*/
			break;

		}
	}

	/*ѭ���������ж��Ƿ��ҵ��豸*/
	if (i == DsuBookIpInfoStru.ZcdsuCommInfoLen)
	{
		/*û���ҵ��豸������ʧ��*/
		bRet = 0;
	}
	else
	{
		/*�ҵ��豸�����سɹ�*/
		bRet = 1;
	}

	return bRet;
}
